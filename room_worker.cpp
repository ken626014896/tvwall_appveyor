﻿#include "room_worker.h"
#include "sy_utils.h"
#include "sy_logger.h"
#include "signal_resource_info.h"
#include <qmath.h>
#include <QHostAddress>
#include <QtDebug>
#include <QTextCodec>
#include <QtTest/QTest>
#ifdef WIN32
    #include <qt_windows.h>
#endif


#define RES_UNKNOWN		-1
#define RES_NULL			0
#define RES_WALL			1
#define RES_SIGNAL			2
#define RES_SCENE			3
#define RES_SEQUENCE		4
#define RES_MATRIX		5
#define RES_USER			6
#define RES_GROUP			7
#define RES_OPERATION		8
#define RES_TASK			9
#define RES_FTEXT			10

#define MAX_CMD_LEN 256


//ptz定义：
#define CODE_PTZ_UP		    1
#define CODE_PTZ_DOWN		2
#define CODE_PTZ_LEFT		3
#define CODE_PTZ_RIGHT		4
#define CODE_ZOOM_IN		5
#define CODE_ZOOM_OUT		6

#pragma pack(push, 1)
typedef struct {
    int len;
    int id;
    int cmd;
} CMD_HEADER;

typedef struct {
    int id;
    int status;
    int connect_id;
} LOGIN_RESPONSE, *PLOGIN_RESPONSE;

typedef struct {
    int id;
    int status;
    short version;
    unsigned char com_count;
    unsigned char audio_count;
    unsigned char reserve[6];
    unsigned char row;
    unsigned char column;
    short width;
    short height;
} SYS_INFO, *PSYS_INFO;

typedef struct {
    int id;
    int status;
    int count;
} SIGNAL_HEADER, *PSIGNAL_HEADER;

typedef struct {
    int type;
    char id[16];
    int len;
} SIGNAL_RESOURCE_FIRST, *PSIGNAL_RESOURCE_FIRST;

typedef struct {
    int type;
    char cmd[MAX_CMD_LEN];
} SIGNAL_RESOURCE_SECOND, *PSIGNAL_RESOURCE_SECOND;

//typedef struct{
//    unsigned char name_len;

//} SIGNAL_RESOURCE_IPC, *PSIGNAL_RESOURCE_IPC;

typedef struct {
    char id[16];
    int x;
    int y;
    int width;
    int height;
    int win_unknow;
    unsigned int win_id;
    int signal_type;
} WALL_WIN_INFO, *PWALL_WIN_INFO;

typedef struct {
    CMD_HEADER head;
    int wall_id;
    char id[16];
    int x;
    int y;
    int width;
    int height;
    int win_unknow;
} WIN_OPEN, *PWIN_OPEN;

typedef struct {
    int id;
    int status;
    unsigned int win_id;
} WIN_OPEN_RESPONSE, *PWIN_OPEN_RESPONSE;

typedef struct {
    CMD_HEADER head;
    unsigned int win_id;
    int x;
    int y;
    int width;
    int height;
} WIN_UPDATE, *PWIN_UPDATE;

typedef struct {
    int id;
    int status;
} WIN_UPDATE_RESPONSE, *PWIN_UPDATE_RESPONSE;

typedef struct {
    CMD_HEADER head;
    unsigned int win_id;
} WIN_CLOSE, *PWIN_CLOSE;

typedef struct {
    CMD_HEADER head;
    int node_type;
} GET_NODE, *PGET_NODE;

typedef struct {
    int id;
    int status;
    unsigned char count;
} NODE_HEADER, *PNODE_HEADER;

typedef struct {
    CMD_HEADER head;
    int type;
    char groud_id[16];
    char source_id[16];
} RESOURCE_DELETE, *PRESOURCE_DELETE;
typedef struct {
    int id;
    int status;
    int len;
    char groud_id[16];
    int len_name;
} RESOURCE_GROUP_FIRST, *PRESOURCE_GROUP_FIRST;

typedef struct {
    int type;
    int count;
} RESOURCE_GROUP_SECOND, *PRESOURCE_GROUP_SECOND;

typedef struct {
    char groud_id[16];
    int flag;
} RESOURCE_GROUP_LEAF, *PRESOURCE_GROUP_LEAF;

typedef struct {
    CMD_HEADER head;
    unsigned int win_id;
    int level;
} WIN_LEVEL_CHANGED, *PWIN_LEVEL_CHANGED;

typedef struct {  //二期新增 轮询指令
    int len;
    int id;
    int cmd;
} WIN_POLLING, *PWIN_POLLING;

typedef struct { //二期新增 轮询响应
    int  id;
    int status;
    int count;

} WIN_POLLING_RESPONSE, *PWIN_POLLING_RESPONSE;

typedef struct {  //二期新增 操作轮询的返回结果的数据结构

    int id;
    int status;

} SET_POLLING_RESPONSE, *PSET_POLLING_RESPONSE;
typedef struct {  //二期新增 操作轮询的数据结构
    CMD_HEADER head;
    unsigned char index;
    unsigned char op;


} SET_POLLING, *PSET_POLLING;


typedef struct {  //测试二期新增获取场景
    char groud_id[16];
    int len;

} TEST_RESPONSE, *PTEST_RESPONSE;//二期新增获取信号模式
typedef struct {
    int num;
} NUM, *PNUM;
typedef struct {
    int id;
    int status;
    int count;
} MODE_HEADER, *PMODE_HEADER;

typedef struct {
    int cdSize; //整个结构体的大小
    char id[16]; //guid
    int len;  //单个场景名称的长度
} MODE_RESOURCE_FIRST, *PMODE_RESOURCE_FIRST;
typedef struct tagWIN {
    char signal_id[16];
    //WINPOSITION
    int left;
    int top;
    unsigned int width;
    unsigned int height;
    int style;
    unsigned int win_id;
    int signal_type;
} WIN, *PWIN;

typedef struct tagAUDIO {
    char signal_id[16];
    int channel;
    int opt;//volume;
    unsigned int play_id;
} AUDIO, *PAUDIO;

typedef struct {        //发送信号源变化的结构
    CMD_HEADER head;
    int wall_id;
    char id[16];
} SIG_CHANGE, *PSIG_CHANGE;

typedef struct {    //接受信号源变化的结构
    int id;
    int status;
} SIG_CHANGE_RESPONSE, *PSIG_CHANGE_RESPONSE;
typedef struct {
    int id;
    int status;

} SYNC_RESPONSE, *PSYNC_RESPONSE;


typedef struct { //字幕响应
    int id;
    int status;

} CAPTION_RESPONSE, *PCAPTION_RESPONSE;


typedef struct { //打开音频的响应结构体
    int id;
    int status;
    int audio_id;

} OPEN_AUDIO_RESPONSE, *POPEN_AUDIO_RESPONSE;

#pragma pack(pop)


room_worker::room_worker(data_info *data, QObject *parent) : QObject(parent),
    m_tcp_socket(nullptr),
    m_packet_id(1),
    m_data_info(data),
    m_group_id("02000000000000000000000000000000"),
    m_group_id_mode("03000000000000000000000000000000"),
    m_timer(nullptr),
    m_signal_source_index(0),
    m_accept_msg_hint(0),
    isFirstConnect(true)
{
}

room_worker::~room_worker()
{
    stop();
}


void room_worker::start()
{
    if(m_tcp_socket != nullptr) {
        return;
    }
//    qDebug()<< QByteArray::fromHex(m_group_id_mode.toLatin1()).toHex();
    qDebugxxxx(qtr("连接场景"), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());
    m_tcp_socket = new QTcpSocket();
    connect(m_tcp_socket, &QTcpSocket::readyRead, this, &room_worker::recv_data);
    connect(m_tcp_socket, &QTcpSocket::connected, this, &room_worker::connected);
    connect(m_tcp_socket, &QTcpSocket::disconnected, this, &room_worker::disconnected_status);
    if(isFirstConnect) {
        //二期新增，连接操作轮询的信号与曹
        connect(m_data_info, &data_info::open_polling, this, &room_worker::open_polling_slot);
        connect(m_data_info, &data_info::stop_polling, this, &room_worker::stop_polling_slot);
        connect(m_data_info, &data_info::stop_and_open_polling, this, &room_worker::open_and_stop_polling_slot);
        //二期新增,连接操作字幕的信号与曹
        connect(m_data_info, &data_info::caption_signal, this, &room_worker::change_caption_slot);
        //二期 刷新窗口
        connect(m_data_info, &data_info::fresh_win_signal, this, &room_worker::fresh_win_slot);
        //触发同步
        connect(m_data_info, &data_info::changed_mode_signal, this, &room_worker::mode_chanaged_slot);
        connect(m_data_info, &data_info::changed_mode_signal_int, this, &room_worker::mode_chanaged_slot_int);
        connect(m_data_info, &data_info::add_mode_signal, this, &room_worker::mode_add_slot);
        connect(m_data_info, &data_info::remove_mode_signal, this, &room_worker::mode_remove_slot);
        connect(m_data_info, &data_info::edit_mode_signal, this, &room_worker::mode_edit_slot);
        //云台控制
        connect(m_data_info, &data_info::ptz_signal, this, &room_worker::ptz_slot);
        //音频控制
        connect(m_data_info, &data_info::open_audio_signal, this, &room_worker::open_audio_slot);
        connect(m_data_info, &data_info::close_audio_signal, this, &room_worker::close_audio_slot);
        connect(m_data_info, &data_info::set_audio_signal, this, &room_worker::set_audio_slot);
        //获取信号墙信息
        connect(m_data_info, &data_info::get_wall_signal, this, &room_worker::get_wall_signal_slot);
    }
    //    qDebug()<<"ip:"<<m_data_info->get_ip_address()<<":"<<m_data_info->get_port();
    m_tcp_socket->connectToHost(QHostAddress(m_data_info->get_ip_address()), m_data_info->get_port());
    if(m_timer == nullptr) {
        m_timer = new QTimer();
        connect(m_timer, &QTimer::timeout, this, &room_worker::time_reconnect);
    } else {
        m_timer->stop();
    }
    if(isFirstConnect) {
        create_connect_for_signal_source(m_data_info->get_list_signal_source());
        m_packet_id = 1;
    }
}

