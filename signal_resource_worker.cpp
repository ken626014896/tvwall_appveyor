﻿#include "signal_resource_worker.h"
#include "sy_logger.h"
#include "sy_utils.h"
//#include "gdsdk.h"

#include <QHostAddress>
#include <QPixmap>
#include <QImage>

#ifdef WIN32
    #include <qt_windows.h>
#endif


typedef struct tagSETSIGNALINFO {
    int msg;		//固定值：26
    char type;		//信号源类型：IPC为5
    char reserved[3];	//保留
    char cmd[256];	//信号源信息
} SETSIGNALINFO;

typedef struct tagINITREQ {
    int msg;	//固定值：2
    char type;	//信号源类型：参考hlsdk.h定义
    char ch;	//通道号，从0开始（如果界面上填写形式如192.168.1.20:3,则通道号是2）
    char e;	//固定值：1
    char strtype;	//码流类型：jpeg类型为3
} INITREQ, *PINITREQ;

typedef struct tagREQPACKET {
    unsigned int frameNo;	//请求帧序列号
    int reserved[2];		//保留
} REQPACKET, *PREQPACKET;


typedef struct tagSTREAMHEADER {
    unsigned int frameNo;	//帧序列号
    int streamLen;		//帧长度
    unsigned short width;	//图像宽
    unsigned short height; 	//图像高
    unsigned int frameFlag;	//帧标志

} STREAMHEADER, *PSTREAMHEADER;


signal_resource_worker::signal_resource_worker(data_info *data, QObject *parent) : QObject(parent),
    m_tcp_socket(nullptr),
    m_data_info(data),
    m_last_frame_no(0),
    m_getted_image(false),
    m_timer(nullptr),
    m_sended_signal_info(false),
    m_echo_ip_index(0),
    m_timer_reconnect(nullptr)
{
    if(m_data_info != nullptr) {
        connect(m_data_info, &data_info::status_chanaged,
                this, &signal_resource_worker::status_chanaged_slot);
    }
}

signal_resource_worker::~signal_resource_worker()
{
    stop();
    if(m_timer_reconnect != nullptr) {
        m_timer_reconnect->stop();
        delete m_timer_reconnect;
        m_timer_reconnect = nullptr;
    }
}

void signal_resource_worker::start()
{
    if(m_tcp_socket != nullptr || m_data_info == nullptr) {
        qWarnxxx(qtr("已经连接"), m_tcp_socket == nullptr, m_data_info == nullptr);
        return;
    }

    QString ip = m_data_info->get_ip_address();
#ifdef IPC_NEW_VERSION
    if(m_data_info->get_device_type() == 5) {
//        m_echo_ip_index=0;
        m_sended_signal_info=false;
        QList<QString> list = m_data_info->get_echo_ip();
        if(list.count()) {
            m_echo_ip_index = (m_echo_ip_index + 1) % list.count();
        }
        ip = m_data_info->get_echo_ip_by_index(m_echo_ip_index);
        qDebugxxxx(qtr("ipc信号选择回显:"), m_data_info->get_name(), ip, m_data_info->get_port());

    }
#endif
    if(ip.isEmpty()) {
        qWarnxxxx(qtr("不能获取有效的回显服务器IP地址"), m_data_info->get_name(), m_echo_ip_index, m_data_info->get_echo_ip().join(','));
        return;
    }
    qDebugxxxx(qtr("连接信号源"), m_data_info->get_name(), ip, m_data_info->get_port());
    m_tcp_socket = new QTcpSocket();
    connect(m_tcp_socket, &QTcpSocket::readyRead, this, &signal_resource_worker::recv_data);
    connect(m_tcp_socket, &QTcpSocket::connected, this, &signal_resource_worker::connected);
    connect(m_tcp_socket, &QTcpSocket::disconnected, this, &signal_resource_worker::disconnected_status);

    m_tcp_socket->connectToHost(QHostAddress(ip), m_data_info->get_port());
    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &signal_resource_worker::request_jpg_slot);
    if(m_timer_reconnect == nullptr) {
        m_timer_reconnect = new QTimer();
        connect(m_timer_reconnect, &QTimer::timeout, this, &signal_resource_worker::timer_reconnect_slot);
    } else {
        m_timer_reconnect->stop();
    }
}
void signal_resource_worker::disconnected_status()
{

    m_tcp_socket->blockSignals(true);
    if(m_tcp_socket != nullptr) {
        m_tcp_socket->close();
        m_tcp_socket->deleteLater();
        m_tcp_socket = nullptr;
    }
    if (m_timer != nullptr) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if(m_timer_reconnect != nullptr) {
        m_timer_reconnect->start(3000);
    }
}
void signal_resource_worker::reconnect()
{
    qDebugx(qtr("启动重连"));
    //这行新增20200701
    m_tcp_socket->blockSignals(true);
    stop();
    if(m_timer_reconnect != nullptr) {
        m_timer_reconnect->start(3000);
    }
//    start();
}
void signal_resource_worker::stop()
{
    if(m_timer != nullptr) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if(m_tcp_socket != nullptr) {
        m_tcp_socket->close();
        delete m_tcp_socket;
        m_tcp_socket = nullptr;
    }
}



