#include "room_info.h"
#include "sy_utils.h"
#include "sy_logger.h"

#include <QHostAddress>
#include <QByteArray>




room_info::room_info(QObject *parent) : data_info(parent),
    m_room_thread(nullptr), m_room_signal_sync_thread(nullptr), workerThread(nullptr), buff(nullptr)
{
}

room_info::~room_info()
{
    stop();
}

void room_info::start()
{
    qDebugxx(qtr("启动场景"), get_name());
    if(m_room_thread != nullptr) {
        return;
    }
    m_room_thread = new room_thread((data_info *)this);
    connect(this, &room_info::clear_window_signal, m_room_thread, &room_thread::clear_window_signal);
    connect(this, &room_info::add_signal_source_signal, m_room_thread, &room_thread::add_signal_source_signal);
    //二期
    connect(m_room_thread, &room_thread::polling_status_signal, this, &room_info::send_polling_status_signal_slot);
    connect(m_room_thread, &room_thread::get_resolution, this, &room_info::send_get_resolution_slot);

    m_room_thread->start();
    //二期开启信号同步线程
    qDebugxx(qtr("启动信号同步"), get_name());
    if(m_room_signal_sync_thread != nullptr) {
        return;
    }
    m_room_signal_sync_thread = new room_signal_sync_thread((data_info *)this);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_win_open_signal, this, &room_info::sync_win_open_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_win_close_signal, this, &room_info::sync_win_close_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_win_close_all_signal, this, &room_info::sync_win_close_all_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_win_move_signal, this, &room_info::sync_win_move_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_win_level_signal, this, &room_info::sync_win_level_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_win_change_signal, this, &room_info::sync_win_change_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_mode_change_signal, this, &room_info::sync_mode_change_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_mode_add_signal, this, &room_info::sync_mode_add_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_mode_edit_signal, this, &room_info::sync_mode_edit_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_mode_remove_signal, this, &room_info::sync_mode_remove_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_audio_open_signal, this, &room_info::sync_audio_open_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_audio_close_signal, this, &room_info::sync_audio_close_signal_slot);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::sync_audio_set_signal, this, &room_info::sync_audio_set_signal_slot);

    connect(m_room_signal_sync_thread, &room_signal_sync_thread::disconnect_signal, this, &room_info::disconnect_signal_slot);
     connect(m_room_signal_sync_thread, &room_signal_sync_thread::reconnect_signal, this, &room_info::reconnect_signal_slot);

    m_room_signal_sync_thread->start();
    //二期开启处理字节流的线程
    buff = new room_databuff();
    workerThread = new QThread();
    buff->moveToThread(workerThread);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    connect(workerThread, &QThread::finished, buff, &room_databuff::deleteLater);
    connect(m_room_signal_sync_thread, &room_signal_sync_thread::msgCome, buff, &room_databuff::spliceData);
    connect(buff, &room_databuff::canSend, m_room_signal_sync_thread, &room_signal_sync_thread::canSend);
    workerThread->start();
    //二期开启处理串口的线程
//    buff_c=new room_com();
//    workerThread_com =new QThread();
//    buff_c->moveToThread(workerThread_com);
//    connect(workerThread_com, &QThread::finished, workerThread_com, &QThread::deleteLater);
//    connect(workerThread_com, &QThread::finished, buff_c, &room_com::deleteLater);
//    connect(this, &room_info::write_to_serial_signal, buff_c, &room_com::operation_direction);
//    connect(this, &room_info::write_to_serial_signal, buff_c, &room_com::operation_zoom);
//    connect(this, &room_info::close_serial_signal, buff_c, &room_com::closeSerialPort);
//    workerThread_com->start();
}

void room_info::stop()
{
    if(m_room_thread != nullptr) {
        m_room_thread->quit();
        m_room_thread->wait();
        delete m_room_thread;
        m_room_thread = nullptr;
    }
    data_info::stop();
    qDebugxx(qtr("停止场景连接"), get_name());
    //二期 停止
    if(m_room_signal_sync_thread != nullptr) {
        m_room_signal_sync_thread->quit();
        m_room_signal_sync_thread->wait();
        delete m_room_signal_sync_thread;
        m_room_signal_sync_thread = nullptr;
    }
    data_info::stop();
    qDebugxx(qtr("停止信号同步"), get_name());
    //二期 字节流线程停止
    if(workerThread) {
        workerThread->quit();
        workerThread->wait();
        buff = nullptr;
        workerThread = nullptr;
    }
    data_info::stop();
    qDebugxx(qtr("停止字节流线程"), get_name());
    //二期 串口线程停止
//    if(workerThread_com)
//    {
//    workerThread_com->quit();
//    workerThread_com->wait();
//    buff_c=nullptr;
//    workerThread_com=nullptr;
//    }
//    data_info::stop();
//    qDebugxx(qtr("停止串口线程"), get_name());
}

void room_info::sync_win_open_signal_slot(QSharedPointer<data_info> val)
{
    emit sync_win_open_signal(val); //当前场景发出
}

void room_info::sync_win_close_signal_slot(int win_id)
{
    emit sync_win_close_signal(win_id);
}

void room_info::sync_win_close_all_signal_slot()
{
    emit sync_win_close_all_signal();
}

void room_info::sync_win_move_signal_slot(QSharedPointer<data_info> val)
{
    emit sync_win_move_signal(val);
}

void room_info::sync_win_level_signal_slot(int win_id, int hint)
{
    emit sync_win_level_signal(win_id, hint);
}

void room_info::sync_win_change_signal_slot(QSharedPointer<data_info> val)
{
    emit sync_win_change_signal(val);
}

void room_info::send_polling_status_signal_slot(int index)
{
    emit polling_status_signal(index);
}

void room_info::sync_mode_change_signal_slot(QString id)
{
    emit sync_mode_change_signal(id);
}

void room_info::sync_mode_remove_signal_slot(QString id)
{
    emit sync_mode_remove_signal(id);
}

void room_info::sync_mode_add_signal_slot(QSharedPointer<data_info> val)
{
    emit sync_mode_add_signal(val);
}


void room_info::sync_mode_edit_signal_slot(QSharedPointer<data_info> val)
{
    emit sync_mode_edit_signal(val);
}

void room_info::sync_audio_open_signal_slot(QString sig_id, int channel, int vol, int au_id)
{
    emit sync_audio_open_signal(sig_id, channel, vol, au_id);
}

void room_info::sync_audio_close_signal_slot(int au_id)
{
    emit sync_audio_close_signal(au_id);
}

void room_info::sync_audio_set_signal_slot(int au_id, int vol)
{
    emit sync_audio_set_signal(au_id, vol);
}

void room_info::disconnect_signal_slot()
{
    emit disconnect_signal();
}

void room_info::reconnect_signal_slot()
{
    emit reconnect_signal();
}

void room_info::send_get_resolution_slot(QString str)
{
    emit get_resolution(str);
}