void room_worker::create_connect_for_signal_source(QList<QSharedPointer<data_info>> list)
{
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        create_connect_for_signal_source(info);
        if(info->get_device_type() == 0x07) {
            create_connect_for_signal_source(info->get_list_signal_source());
        }
    }
}

void room_worker::get_polling()
{
    //发送指令  获取轮巡列表
    if(m_tcp_socket == nullptr) {
        return;
    }
    int a = RES_SEQUENCE;
    QByteArray data;
    add_cmd_header(data, 0x10, 4 + 4 + 4);
    add_data_to_byte_array(data, (char *)&a, sizeof ( int));
    m_tcp_socket->write(data);
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("轮询失败，重新登录"));
        reconnected();
        return;
    }
    qDebugxx(qtr("查询轮询返回"), QString(res.toHex()));
    PWIN_POLLING_RESPONSE polling = (PWIN_POLLING_RESPONSE)res.constData();
    m_data_info->clear_polling();
    for (int i = 0; i < polling->count; i++) { //循环获取场景
        QSharedPointer<data_info> info(new data_info());
        info->set_name(qtr("轮巡:") + QString::number(i));
        info->set_index(i);
        m_data_info->add_polling(info);
        if(info.isNull()) {
            qWarnx(qtr("未正确创建对象"));
            continue;
        }
    }
    m_data_info->send_polling_changed_event();
}

void room_worker::get_mode() //从场景中获取信号模式
{
    if(m_tcp_socket == nullptr) {
        return;
    }
    QByteArray data;
    int a = 3;
    add_cmd_header(data, 0x10, 4 + 4 + 4);
    add_data_to_byte_array(data, (char *)&a, sizeof ( int));
//    qDebug()<<data.toHex();
    m_tcp_socket->write(data);
    QByteArray res = get_response();
//    qDebug()<<res.toHex();
    if(res.count() <= 0) {
        qWarnx(qtr("查询信号模式失败，重新登录"));
//        reconnected();
        return;
    }
    PMODE_HEADER mode = (PMODE_HEADER)res.constData();
    if(mode->count <= 0) {
        qWarnx(qtr("未配置信号模式源"));
        return;
    }
//    qDebug()<<res.toHex();
    qDebugxx(qtr("获取到信号模式源"), mode->count);
    QList<QSharedPointer<data_info>> list_mode;
    int pos = sizeof (MODE_HEADER);
    int index = 0;
//    qDebug()<<mode->count;
    for (int i = 0; i < mode->count; i++) { //得到不同的信号模式

        QSharedPointer<data_info> info(new data_info());
        PMODE_RESOURCE_FIRST first = (PMODE_RESOURCE_FIRST)(res.constData() + pos);
        QByteArray  sig_byte = res.mid(pos, first->cdSize);
        QString id = QString(QByteArray(first->id, sizeof(first->id)).toHex());
        QString scene_name = QString(res.mid(pos + sizeof (MODE_RESOURCE_FIRST), first->len));

//        qDebug()<<res.mid(pos + sizeof (MODE_RESOURCE_FIRST), first->len).toHex();
        info->set_id(id);  //信号模式id
        info->set_name(scene_name);
        info->set_logical_wall_row(2);
        info->set_logical_wall_column(2);
        if(index <= 5) {
            info->set_ico_path(QString("qrc:/images/mode/img-%1.jpg").arg(index));
        } else {
            index = 0;
            info->set_ico_path(QString("qrc:/images/mode/img-%1.jpg").arg(index));
        }
        index++;
        info->clear_signal_source_in_tv_wall();
        //获取信号窗口
        PNUM num_st = (PNUM)(sig_byte.constData() + sizeof (MODE_RESOURCE_FIRST) + first->len);
        int win_pos = sizeof (MODE_RESOURCE_FIRST) + first->len + 4;
        for (int j = 0; j < num_st->num; j++) {
            PWIN win_st = (PWIN)(sig_byte.constData() + win_pos);
            QSharedPointer<data_info> sig(new signal_resource_info());
            QString id = QString(QByteArray(win_st->signal_id, sizeof(win_st->signal_id)).toHex());
            QSharedPointer<data_info> old = sig->get_old_data_info();
            if(old.isNull()) {
                old = m_data_info->get_signal_source_by_id(id);
            }
            sig->set_old_data_info(old);
            sig->set_display_x(win_st->left * 1.0 / m_data_info->get_wall_width());
            sig->set_display_y(win_st->top * 1.0 / m_data_info->get_wall_height());
            sig->set_display_width(win_st->width * 1.0 / m_data_info->get_wall_width());
            sig->set_display_height(win_st->height * 1.0 / m_data_info->get_wall_height());
            sig->set_id(id);
//            qDebug()<<win_st->width*1.0<<m_data_info->get_wall_width();
            sig->set_win_id(win_st->win_id);
//            qDebug()<<win_st->win_id;
            info->add_signal_source_in_tv_wall(sig);
            win_pos = win_pos + sizeof (WIN);
        }
        //获取音频   暂时不处理
//        PNUM num_st2 = (PNUM)(sig_byte.constData() + win_pos);
//        qDebug() << "video num:" << num_st2->num;
//        qDebugxx(qtr("video num:"), num_st2->num);
//        int audio_pos = win_pos + 4;
//        for (int j = 0; j < num_st2->num; j++) {
//            PAUDIO audio_st = (PAUDIO)(sig_byte.constData() + audio_pos);
//            QSharedPointer<data_info> a(new data_info()); //音频
//            QString id = QString(QByteArray(audio_st->signal_id, sizeof(audio_st->signal_id)).toHex());
//            a->set_id(id);
//            a->set_audio_channel(audio_st->channel);
//            a->set_volume(audio_st->opt);
//            a->set_audio_id(audio_st->play_id);
//            info->add_audio_list(a);       //不同信号模式存自己的音频
//            m_data_info->add_audio_list(a);//暂时将不同信号模式中的音频都放在同一个场景中
//            audio_pos = audio_pos + sizeof (AUDIO);
//        }
        pos = pos + first->cdSize;
        list_mode.append(info);
    }
    m_data_info->append_signal_mode(list_mode);
    m_data_info->send_mode_changed_event();
    m_tcp_socket->readAll();
}

int room_worker::hexToTen(QString hex)
{
    bool ok;
    QString hex2 = hex.mid(2, 2) + hex.mid(0, 2);
    int dec = hex2.toInt(&ok, 16);
    return dec;
}

void room_worker::stop_polling_slot(int index)
{
//    qDebug()<<"stop"<<index;
    char num = (char)index; //会自己转换为16进制的形式
    char cmd = 0x00;
    QByteArray data;
    add_cmd_header(data, 0x50, 4 + 4 + 1 + 1);
    add_data_to_byte_array(data, (char *)&num, sizeof ( char));
    add_data_to_byte_array(data, (char *)&cmd, sizeof ( char));
//    qDebug()<<"stop polling"<<QString(data.toHex());
    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
//    QByteArray res = get_response();
//    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
//    if(rv->status != 0){
//        qWarnxx(qtr("停止轮巡失败"), rv->status);
//        return;
//    }
    emit polling_status_signal(-1);
    m_tcp_socket->readAll();
}
void room_worker::open_polling_slot(int index)
{
    //开启前先暂停全部轮巡比较安全
    QVariantList list = m_data_info->get_list_polling();
    for(int i = 0; i < list.length(); i++) {
        stop_polling_slot(i);
    }
//    stop_polling_slot(index);
    qDebug() << "open polling";
    char num = (char)index; //会自己转换为16进制的形式
    char cmd = 0x01;
    QByteArray data;
    add_cmd_header(data, 0x50, 4 + 4 + 1 + 1);
    add_data_to_byte_array(data, (char *)&num, sizeof ( char));
    add_data_to_byte_array(data, (char *)&cmd, sizeof ( char));
//    qDebug()<<"begain polling"<<QString(data.toHex());
    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
//    QByteArray res = get_response();
//    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
//    if(rv->status != 0){
//        qWarnxx(qtr("打开轮巡失败"), rv->status);
//        return;
//    }
//    qWarnxx(qtr("打开轮巡成功"), rv->status);
    emit polling_status_signal(index);
    m_tcp_socket->readAll();
}
void room_worker::open_and_stop_polling_slot(int o_index, int s_index)
{
    //先关闭再打开
    stop_polling_slot(s_index);
    open_polling_slot(o_index);
}

void room_worker::sync()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
    QByteArray data;
    add_cmd_header(data, 0x03, 4 + 4 + 4);
    int cmd = 1;
    add_data_to_byte_array(data, (char *)&cmd, sizeof(int));
//    qDebug()<<"heollo";
//    qDebug()<<data.toHex();
    qDebugxx(qtr("开启同步指令"), QString(data.toHex()));
    m_tcp_socket->write(data);
}



void room_worker::time_reconnect()
{
    start();
}



