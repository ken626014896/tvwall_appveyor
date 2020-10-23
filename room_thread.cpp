#include "room_thread.h"
#include "room_worker.h"


room_thread::room_thread(data_info *data, QObject *parent) : QThread(parent),
    m_data_info(data)
{

}

void room_thread::run()
{
    room_worker work(m_data_info);
    connect(this, &room_thread::clear_window_signal, &work, &room_worker::clear_window_slot);
    connect(this, &room_thread::add_signal_source_signal, &work, &room_worker::add_signal_source_slot);
//    connect(&work, &room_worker::image_incoming_signal, this, &room_thread::image_incoming_signal);
    //二期返回轮巡结果状态
    connect(&work,&room_worker::polling_status_signal,this,&room_thread::polling_status_signal);
    connect(&work,&room_worker::get_resolution,this,&room_thread::get_resolution);

    work.start();
    this->exec();
    work.stop();
}