bool signal_resource_worker::send_signal_info()
{
    if(m_data_info == nullptr) {
        return false;
    }
    SETSIGNALINFO info;
    memset(&info, 0, sizeof(SETSIGNALINFO));
    info.type = 5;
    info.msg = 26;
//    QString cmd = QString("%1:%2").arg(m_data_info->get_ip_address()).arg(m_data_info->get_channel_no() + 1);
//    strcpy_s(info.cmd, m_data_info->get_signal_source_cmd().toLocal8Bit().constData());
    memcpy(info.cmd, m_data_info->get_signal_source_cmd(), sizeof(info.cmd));
    qWarnxx(qtr("发发的数据:")+m_data_info->get_name(),(const char *)&info);

    m_tcp_socket->write((const char *)&info, sizeof(SETSIGNALINFO));
    return true;
}

void signal_resource_worker::send_init_req()
{
    INITREQ req;
    req.e = 1;
    req.ch = m_data_info->get_channel_no() & 0xff;
#ifdef IPC_NEW_VERSION
    if(m_data_info->get_device_type() == 0x05) {
        req.e = (m_data_info->get_channel_no() >> 8) & 0xff;
    } else {
        req.e = 0x00;
    }
#endif
    req.msg = 2;
    req.type = m_data_info->get_device_type();
    req.strtype = m_data_info->get_device_type() == 1 ? 3 : 2;
#ifdef IPC_NEW_VERSION
    req.strtype = 3;
#endif
    m_tcp_socket->write((const char *)&req, sizeof(INITREQ));
    bool send_req = false;
    if(m_data_info->get_device_type() == 1) {
        send_req = true;
    }
#ifdef IPC_NEW_VERSION
    send_req = true;
#endif
    if(send_req) {
        REQPACKET p;
        memset(&p, 0, sizeof(REQPACKET));
        m_tcp_socket->write((const char *)&p, sizeof(REQPACKET));
        m_timer->start(40);
//        qDebugxx(qtr("发送包请求"), m_data_info->get_name());
    }
}

void signal_resource_worker::connected()
{
    qDebugxxxx(qtr("连接成功"), m_data_info->get_name(), m_data_info->get_channel_no(), m_data_info->get_device_type());
    if(m_tcp_socket == nullptr) {
        return;
    }
#ifdef IPC_NEW_VERSION
    if(m_data_info->get_device_type() == 5) {
        send_signal_info();
    } else {
        send_init_req();
    }
#else
    send_init_req();
#endif
}




void signal_resource_worker::recv_timestamp()
{
    if(m_data_info->get_device_type() == 5) {
        unsigned int ts = 0;
        int rv = safeRecvbyTcp(m_tcp_socket, (char *)&ts, sizeof(unsigned int));
        if(rv != sizeof(unsigned int)) {
            qWarnxxx(qtr("接收时间戳错误"), ts, rv);
            reconnect();
            return;
        }
//        qDebugxx(qtr("接收到timestamp"), ts);
    }
}