void room_worker::stop()
{
//    logout();
    if(m_tcp_socket != nullptr) {
        m_tcp_socket->close();
        delete m_tcp_socket;
        m_tcp_socket = nullptr;
    }
    if(m_timer != nullptr) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    qDebugxxxx(qtr("停止连接场景"), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());
}

void room_worker::clear_window_slot()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
    WIN_CLOSE win;
    memset(&win, 0, sizeof(WIN_CLOSE));
    win.head.id = m_packet_id ++;
    win.head.cmd = 0x23;
    win.head.len = sizeof (WIN_CLOSE) - 4;
    m_data_info->set_isMe(true);
    m_tcp_socket->write((const char *)&win, sizeof(WIN_CLOSE));
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("清除窗口收取数据失败，重新登录"));
        reconnected();
        return;
    }
    PWIN_UPDATE_RESPONSE rv = (PWIN_UPDATE_RESPONSE)res.constData();
    if(rv->status != 0) {
        qWarnxx(qtr("清除窗口失败"), rv->status);
        return;
    }
    qDebugxx(qtr("清除窗口成功"), QString(m_tcp_socket->readAll().toHex()));
}

void room_worker::add_signal_source_slot(QSharedPointer<data_info> val)
{
    QByteArray data;
    int len = sizeof (CMD_HEADER) - 4 + 4 + 16 + sizeof (SIGNAL_RESOURCE_FIRST) + val->get_name().toUtf8().length() + sizeof (SIGNAL_RESOURCE_SECOND);
    add_cmd_header(data, 0x12, len);
    int type = 0x02;
    add_data_to_byte_array(data, (char *)&type, sizeof (int));
    QByteArray ba_group = QByteArray::fromHex(m_group_id.toLatin1());
    add_data_to_byte_array(data, ba_group.data(), ba_group.length());
    SIGNAL_RESOURCE_FIRST first;
    first.len = val->get_name().toUtf8().length();
    first.type = val->get_device_type() == 5 ? 0x120 : 0x121;
    memcpy(first.id, QByteArray::fromHex(val->get_id().toLatin1()).constData(), sizeof (first.id));
    add_data_to_byte_array(data, (char *)&first, sizeof (SIGNAL_RESOURCE_FIRST));
    add_data_to_byte_array(data, (char *)val->get_name().toUtf8().data(), val->get_name().toUtf8().length());
    SIGNAL_RESOURCE_SECOND sec;
    sec.type = val->get_device_type();
    QString cmd = QString();//QString("%1:%2").arg(val->get_ip_address()).arg(val->get_channel_no());
    if(sec.type == 1) {
        cmd = val->get_ip_address();
    } else if(sec.type == 5) {
        cmd = QString("%1:%2").arg(val->get_ip_address()).arg(val->get_channel_no());
    } else {
        qWarnxx(qtr("不支持的信号源类型"), sec.type);
        return;
    }
#ifdef WIN32
    memcpy_s(sec.cmd, sizeof(sec.cmd), cmd.toLocal8Bit(), cmd.toLocal8Bit().length());
#else
    memcpy(sec.cmd, cmd.toLocal8Bit(), cmd.toLocal8Bit().length());
#endif
    add_data_to_byte_array(data, (char *)&sec, sizeof (SIGNAL_RESOURCE_SECOND));
    m_tcp_socket->write(data.constData(), data.length());
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("添加信号源收取数据失败，重新登录"));
        reconnected();
        return;
    }
    PWIN_UPDATE_RESPONSE rv = (PWIN_UPDATE_RESPONSE)res.constData();
    if(rv->status != 0) {
        qWarnxx(qtr("添加信号源失败"), rv->status);
        return;
    }
    create_connect_for_signal_source(val);
    qDebugxxx(qtr("添加信号源成功"), val->get_name(), val->get_ip_address());
}

void room_worker::get_signal_source()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
}



void room_worker::add_cmd_header(QByteArray &data, int cmd, int len)
{
    CMD_HEADER cmd_header;
    cmd_header.len = len;
    cmd_header.id = m_packet_id ++;
    cmd_header.cmd = cmd;
    add_data_to_byte_array(data, (char *)&cmd_header, sizeof(CMD_HEADER));
}

void room_worker::add_cmd_header2(QByteArray &data, int cmd, int len)
{
    CMD_HEADER cmd_header;
    cmd_header.len = len;
    cmd_header.id = 0;
    cmd_header.cmd = cmd;
    add_data_to_byte_array(data, (char *)&cmd_header, sizeof(CMD_HEADER));
}

void room_worker::logout()
{
    if(m_tcp_socket == nullptr || m_tcp_socket->state() != QTcpSocket::ConnectedState) {
        return;
    }
    QByteArray data;
    add_cmd_header(data, 0x01, 4 + 4);
    m_tcp_socket->write(data);
    if(m_tcp_socket->waitForReadyRead(100)) {
        m_tcp_socket->readAll();
    }
}

QByteArray room_worker::get_response()
{
    if(!m_tcp_socket->waitForReadyRead()) {
        qWarnx(qtr("未接收到响应"));
        return QByteArray();
    }
    int len = 0;
    if(m_tcp_socket->read((char *)&len, sizeof(int)) != sizeof(int)) {
        qWarnx(qtr("未能收取有效的响应长度"));
        m_tcp_socket->readAll();
        return QByteArray();
    }
//    qDebugxx(qtr("接收响应数据的长度"), len);
    if(len <= 0) {
        return QByteArray();
    }
    if(len > 2 * 1024 * 1024) {
        qWarnx(qtr("收到的数据长度过长"));
        m_tcp_socket->readAll();
        return QByteArray();
    }
    return safeRecvbyTcp(m_tcp_socket, len);
}

QByteArray room_worker::get_response2()
{
    if(!m_tcp_socket->waitForReadyRead(1000)) {
        qWarnx(qtr("未接收到响应"));
        return QByteArray();
    }
    int len = 0;
    if(m_tcp_socket->read((char *)&len, sizeof(int)) != sizeof(int)) {
        qWarnx(qtr("未能收取有效的响应长度"));
        m_tcp_socket->readAll();
        return QByteArray();
    }
//    qDebugxx(qtr("接收响应数据的长度"), len);
    if(len <= 0) {
        return QByteArray();
    }
    if(len > 2 * 1024 * 1024) {
        qWarnx(qtr("收到的数据长度过长"));
        m_tcp_socket->readAll();
        return QByteArray();
    }
    return safeRecvbyTcp(m_tcp_socket, len);
}

void room_worker::login()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
    QByteArray data;
    add_cmd_header(data, 0x00, 4 + 4 + 4 + m_data_info->get_user_name().length() + 4 + m_data_info->get_user_pwd().length());
    int len = m_data_info->get_user_name().length();
    add_data_to_byte_array(data, (char *)&len, sizeof(int));
    data.append(m_data_info->get_user_name().toLocal8Bit().constData());
    len = m_data_info->get_user_pwd().length();
    add_data_to_byte_array(data, (char *)&len, sizeof(int));
    data.append(m_data_info->get_user_pwd().toLocal8Bit().constData());
//    QByteArray data2;
//    data2.append(QString("admin").toLocal8Bit().constData());
//    qDebug()<<QString(data2.toHex());
//    qDebug()<<QString(data2);
//    QString cmd="6C 78 31";
//    QByteArray data3=QByteArray::fromHex(cmd.toLatin1());
//    qDebug()<<QString(data3);
    qDebugxx(qtr("登录指令"), QString(data.toHex()));
    m_tcp_socket->write(data);
}

void room_worker::get_sys_info()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
    QByteArray data;
    add_cmd_header(data, 0x02, 4 + 4);
    m_tcp_socket->write(data);
}


void room_worker::get_node_info()
{
    GET_NODE node;
    node.head.cmd = 0x07;
    node.head.id = m_packet_id ++;
    node.node_type = 0x20;
    node.head.len = sizeof(GET_NODE) - sizeof(int);
    m_tcp_socket->write((const char *)&node, sizeof(GET_NODE));
}

void room_worker::get_resource_info(int type)
{
    if(m_tcp_socket == nullptr) {
        return;
    }
    QByteArray data;
    add_cmd_header(data, 0x10, 4 + 4 + 4);
    add_data_to_byte_array(data, (char *)&type, sizeof(int));
    qDebugxx(qtr("获取资源指令"), QString(data.toHex()));
    m_tcp_socket->write(data);
}

void room_worker::get_resource_group_info(const QString &id, data_info *info, const bool root)
{
    if(info == nullptr || id.length() != 32) {
        return;
    }
//    qDebug()<<info->get_name();
    get_resource_group_info(id);
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("获取分组信息失败"));
        reconnected();
        return;
    }
//    qDebugxx(qtr("分组返回"), QString(res.toHex()));
    PRESOURCE_GROUP_FIRST f = (PRESOURCE_GROUP_FIRST)res.constData();
    if(f->status != 0) {
        qWarnxxx(qtr("获取分组信息异常"), f->status, id);
        return;
    }
    QString name = QString();
    if(f->len_name != 0) {
        name = QString(res.mid(sizeof(RESOURCE_GROUP_FIRST), f->len_name));
    }
    PRESOURCE_GROUP_SECOND s = (PRESOURCE_GROUP_SECOND)(res.constData() + sizeof(RESOURCE_GROUP_FIRST) + f->len_name);
    if(!root && !name.isEmpty()) {
        info->set_name(name);
//        qDebug()<<name;
        info->set_device_type(RES_GROUP);
//        qDebugxxx(qtr("找到分组"), info->get_name(), info->get_device_type());
    }
