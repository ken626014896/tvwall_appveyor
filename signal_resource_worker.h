#ifndef SIGNAL_RESOURCE_WORKER_H
#define SIGNAL_RESOURCE_WORKER_H

#include "data_info.h"
//#include "evwsdk.h"

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QFile>



class signal_resource_worker : public QObject
{
    Q_OBJECT
public:
    explicit signal_resource_worker(data_info *data, QObject *parent = nullptr);
    ~signal_resource_worker();

    void start();
    void stop();

signals:


public slots:

private slots:
    void connected();
    void recv_data();
    void disconnected_status();
    void request_jpg_slot();
    void timer_reconnect_slot();
    void start_image_request_slot();
    void status_chanaged_slot(bool val);

    void reconnect();
private:
    QTcpSocket *m_tcp_socket;
    data_info *m_data_info;
    QTimer *m_timer;
    int m_last_frame_no;
    bool m_getted_image;
    QTimer *m_timer_reconnect;
    bool m_sended_signal_info;
    int m_echo_ip_index;

//    void reconnect();
    void recv_timestamp();
    void send_init_req();
    bool send_signal_info();
};

#endif // SIGNAL_RESOURCE_WORKER_H