void signal_resource_worker::recv_data()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
#ifdef IPC_NEW_VERSION
    if(m_data_info->get_device_type() == 5 && !m_sended_signal_info) {
        QByteArray data = safeRecvbyTcp(m_tcp_socket, 4);
        QString ss=data.toHex();
        qWarnxx(qtr("接收到的数据:")+m_data_info->get_name(),ss);

        if(data.length() < 4) {
            qWarnxx(qtr("接收到的数据长度错误"), data.length());
            reconnect();
            return;
        }
        int status = data[3];
        if(status != 0) {
            qWarnxxxxx(qtr("状态错误,重新选择回显IP"),m_data_info->get_name(), status, m_data_info->get_ip_address(), m_data_info->get_channel_no() + 1);
            reconnect();
            return;
        }
        int c = data[0] | data[1] << 8;
        m_data_info->set_channel_no(c);
//        qDebugxxxxx(qtr("找到通道号"), c, (int)data[0], (int)data[1], m_data_info->get_name());
//        if(c - 1 != m_data_info->get_channel_no()){
//            qWarnxxxxx(qtr("通道号不一致"), m_data_info->get_ip_address(), m_tcp_socket->peerAddress().toString(), c - 1, m_data_info->get_channel_no());
//            reconnect();
//            return;
//        }
        m_sended_signal_info = true;
        send_init_req();
        return;
    }
#endif
//    qDebugx(qtr("接收图片头"));
    QByteArray data = safeRecvbyTcp(m_tcp_socket, 16);
    if(data.length() < 16) {
        qWarnxx(qtr("接收到的数据长度错误"), data.length());
        reconnect();
        return;
    }
//    qDebug()<<data.length();
    PSTREAMHEADER head = (PSTREAMHEADER)data.constData();

    m_last_frame_no = head->frameNo;
    if(head->streamLen <= 0) {
        if(m_getted_image) {
            m_timer->setInterval(m_timer->interval() + 5);
        }
//        qWarnxxxx(qtr("没有图片"), head->streamLen, m_data_info->get_id(), m_data_info->get_name());
#ifndef IPC_NEW_VERSION
        if(m_data_info->get_device_type() == 5) {
            recv_timestamp();
        }
#endif
        return;
    }
    if(head->streamLen > 2 * 1024 * 1024 || head->width > 1920 || head->height > 1080 || head->width <= 0 || head->height <= 0) {
        qWarnxxxxxxx(qtr("收到错误的图片长度"), head->streamLen, head->frameNo, head->frameFlag, head->width, head->height, m_data_info->get_name());
        reconnect();
        return;
    }
    m_getted_image = true;
//    qWarnxxxxxxx(qtr("找到图片"), head->streamLen, head->frameNo, head->frameFlag, head->width, head->height, m_data_info->get_name());
    QByteArray jpg = safeRecvbyTcp(m_tcp_socket, head->streamLen);
    if(jpg.length() != head->streamLen) {
        qWarnxxx(qtr("接收到的数据错误"), jpg.length(), head->streamLen);
        reconnect();
        return;
    }
    QImage img = QImage::fromData(jpg);
    if(!img.isNull()) {
        m_data_info->send_image_incoming_event(img);  // 发送信号到video_item中的槽  二期免费版不显示图像
        m_data_info->set_first_image(img);        //二期免费版不显示图像


//        if(!m_data_info->get_device_status() && m_data_info->get_device_type() != 1){
//            m_timer->stop();
//        }
    }
}

void signal_resource_worker::request_jpg_slot()
{
    if(m_tcp_socket == nullptr) {
        return;
    }
//    if(m_tcp_socket->bytesAvailable()){
//        m_tcp_socket->readAll();
//    }
    REQPACKET p;
    memset(&p, 0, sizeof(REQPACKET));
    p.frameNo = m_last_frame_no;
    m_tcp_socket->write((const char *)&p, sizeof(REQPACKET));
}

void signal_resource_worker::timer_reconnect_slot()
{
    start();
}

void signal_resource_worker::start_image_request_slot()
{
    if(m_timer == nullptr) {
        return;
    }
    if(m_data_info->get_device_status()) {
        request_jpg_slot();
        m_timer->start(40);
    } else {
        m_timer->stop();
    }
}

void signal_resource_worker::status_chanaged_slot(bool val)
{
    if(m_timer == nullptr || m_data_info == nullptr) {
        return;
    }
    if(val) {
        m_timer->start(40);
    } else if(m_data_info->get_device_type() != 1) {
        m_timer->stop();
    }
}