//    qDebugxxxxx(qtr("分组名称"), f->len_name, name, s->count, s->type);
    for(int i = 0; i < s->count; i++) {
        PRESOURCE_GROUP_LEAF l = (PRESOURCE_GROUP_LEAF)(res.constData() + sizeof(RESOURCE_GROUP_FIRST) + f->len_name + sizeof(RESOURCE_GROUP_SECOND) + sizeof(RESOURCE_GROUP_LEAF) * i);
        QString id = QString(QByteArray(l->groud_id, sizeof(l->groud_id)).toHex());
        QSharedPointer<data_info> data = get_signal_source_object(id, info);
        if(data.isNull()) {
            qWarnxx(qtr("不能创建信号源对象"), id);
            continue;
        }
        if(l->flag) {
//            qDebugxxx(qtr("找到子分组"), info->get_name(), id);
            data->set_index(m_signal_source_index++);
            data->set_device_type(RES_GROUP);
            get_resource_group_info(id, data.data());
        }
//        else{
//            qDebugxxx(qtr("找到分组信号源"), info->get_name(), id);
//        }
    }
}

void room_worker::get_resource_group_info(const QString &id)
{
    if(m_tcp_socket == nullptr) {
        return;
    }
    int type = RES_GROUP;
    QByteArray data;
    add_cmd_header(data, 0x11, 4 + 4 + 4 + 16);
    add_data_to_byte_array(data, (char *)&type, sizeof(int));
    QByteArray ba_id = QByteArray::fromHex(id.toLatin1());
    data.append(ba_id);
//    qDebugxx(qtr("获取分组资源指令"), QString(data.toHex()));
    m_tcp_socket->write(data);
}

void room_worker::get_window_info()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
    QByteArray data;
    add_cmd_header(data, 0x20, 4 + 4 + 4);
    int id = 0;
    add_data_to_byte_array(data, (char *)&id, sizeof(int));
    m_tcp_socket->write(data);
}

void room_worker::reconnected()
{
    isFirstConnect = false;
    m_tcp_socket->blockSignals(true);
    if(m_tcp_socket != nullptr) {
        m_tcp_socket->close();
        m_tcp_socket->deleteLater();
        m_tcp_socket = nullptr;
    }
    if(m_timer != nullptr) {
        m_timer->start(3000);
    }
    //    start();
}

void room_worker::connected()
{
    qDebugxx(qtr("连接场景成功"), m_data_info->get_name());
    login();
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("登录失败，重新登录"));
        reconnected();
        return;
    }
    PLOGIN_RESPONSE rv = (PLOGIN_RESPONSE)res.constData();
    if(rv->status != 0) {
        qWarnxx(qtr("登录失败"), rv->status);
        return;
    }
    //测试 获取轮询列表
    get_polling();
    get_sys_info();
    res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("登录失败，重新登录"));
        reconnected();
        return;
    }
    PSYS_INFO sys = (PSYS_INFO)res.constData();
    if(!m_data_info->getIs_led()){
        m_data_info->set_phy_wall_row(sys->row);
        m_data_info->set_phy_wall_column(sys->column);
        m_data_info->set_wall_width(sys->width * sys->column);  //实际大屏的宽和高   //
        m_data_info->set_wall_height(sys->height * sys->row);

    }


    else{
        m_data_info->set_phy_wall_row(1);
        m_data_info->set_phy_wall_column(1);

    }
    emit get_resolution(QString::number(m_data_info->get_wall_width())+"*"+QString::number(m_data_info->get_wall_height()));

    //获取场景模式
    get_mode();
//    qDebug()<<(sys->width * sys->column);
//    qDebugxxxxx(qtr("电视墙信息"), sys->row, sys->column, sys->width, sys->height);
    get_node_info();
    res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("未能获取节点信息，重新登录"));
        reconnected();
        return;
    }
//    qDebug()<<8555;
    PNODE_HEADER node = (PNODE_HEADER)res.constData();
    if(node->count <= 0) {
        qWarnx(qtr("未配置节点信息"));
    }
//    qDebugxx(qtr("获取到配置节点信息"), node->count);
    QList<QString> list_ip;
    int pos_node = sizeof (NODE_HEADER);
    for (int i = 0; i < node->count; i++) {
        QString ip = QString(res.constData() + pos_node);
        qDebugxx(qtr("查询到回显ip地址"), ip);
        list_ip.append(ip);
        pos_node += 20;
    }
    get_resource_group_info("02000000000000000000000000000000", m_data_info, true);
//    m_data_info->print_signal_source();
    get_resource_info(RES_SIGNAL);
    res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("登录失败，重新登录"));
        reconnected();
        return;
    }
    PSIGNAL_HEADER sig = (PSIGNAL_HEADER)res.constData();
    if(sig->count <= 0) {
        qWarnx(qtr("未配置信号源"));
    }
    qDebugxx(qtr("获取到信号源"), sig->count);
    int pos = sizeof (SIGNAL_HEADER);
    for (int i = 0; i < sig->count; i++) {
        PSIGNAL_RESOURCE_FIRST first = (PSIGNAL_RESOURCE_FIRST)(res.constData() + pos);
        QString id = QString(QByteArray(first->id, sizeof(first->id)).toHex());
        QSharedPointer<data_info> info = get_signal_source_object(id);
        if(info.isNull()) {
            qWarnx(qtr("未正确创建对象"));
            break;
        }
        if(first->len) {
            char *name = (char *)(res.constData() + pos + sizeof (SIGNAL_RESOURCE_FIRST));
            info->set_name(QString(QByteArray(name, first->len)));
        }
        pos += 4 + 16 + 4 + first->len;
        PSIGNAL_RESOURCE_SECOND sec = (PSIGNAL_RESOURCE_SECOND)(res.constData() + pos);
        info->set_signal_source_cmd((const unsigned char *)&sec->cmd[0]);
        bool parse_ip = false;
//        qDebug()<<sec->type;
#ifdef IPC_NEW_VERSION
        if(sec->type == 0x05) {
            int len = sec->cmd[0];
            if(len > 0) {
                info->set_user_name(QString(QByteArray(sec->cmd + 1, len)));
            }
            int len_p = sec->cmd[len + 1];
            if(len_p > 0) {
                info->set_user_pwd(QString(QByteArray(sec->cmd + 1 + len + 1, len_p)));
            }
            info->set_channel_no((int)sec->cmd[len + len_p + 2 + 6]);
            int len_ip = sec->cmd[len + len_p + 2 + 10];
            if(len_ip > 0) {
                info->set_ip_address(QString(QByteArray(sec->cmd + len + len_p + 2 + 10 + 1, len_ip)));
            }
        } else {
            parse_ip = true;
        }
#else
        parse_ip = true;
#endif
        info->set_device_type(sec->type);
        info->set_index(m_signal_source_index++);
        if(parse_ip) {
            QStringList sl = QString(sec->cmd).split(":");
            if(sl.length() == 2) {
                info->set_ip_address(sl.value(0));
                info->set_channel_no(sl.value(1).toInt() - 1);
            } else {
                info->set_ip_address(sl.value(0));
            }
        }
        info->set_echo_ip(list_ip);
        if(sec->type == 5) { //ipc
#ifdef IPC_NEW_VERSION
            info->set_port(6905);
#else
            info->set_port(6904);
#endif
        } else if(sec->type == 1) { //hdmi
            info->set_port(6902);
        } else if(sec->type == 4) { //net 抓屏信号
            info->set_port(6902);
        }
        qDebugxxxxxxx(qtr("获取到信号源"), sec->type, id, info->get_ip_address(), info->get_channel_no(), info->get_name(), list_ip.join(','));
        pos += sizeof(SIGNAL_RESOURCE_SECOND);
    }
//    m_data_info->sort_signal_source();
//    m_data_info->print_signal_source();
    m_data_info->send_signal_source_changed_event();
    get_window_info();
    res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("登录失败，重新登录"));
        reconnected();
        return;
    }
    qDebugxx(qtr("查询窗口返回"), QString(res.toHex()));
    PSIGNAL_HEADER win_header = (PSIGNAL_HEADER)res.constData();
    if(win_header->status == -1) {
//        reconnected();
        qWarnx(qtr("用户未登录"));
        return;
    }
    pos = sizeof (SIGNAL_HEADER);
    m_data_info->clear_signal_source_in_tv_wall();
    for (int i = 0; i < win_header->count; i++) { //获取大屏中的信号位置
        PWALL_WIN_INFO win = (PWALL_WIN_INFO)(res.constData() + pos);
        pos += sizeof (WALL_WIN_INFO);
        QString id = QString(QByteArray(win->id, sizeof(win->id)).toHex());
        QSharedPointer<data_info> old = get_signal_source(id);
        if(old.isNull()) {
            qWarnxx(qtr("未正确创建对象"), QString(QByteArray(win->id, sizeof(win->id)).toHex()));
            continue;
        }
        QSharedPointer<data_info> info = QSharedPointer<data_info>(new signal_resource_info);
        info->set_old_data_info(old);
        info->clone(old);
//        info->set_display_x_old(win->x);  //old代表监控屏的属性  ,这里要替换成软件中的大小，不然出错
//        info->set_display_y_old(win->y);
//        info->set_display_width_old(win->width);
//        info->set_display_height_old(win->height);
        info->set_display_x_old(win->x * 1.0 / m_data_info->get_wall_width());  //old代表监控屏的属性  ,这里要替换成比例，不然出错
        info->set_display_y_old(win->y * 1.0 / m_data_info->get_wall_height());
        info->set_display_width_old(win->width * 1.0 / m_data_info->get_wall_width());
        info->set_display_height_old(win->height * 1.0 / m_data_info->get_wall_height());
        if(win->x < 0 || win->x > m_data_info->get_wall_width()) {
            win->x = 0;
        }
        if(win->y < 0 || win->y > m_data_info->get_wall_height()) {
            win->y = 0;
        }
        info->set_display_x(win->x * 1.0 / m_data_info->get_wall_width());
        info->set_display_y(win->y * 1.0 / m_data_info->get_wall_height());
        if(win->width == 0) {
            info->set_display_width(0.5);
        } else {
            info->set_display_width(win->width * 1.0 / m_data_info->get_wall_width());
        }
        if(win->height == 0) {
            info->set_display_height(0.5);
        } else {
            info->set_display_height(win->height * 1.0 / m_data_info->get_wall_height());
        }
//        qDebug() << win->y * 1.0 / m_data_info->get_wall_height();
        info->set_win_id(win->win_id);
        qDebugxxxxxxxx(qtr("获取到窗口信号源"), win->win_id, QString("%1:%2:%3:%4:%5").arg(id).arg(info->get_display_x_old()).arg(info->get_display_y_old()).arg(info->get_display_width_old()).arg(info->get_display_height_old()), info->get_win_id(), info->get_display_x(), info->get_display_y(), info->get_display_width(), info->get_display_height());
        m_data_info->add_signal_source_in_tv_wall(info);
    }
    m_data_info->send_signal_source_in_wall_changed_event();
    m_tcp_socket->readAll();
    m_accept_msg_hint = 1;
}

