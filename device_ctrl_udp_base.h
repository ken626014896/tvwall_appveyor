#ifndef DEVICE_CTRL_UDP_BASE_H
#define DEVICE_CTRL_UDP_BASE_H

#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "data_info.h"
#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QDebug>
class device_ctrl_udp_base : public QObject
{
    Q_OBJECT
public:
    explicit device_ctrl_udp_base(QString localIp,int revPort,QString send_ip,int send_port,QObject *parent = nullptr);
     void start();
     void restart();
     void stop();
     void send_cmd(const QString &cmd,data_info *data);
     void init_send_cmd(QString cmd);
//     bool send_cmd(const QString &cmd);
//     bool send_content(const QByteArray &content);
//     bool send_content(const QString &content);
//     bool send_content_by_hex(const QString &content);
signals:
    void device_number_return_information(QString hint);  //中控返回的信息区别设备的信号
    void init_device_number_return_information(QString hint);  //中控返回的信息区别设备的信号
public slots:
     void recv_data();
     void connected();
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void device_ctrl_slot(QString cmd, data_info *data);
private slots:
    void disconnected_status();
    void time_reconnect();



protected slots:


protected:
    QUdpSocket *m_udp_socket;
    data_info *m_data_info;
    QSharedPointer<acc_conf_info> m_acc_conf_info;
    bool isFirstConnect;
    bool isInit;
    QTimer *m_timer;

    QString m_ip_address;
    QString m_local_ip_address;

    int m_port;
    int m_rev_port;
    bool isStop;
    int reconnectnum;
    void reconnected();
};

#endif // DEVICE_CTRL_UDP_BASE_H