QSharedPointer<data_info> room_worker::get_signal_source_object(const QString &id, data_info *data)
{
    if(data == nullptr) {
        qWarnx(qtr("参数错误"));
        return QSharedPointer<data_info>();
    }
    QSharedPointer<data_info> info = data->get_signal_source_by_id(id);
    if(!info.isNull()) {
        return info;
    }
//    qDebugxxxx(qtr("创建信号源"), id, data->get_id(), data->get_name());
    info = QSharedPointer<data_info>(new signal_resource_info);
    info->set_id(id);
    create_connect_for_signal_source(info);
//    connect(info.data(), &data_info::add_signal_source_to_wall_signal, this, &room_worker::add_signal_source_to_wall_slot);
//    connect(info.data(), &data_info::update_signal_source_to_wall_signal, this, &room_worker::update_signal_source_to_wall_slot);
//    connect(info.data(), &data_info::remove_signal_source_to_wall_signal, this, &room_worker::remove_signal_source_to_wall_slot);
//    connect(info.data(), &data_info::remove_signal_source_signal, this, &room_worker::remove_signal_source_slot);
    data->add_signal_source(info);   //增加信号来源
    return info;
}


QSharedPointer<data_info> room_worker::get_signal_source_object(const QString &id)
{
    return get_signal_source_object(id, m_data_info);
}

void room_worker::create_connect_for_signal_source(QSharedPointer<data_info> &info)
{
    if(info->get_device_type() == 0x07) {
        return;
    }
    //作用为 使最终显示的大屏的同步该程序 ，该程序只是管理端，真的显示监控的是另一台显示屏
    connect(info.data(), &data_info::add_signal_source_to_wall_signal, this, &room_worker::add_signal_source_to_wall_slot); //拖拽信号到C区时
    connect(info.data(), &data_info::update_signal_source_to_wall_signal, this, &room_worker::update_signal_source_to_wall_slot); //C区信号位置大小发生变化时
    connect(info.data(), &data_info::remove_signal_source_to_wall_signal, this, &room_worker::remove_signal_source_to_wall_slot);   //删除C区信号时
    connect(info.data(), &data_info::remove_signal_source_signal, this, &room_worker::remove_signal_source_slot);
    connect(info.data(), &data_info::win_level_changed, this, &room_worker::win_level_chanaged_slot);   //选中某个信号时
    //二期
    connect(info.data(), &data_info::change_signal_source_to_wall_signal, this, &room_worker::change_signal_source_to_wall_slot);   //更换信号源时
}

QSharedPointer<data_info> room_worker::get_signal_source(const QString &id)
{
    if(m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return QSharedPointer<data_info>();
    }
    QSharedPointer<data_info> info = m_data_info->get_signal_source_by_id(id);
    if(!info.isNull()) {
        return info;
    }
    return QSharedPointer<data_info>();
}


void room_worker::recv_data()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
//    if(m_accept_msg_hint==1){
//        QByteArray res=m_tcp_socket->readAll();
//        qDebug()<<res.toHex();
//        }
//    qDebugxx(qtr("接收到未知数据"), QString(m_tcp_socket->readAll().toHex()));
    //    qDebug()<<QString(m_tcp_socket->readAll().toHex());
}

void room_worker::disconnected_status()
{
    qDebug() << "work disconnect";
    reconnected();
}

void room_worker::add_signal_source_to_wall_slot(QSharedPointer<data_info> val)   //val为C区的信号源
{
    if(val.isNull() || m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    WIN_OPEN open;
    memset(&open, 0, sizeof(WIN_OPEN));
    //吸附小宫格
    if(val->get_display_column() >= 0 && val->get_display_row() >= 0
       && val->get_display_column_num() > 0 && val->get_display_row_num() > 0) {
        open.x = (val->get_display_column() * m_data_info->get_wall_width()) / (m_data_info->get_phy_wall_column() * m_data_info->get_logical_wall_column());
//        qDebug()<<open.x;
        open.y = (val->get_display_row() * m_data_info->get_wall_height()) / (m_data_info->get_phy_wall_row() * m_data_info->get_logical_wall_row());
        open.width = (val->get_display_column_num() * m_data_info->get_wall_width()) / (m_data_info->get_phy_wall_column() * m_data_info->get_logical_wall_column());
        open.height = (val->get_display_row_num() * m_data_info->get_wall_height()) / (m_data_info->get_phy_wall_row() * m_data_info->get_logical_wall_row());
        val->set_display_column(-1);
        val->set_display_column_num(-1);
        val->set_display_row(-1);
        val->set_display_row_num(-1);
    } else {
        open.x = (val->get_display_x_old() * m_data_info->get_wall_width() / val->get_wall_width());
        open.y = (val->get_display_y_old() * m_data_info->get_wall_height() / val->get_wall_height());
        open.width = ceil(val->get_display_width_old() * m_data_info->get_wall_width() / val->get_wall_width());
        open.height = ceil(val->get_display_height_old() * m_data_info->get_wall_height() / val->get_wall_height());
    }
//    open.x = val->get_display_x() * m_data_info->get_wall_width();
//    open.y = val->get_display_y() * m_data_info->get_wall_height();
//    open.width = val->get_display_width() * m_data_info->get_wall_width();
//    open.height = val->get_display_height() * m_data_info->get_wall_height();
    open.head.id = m_packet_id ++;
    open.head.cmd = 0x21;
    open.head.len = sizeof (WIN_OPEN) - 4;
    memcpy(open.id, QByteArray::fromHex(val->get_id().toLatin1()).constData(), sizeof (open.id));
//    char *tmp = (char *)&open;
//    QByteArray qba;
//    for (int i=0;i<sizeof (WIN_OPEN);i++) {
//        qba.append(tmp[i]);
//    }
//    qDebugxxxxxxxx(qtr("添加窗口"), open.x, open.y, open.width, open.height, m_data_info->get_wall_width(), m_data_info->get_wall_height(), QString(qba.toHex()));
    m_data_info->set_isMe(true);
    m_tcp_socket->write((const char *)&open, sizeof(WIN_OPEN));
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("开窗口收取数据失败，重新登录"));
        reconnected();
        return;
    }
    PWIN_OPEN_RESPONSE win = (PWIN_OPEN_RESPONSE)res.constData();
    val->set_win_id(win->win_id); //在这里分配win_id
//    qDebug()<<win->win_id;
    qDebugxxxxx(qtr("添加窗口，获取窗口id"), val->get_id(), win->win_id, win->status, QString(res.toHex()));
    m_tcp_socket->readAll();
}

void room_worker::update_signal_source_to_wall_slot(QSharedPointer<data_info> val)
{
    if(val.isNull() || m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    WIN_UPDATE update;
    memset(&update, 0, sizeof(WIN_UPDATE));
    update.head.id = m_packet_id ++;
    update.head.cmd = 0x24;
    update.head.len = sizeof (WIN_UPDATE) - 4;
    update.win_id = val->get_win_id();
    //吸附小宫格
    if(val->get_display_column() >= 0 && val->get_display_row() >= 0
       && val->get_display_column_num() > 0 && val->get_display_row_num() > 0) {
        update.x = (val->get_display_column() * m_data_info->get_wall_width()) / (m_data_info->get_phy_wall_column() * m_data_info->get_logical_wall_column());
        update.y = (val->get_display_row() * m_data_info->get_wall_height()) / (m_data_info->get_phy_wall_row() * m_data_info->get_logical_wall_row());
        update.width = (val->get_display_column_num() * m_data_info->get_wall_width()) / (m_data_info->get_phy_wall_column() * m_data_info->get_logical_wall_column());
        update.height = (val->get_display_row_num() * m_data_info->get_wall_height()) / (m_data_info->get_phy_wall_row() * m_data_info->get_logical_wall_row());
        val->set_display_column(-1);
        val->set_display_column_num(-1);
        val->set_display_row(-1);
        val->set_display_row_num(-1);
    } else {
        update.x = (val->get_display_x_old() * m_data_info->get_wall_width() / val->get_wall_width());
        update.y = (val->get_display_y_old() * m_data_info->get_wall_height() / val->get_wall_height());
        update.width = ceil(val->get_display_width_old() * m_data_info->get_wall_width() / val->get_wall_width());
        update.height = ceil(val->get_display_height_old() * m_data_info->get_wall_height() / val->get_wall_height());
    }
    qDebugxxxxxxxx(qtr("更新窗口"), update.x, update.y, update.width, update.height, m_data_info->get_wall_width(), m_data_info->get_wall_height(), QString(QByteArray((const char *)&update).toHex()));
//    qDebug()<<"delete";
    QString qStr((const char *)&update);
    m_data_info->set_isMe(true);
    m_tcp_socket->write((const char *)&update, sizeof(WIN_UPDATE));
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("开窗口收取数据失败，重新登录"));
        reconnected();
        return;
    }
    PWIN_UPDATE_RESPONSE win = (PWIN_UPDATE_RESPONSE)res.constData();
    if(win->status != 0) {
        qWarnxxx(qtr("更新窗口位置失败"), win->status, val->get_win_id());
        return;
    }
    m_tcp_socket->readAll();
}

void room_worker::remove_signal_source_to_wall_slot(QSharedPointer<data_info> val)
{
    qDebugx(qtr("删除窗口"));
//    if(m_data_info->getPolling_hint()>-1){
//        stop_polling_slot(m_data_info->getPolling_hint());
//    }
    if(val.isNull() || m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    WIN_CLOSE win;
    memset(&win, 0, sizeof(WIN_CLOSE));
    win.head.id = m_packet_id ++;
    win.head.cmd = 0x22;
    win.head.len = sizeof(WIN_CLOSE) - 4;
    win.win_id = val->get_win_id();
    m_data_info->set_isMe(true);
    m_tcp_socket->write((const char *)&win, sizeof(WIN_CLOSE));
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("删除窗口收取数据失败，重新登录"));
        reconnected();
        return;
    }
    PWIN_UPDATE_RESPONSE rv = (PWIN_UPDATE_RESPONSE)res.constData();
    if(rv->status != 0) {
        qWarnxx(qtr("删除窗口失败"), rv->status);
        m_tcp_socket->readAll();
        return;
    }
    m_tcp_socket->readAll();
}

void room_worker::remove_signal_source_slot(QSharedPointer<data_info> val)
{
//    if(m_data_info->getPolling_hint()>-1){
//        stop_polling_slot(m_data_info->getPolling_hint());
//    }
    if(val.isNull() || m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    RESOURCE_DELETE del;
    memset(&del, 0, sizeof(RESOURCE_DELETE));
    del.head.id = m_packet_id ++;
    del.head.cmd = 0x13;
    del.head.len = sizeof (RESOURCE_DELETE) - 4;
    del.type = val->get_device_type();
    QByteArray ba_group = QByteArray::fromHex(m_group_id.toLatin1());
    memcpy(del.groud_id, ba_group.constData(), sizeof (del.groud_id));
    memcpy(del.source_id, QByteArray::fromHex(val->get_id().toLatin1()).constData(), sizeof (del.source_id));
    m_data_info->set_isMe(true);
    m_tcp_socket->write((const char *)&del, sizeof(RESOURCE_DELETE));
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("删除信号源收取数据失败，重新登录"));
        reconnected();
        return;
    }
    PWIN_UPDATE_RESPONSE rv = (PWIN_UPDATE_RESPONSE)res.constData();
    if(rv->status != 0) {
        qWarnxx(qtr("删除信号源失败"), rv->status);
        return;
    }
    m_tcp_socket->readAll();
}

void room_worker::win_level_chanaged_slot(QSharedPointer<data_info> val) //设置窗口层次
{
    if(val.isNull() || m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    WIN_LEVEL_CHANGED cmd;
    memset(&cmd, 0, sizeof(WIN_LEVEL_CHANGED));
    cmd.head.id = m_packet_id ++;
    cmd.head.cmd = 0x26;
    cmd.head.len = sizeof (WIN_LEVEL_CHANGED) - 4;
    cmd.win_id = val->get_win_id();
    cmd.level = 1;
    m_data_info->set_isMe(true);
    m_tcp_socket->write((const char *)&cmd, sizeof(WIN_LEVEL_CHANGED));
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("更新信号源显示层次收取数据失败，重新登录"));
        reconnected();
        return;
    }
    PWIN_UPDATE_RESPONSE rv = (PWIN_UPDATE_RESPONSE)res.constData();
    if(rv->status != 0) {
        qWarnxx(qtr("更新信号源显示层次失败"), rv->status);
        return;
    }
//    qDebugxx(qtr("设置显示窗口置顶"), val->get_win_id());
    m_tcp_socket->readAll();
}

void room_worker::change_signal_source_to_wall_slot(QSharedPointer<data_info> val)
{
    if(val.isNull() || m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    SIG_CHANGE open;
    memset(&open, 0, sizeof(SIG_CHANGE));
    open.head.id = m_packet_id ++;
    open.head.cmd = 0x25;
    open.head.len = sizeof (SIG_CHANGE) - 4;
    open.wall_id = val->get_win_id();
    memcpy(open.id, QByteArray::fromHex(val->get_id().toLatin1()).constData(), sizeof (open.id));
    m_data_info->set_isMe(true);
    m_tcp_socket->write((const char *)&open, sizeof(SIG_CHANGE));
//    qDebug()<<QByteArray((const char *)&open, sizeof(SIG_CHANGE)).toHex();
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("开窗口收取数据失败，重新登录"));
        reconnected();
        return;
    }
    PSIG_CHANGE_RESPONSE win = (PSIG_CHANGE_RESPONSE)res.constData();
    if(win->status != 0) {
        qWarnxx(qtr("更换信号源失败"), win->status);
        return;
    }
    qDebugx(qtr("更换信号源成功"));
    m_tcp_socket->readAll();
}
void room_worker:: fresh_win_slot()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
    QByteArray data;
    add_cmd_header(data, 0x20, 4 + 4 + 4);
    int id = 0;
    add_data_to_byte_array(data, (char *)&id, sizeof(int));
//    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
    QByteArray res = get_response();
//    qDebug()<<res.toHex();
    if(res.count() <= 0) {
        qWarnx(qtr("登录失败，重新登录"));
        reconnected();
        return;
    }
    qDebugxx(qtr("查询窗口返回"), QString(res.toHex()));
    PSIGNAL_HEADER win_header = (PSIGNAL_HEADER)res.constData();
    if(win_header->status == -1) {
//        reconnected();
        qWarnx(qtr("用户未登录"));
        return;
    }
    if(res.toHex().length()<24)
       {
           m_tcp_socket->readAll();
           return;

       }
    int pos = sizeof (SIGNAL_HEADER);
    m_data_info->clear_signal_source_in_tv_wall();
    for (int i = 0; i < win_header->count; i++) { //获取大屏中的信号位置
        PWALL_WIN_INFO win = (PWALL_WIN_INFO)(res.constData() + pos);
        pos += sizeof (WALL_WIN_INFO);
        QString id = QString(QByteArray(win->id, sizeof(win->id)).toHex());
        QSharedPointer<data_info> old = get_signal_source(id);
        if(old.isNull()) {
            qWarnxx(qtr("未正确创建对象"), QString(QByteArray(win->id, sizeof(win->id)).toHex()));
            continue;
        }
        QSharedPointer<data_info> info = QSharedPointer<data_info>(new signal_resource_info);
        info->set_old_data_info(old);
        info->clone(old);
//        info->set_display_x_old(win->x);  //old代表监控屏的属性  ,这里要替换成软件中的大小，不然出错
//        info->set_display_y_old(win->y);
//        info->set_display_width_old(win->width);
//        info->set_display_height_old(win->height);
        info->set_display_x_old(win->x * 1.0 / m_data_info->get_wall_width());  //old代表监控屏的属性  ,这里要替换成比例，不然出错
        info->set_display_y_old(win->y * 1.0 / m_data_info->get_wall_height());
        info->set_display_width_old(win->width * 1.0 / m_data_info->get_wall_width());
        info->set_display_height_old(win->height * 1.0 / m_data_info->get_wall_height());
        if(win->x < 0 || win->x > m_data_info->get_wall_width()) {
            win->x = 0;
        }
        if(win->y < 0 || win->y > m_data_info->get_wall_height()) {
            win->y = 0;
        }
        info->set_display_x(win->x * 1.0 / m_data_info->get_wall_width());
        info->set_display_y(win->y * 1.0 / m_data_info->get_wall_height());
        if(win->width == 0) {
            info->set_display_width(0.5);
        } else {
            info->set_display_width(win->width * 1.0 / m_data_info->get_wall_width());
        }
        if(win->height == 0) {
            info->set_display_height(0.5);
        } else {
            info->set_display_height(win->height * 1.0 / m_data_info->get_wall_height());
        }
//        qDebug()<<win->height * 1.0 / m_data_info->get_wall_height();
//        qDebug()<<win->y * 1.0 / m_data_info->get_wall_height();
        info->set_win_id(win->win_id);
        qDebugxxxxxxxx(qtr("获取到窗口信号源"), win->win_id, QString("%1:%2:%3:%4:%5").arg(id).arg(info->get_display_x_old()).arg(info->get_display_y_old()).arg(info->get_display_width_old()).arg(info->get_display_height_old()), info->get_win_id(), info->get_display_x(), info->get_display_y(), info->get_display_width(), info->get_display_height());
        m_data_info->add_signal_source_in_tv_wall(info);
    }
    m_data_info->send_signal_source_in_wall_changed_event();
    m_tcp_socket->readAll();

}
void room_worker::change_caption_slot(int hint)
{
    if(m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    char temp = 0x00;
    char temp2 = 0x00;
    char temp3 = 0x00;
    char cmd = 0x00;
    if(hint == 1) {
        cmd = 0x01;
    }
    QByteArray data;
    add_cmd_header(data, 0x53, 4 + 4 + 1 + 1 + 2);
    add_data_to_byte_array(data, (char *)&temp, sizeof ( char));
    add_data_to_byte_array(data, (char *)&cmd, sizeof ( char));
    add_data_to_byte_array(data, (char *)&temp2, sizeof ( char));
    add_data_to_byte_array(data, (char *)&temp3, sizeof ( char));
    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
//    QByteArray res = get_response();
//    if(res.count() <= 0){
//        qWarnx(qtr("操作字幕失败，重新登录"));
//        reconnected();
//        return;
//    }
//    PCAPTION_RESPONSE cap = (PCAPTION_RESPONSE)res.constData();
//    if(cap->status != 0){
//        qWarnxx(qtr("操作字幕失败"), cap->status);
//        return;
//    }
    m_tcp_socket->readAll();
}

void room_worker::mode_chanaged_slot(QString index)
{
    if(m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    QByteArray data;
    //    char hint = (char)index; //会自己转换为16进制的形式
    int len = index.toUtf8().length();
    add_cmd_header(data, 0x49, 4 + 4 + 4 + len);
    //    add_data_to_byte_array(data, (char *)&hint, sizeof ( char));
    add_data_to_byte_array(data, (char *)&len, sizeof(int));
    data.append(index.toUtf8().constData());
    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
    qDebugxx(qtr("切换场景指令"), QString(data.toHex()));

    //    QByteArray res = get_response2();
    //        if(res.count() <= 0){
    //            qWarnx(qtr("重新登录"));
    //            reconnected();
    //            return;
    //        }
    //    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
    //        if(rv->status != 0){
    //            qWarnxx(qtr("使用名称调用模式失败"), rv->status);
    //            return;
    //        }
    m_tcp_socket->readAll();
    wait_by_event_loop(200);
    get_wall_signal_slot();
    fresh_win_slot();
}

void room_worker::mode_chanaged_slot_int(int index)
{
    if(m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    char hint = (char)index; //会自己转换为16进制的形式
    QByteArray data;
    add_cmd_header(data, 0x48, 4 + 4 + 1);
    add_data_to_byte_array(data, (char *)&hint, sizeof ( char));
    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
    QByteArray res = get_response();
    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
    if(rv->status != 0) {
        qWarnxx(qtr("使用序号调用模式失败"), rv->status);
        return;
    }
    m_tcp_socket->readAll();
    is_display_wall_signal();
    get_wall_signal_slot();
}

void room_worker::mode_add_slot(QSharedPointer<data_info> info)
{
    if(info.isNull() || m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    QList<QSharedPointer<data_info> > list = info->get_list_signal_source_in_tv_wall();
    QString mode_name = info->get_name();
    QByteArray data;
    int len = sizeof (CMD_HEADER) - 4 + 4 + 16 + sizeof (MODE_RESOURCE_FIRST) + mode_name.toUtf8().length() + 4 + list.length() * sizeof (WIN) + 4;
    add_cmd_header(data, 0x12, len);
    QByteArray group_id = QByteArray::fromHex(m_group_id_mode.toLatin1());
    int type = RES_SCENE;
    add_data_to_byte_array(data, (char *)&type, sizeof(int));
    data.append(group_id);
    MODE_RESOURCE_FIRST mode;
    mode.cdSize = 4 + 16 + 4 + mode_name.toUtf8().length() + 4 + list.length() * sizeof (WIN) + 4;
    memcpy(mode.id, QByteArray::fromHex(info->get_id().toLatin1()).constData(), sizeof (mode.id));
    mode.len =mode_name.toUtf8().length();
    data.append((const char *)&mode, sizeof(MODE_RESOURCE_FIRST));

//        qDebug()<<mode_name.toUtf8().length();;

    data.append(mode_name.toUtf8().constData());
    int win_num = list.length();
    add_data_to_byte_array(data, (char *)&win_num, sizeof(int));
    foreach(QSharedPointer<data_info> val, list) { //每一个窗口信号
        if(val.isNull()) {
            continue;
        }
        WIN win;
        memset(&win, 0, sizeof(WIN));
        if(val->get_display_column() >= 0 && val->get_display_row() >= 0
           && val->get_display_column_num() >= 0 && val->get_display_row_num() >= 0) {
            win.left = (val->get_display_column() * m_data_info->get_wall_width()) / (m_data_info->get_phy_wall_column() * m_data_info->get_logical_wall_column());
            win.top = (val->get_display_row() * m_data_info->get_wall_height()) / (m_data_info->get_phy_wall_row() * m_data_info->get_logical_wall_row());
            win.width = (val->get_display_column_num() * m_data_info->get_wall_width()) / (m_data_info->get_phy_wall_column() * m_data_info->get_logical_wall_column());
            win.height = (val->get_display_row_num() * m_data_info->get_wall_height()) / (m_data_info->get_phy_wall_row() * m_data_info->get_logical_wall_row());
            val->set_display_column(-1);
            val->set_display_column_num(-1);
            val->set_display_row(-1);
            val->set_display_row_num(-1);
        } else {
            if(val->get_display_width_old() < 1) { //说明这些信号源是开机时获取的或者是点击场景获取的
                win.left = val->get_display_x_old() * m_data_info->get_wall_width();
                win.top = val->get_display_y_old() * m_data_info->get_wall_height();
                win.width = ceil( val->get_display_width_old() * m_data_info->get_wall_width());
                win.height = ceil( val->get_display_height_old() * m_data_info->get_wall_height() );
            } else {
                win.left = (val->get_display_x_old() * m_data_info->get_wall_width() / val->get_wall_width());
                win.top = (val->get_display_y_old() * m_data_info->get_wall_height() / val->get_wall_height());
                win.width = ceil(val->get_display_width_old() * m_data_info->get_wall_width() / val->get_wall_width());
                win.height = ceil( val->get_display_height_old() * m_data_info->get_wall_height() / val->get_wall_height());
            }
        }
        win.win_id = val->get_win_id();
        memcpy(win.signal_id, QByteArray::fromHex(val->get_id().toLatin1()).constData(), sizeof (win.signal_id));
        data.append((const char *)&win, sizeof(WIN));
    }
    int video_num = 0;
    add_data_to_byte_array(data, (char *)&video_num, sizeof(int));
    m_data_info->set_isMe(true);
//    qDebug()<<data.toHex();
    m_tcp_socket->write(data);
    QByteArray res = get_response();
    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
    if(rv->status != 0) {
        qWarnxx(qtr("模式增加失败"), rv->status);
        return;
    }
    m_tcp_socket->readAll();
}

void room_worker::mode_edit_slot(QSharedPointer<data_info> info)
{
    if(info.isNull() || m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    QList<QSharedPointer<data_info> > list = info->get_list_signal_source_in_tv_wall();
    QString mode_name = info->get_name();
    QByteArray data;
    int len = sizeof (CMD_HEADER) - 4 + 4 + 16 + sizeof (MODE_RESOURCE_FIRST) + mode_name.toUtf8().length() + 4 + list.length() * sizeof (WIN) + 4;
    add_cmd_header(data, 0x15, len);
    QByteArray group_id = QByteArray::fromHex(m_group_id_mode.toLatin1());
    int type = RES_SCENE;
    add_data_to_byte_array(data, (char *)&type, sizeof(int));
    data.append(group_id);
    MODE_RESOURCE_FIRST mode;
    mode.cdSize = 4 + 16 + 4 + mode_name.toUtf8().length() + 4 + list.length() * sizeof (WIN) + 4;
    memcpy(mode.id, QByteArray::fromHex(info->get_id().toLatin1()).constData(), sizeof (mode.id));
    mode.len =mode_name.toUtf8().length();
    data.append((const char *)&mode, sizeof(MODE_RESOURCE_FIRST));
    data.append(mode_name.toUtf8().constData());
    int win_num = list.length();
    add_data_to_byte_array(data, (char *)&win_num, sizeof(int));
    foreach(QSharedPointer<data_info> val, list) { //每一个窗口信号
        if(val.isNull()) {
            continue;
        }
        WIN win;
        memset(&win, 0, sizeof(WIN));
        if(val->get_display_column() >= 0 && val->get_display_row() >= 0
           && val->get_display_column_num() > 0 && val->get_display_row_num() > 0) {
            win.left = (val->get_display_column() * m_data_info->get_wall_width()) / (m_data_info->get_phy_wall_column() * m_data_info->get_logical_wall_column());
            win.top = (val->get_display_row() * m_data_info->get_wall_height()) / (m_data_info->get_phy_wall_row() * m_data_info->get_logical_wall_row());
            win.width = (val->get_display_column_num() * m_data_info->get_wall_width()) / (m_data_info->get_phy_wall_column() * m_data_info->get_logical_wall_column());
            win.height = (val->get_display_row_num() * m_data_info->get_wall_height()) / (m_data_info->get_phy_wall_row() * m_data_info->get_logical_wall_row());
            val->set_display_column(-1);
            val->set_display_column_num(-1);
            val->set_display_row(-1);
            val->set_display_row_num(-1);
        } else {
            if(val->get_display_width_old() < 1) { //说明这些信号源是开机时获取的或者是点击场景获取的
                win.left = val->get_display_x_old() * m_data_info->get_wall_width();
                win.top = val->get_display_y_old() * m_data_info->get_wall_height();
                win.width = ceil(val->get_display_width_old() * m_data_info->get_wall_width());
                win.height = ceil(val->get_display_height_old() * m_data_info->get_wall_height() );
            } else {
                win.left = (val->get_display_x_old() * m_data_info->get_wall_width() / val->get_wall_width());
                win.top = (val->get_display_y_old() * m_data_info->get_wall_height() / val->get_wall_height());
                win.width = ceil(val->get_display_width_old() * m_data_info->get_wall_width() / val->get_wall_width());
                win.height = ceil(val->get_display_height_old() * m_data_info->get_wall_height() / val->get_wall_height());
            }
        }
        win.win_id = val->get_win_id();
        memcpy(win.signal_id, QByteArray::fromHex(val->get_id().toLatin1()).constData(), sizeof (win.signal_id));
        data.append((const char *)&win, sizeof(WIN));
    }
    int video_num = 0;
    add_data_to_byte_array(data, (char *)&video_num, sizeof(int));
    m_data_info->set_isMe(true);
//    qDebug()<<data.toHex();
    m_tcp_socket->write(data);
    QByteArray res = get_response();
    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
    if(rv->status != 0) {
        qWarnxx(qtr("模式修改失败"), rv->status);
        return;
    }
    m_tcp_socket->readAll();
}

void room_worker::mode_remove_slot(QString id)
{
    if(m_tcp_socket == nullptr || m_data_info == nullptr || id == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    QByteArray group_id = QByteArray::fromHex(m_group_id_mode.toLatin1());
    QByteArray res_id = QByteArray::fromHex(id.toLatin1());
    int type = RES_SCENE;
    QByteArray data;
    add_cmd_header(data, 0x13, 4 + 4 + 4 + 16 + 16);
    add_data_to_byte_array(data, (char *)&type, sizeof(int));
    data.append(group_id);
    data.append(res_id);
    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
//    QByteArray res = get_response();
//    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
//    if(rv->status != 0){
//        qWarnxx(qtr("停止轮巡失败"), rv->status);
//        return;
//    }
    m_tcp_socket->readAll();
}

void room_worker::ptz_slot(int o, int a, int win_id)
{
    if(m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    char temp = 0x00;
    char oper = (char)o;
    char act = (char)a;
    QByteArray data;
    add_cmd_header(data, 0x2a, 4 + 4 + 4 + 1 + 3 + 1 + 3);
    add_data_to_byte_array(data, (char *)&win_id, sizeof(int));
    add_data_to_byte_array(data, (char *)&oper, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&act, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
//    QByteArray res = get_response();
//    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
//    if(rv->status != 0){
//        qWarnxx(qtr("停止轮巡失败"), rv->status);
//        return;
//    }
    m_tcp_socket->readAll();
}

void room_worker::open_audio_slot(QString sig_id, int Channel, int vol)
{
    if(m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    QByteArray sig_id_b = QByteArray::fromHex(sig_id.toLatin1());
    char temp = 0x00;
    char oper = (char)vol;
    QByteArray data;
    add_cmd_header(data, 0x2d, 4 + 4 + 16 + 4 + 4);
    data.append(sig_id_b);
    add_data_to_byte_array(data, (char *)&Channel, sizeof(int));
    add_data_to_byte_array(data, (char *)&oper, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
    QByteArray res = get_response();
    POPEN_AUDIO_RESPONSE rv = (POPEN_AUDIO_RESPONSE)res.constData();
    if(rv->status != 0) {
        qWarnxx(qtr("停止轮巡失败"), rv->status);
        return;
    }
    foreach(QSharedPointer<data_info> info, m_data_info->get_list_signal_source_in_tv_wall()) {
        if(info->get_id() == sig_id) {
            info->set_audio_id(rv->audio_id);
            break;
        }
    }
    m_tcp_socket->readAll();
}

void room_worker::close_audio_slot(int audio_id)
{
    if(m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    QByteArray data;
    add_cmd_header(data, 0x2e, 4 + 4 + 4);
    add_data_to_byte_array(data, (char *)&audio_id, sizeof(int));
    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
//    QByteArray res = get_response();
//    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
//    if(rv->status != 0){
//        qWarnxx(qtr("停止轮巡失败"), rv->status);
//        return;
//    }
    m_tcp_socket->readAll();
}

void room_worker::set_audio_slot(int audio_id, int vol)
{
    if(m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return;
    }
    char oper = (char)vol;
    QByteArray data;
    add_cmd_header(data, 0x2f, 4 + 4 + 4 + 4);
    char temp = 0x00;
    add_data_to_byte_array(data, (char *)&audio_id, sizeof(int));
    add_data_to_byte_array(data, (char *)&oper, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
//    QByteArray res = get_response();
//    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
//    if(rv->status != 0){
//        qWarnxx(qtr("停止轮巡失败"), rv->status);
//        return;
//    }
    m_tcp_socket->readAll();
}

void room_worker::get_wall_signal_slot()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
    QByteArray data;
    add_cmd_header(data, 0x20, 4 + 4 + 4);
    int id = 0;
    add_data_to_byte_array(data, (char *)&id, sizeof(int));
//    m_data_info->set_isMe(true);
    m_tcp_socket->write(data);
    QByteArray res = get_response();
    if(res.count() <= 0) {
        qWarnx(qtr("登录失败，重新登录"));
        reconnected();
        return;
    }
    qDebugxx(qtr("查询窗口返回"), QString(res.toHex()));
    PSIGNAL_HEADER win_header = (PSIGNAL_HEADER)res.constData();
    if(win_header->status == -1) {
//        reconnected();
        qWarnx(qtr("用户未登录"));
        return;
    }
    if(res.toHex().length()<24)
       {
           m_tcp_socket->readAll();
           return;

       }
    int pos = sizeof (SIGNAL_HEADER);
//    qDebug()<<win_header->count;
    m_data_info->clear_signal_source_in_tv_wall();
    for (int i = 0; i < win_header->count; i++) { //获取大屏中的信号位置
        PWALL_WIN_INFO win = (PWALL_WIN_INFO)(res.constData() + pos);
        pos += sizeof (WALL_WIN_INFO);
        QString id = QString(QByteArray(win->id, sizeof(win->id)).toHex());
        QSharedPointer<data_info> old = get_signal_source(id);
        if(old.isNull()) {
            qWarnxx(qtr("切换模式时未正确创建对象"), QString(QByteArray(win->id, sizeof(win->id)).toHex()));
            continue;
        }
        QSharedPointer<data_info> info = QSharedPointer<data_info>(new signal_resource_info);
        info->set_old_data_info(old);
        info->clone(old);
//        info->set_display_x_old(win->x);  //old代表监控屏的属性  ,这里要替换成软件中的大小，不然出错
//        info->set_display_y_old(win->y);
//        info->set_display_width_old(win->width);
//        info->set_display_height_old(win->height);
        info->set_display_x_old(win->x * 1.0 / m_data_info->get_wall_width());  //old代表监控屏的属性  ,这里要替换成比例，不然出错
        info->set_display_y_old(win->y * 1.0 / m_data_info->get_wall_height());
        info->set_display_width_old(win->width * 1.0 / m_data_info->get_wall_width());
        info->set_display_height_old(win->height * 1.0 / m_data_info->get_wall_height());
        if(win->x < 0 || win->x > m_data_info->get_wall_width()) {
            win->x = 0;
        }
        if(win->y < 0 || win->y > m_data_info->get_wall_height()) {
            win->y = 0;
        }
        info->set_display_x(win->x * 1.0 / m_data_info->get_wall_width());
        info->set_display_y(win->y * 1.0 / m_data_info->get_wall_height());
        if(win->width == 0) {
            info->set_display_width(0.5);
        } else {
            info->set_display_width(win->width * 1.0 / m_data_info->get_wall_width());
        }
        if(win->height == 0) {
            info->set_display_height(0.5);
        } else {
            info->set_display_height(win->height * 1.0 / m_data_info->get_wall_height());
        }
//        qDebug()<<info->get_display_width();
        info->set_win_id(win->win_id);
        qDebugxxxxxxxx(qtr("获取到窗口信号源"), win->win_id, QString("%1:%2:%3:%4:%5").arg(id).arg(info->get_display_x_old()).arg(info->get_display_y_old()).arg(info->get_display_width_old()).arg(info->get_display_height_old()), info->get_win_id(), info->get_display_x(), info->get_display_y(), info->get_display_width(), info->get_display_height());
        m_data_info->add_signal_source_in_tv_wall(info);
    }
    m_data_info->send_signal_source_in_wall_changed_event();
    m_tcp_socket->readAll();

}

void room_worker::is_display_wall_signal()
{
    int count = 0;
    while(count == 0) {
        QByteArray data;
        add_cmd_header(data, 0x20, 4 + 4 + 4);
        int id = 0;
        add_data_to_byte_array(data, (char *)&id, sizeof(int));
        //    m_data_info->set_isMe(true);
        m_tcp_socket->write(data);
        QByteArray res = get_response();
        if(res.count() <= 0) {
            continue;
        }
        PSIGNAL_HEADER win_header = (PSIGNAL_HEADER)res.constData();
        if(win_header->status == -1) {
            continue;
        }
        int pos = sizeof (SIGNAL_HEADER);
        //    qDebug()<<win_header->count;
        count = win_header->count;
    }
}
