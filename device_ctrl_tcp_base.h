#ifndef DEVICE_CTRL_TCP_BASE_H
#define DEVICE_CTRL_TCP_BASE_H

#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "data_info.h"


#include <QObject>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>
class device_ctrl_tcp_base : public device_ctrl_base
{
    Q_OBJECT
public:
    explicit device_ctrl_tcp_base(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);

    virtual void start();
    virtual void stop();
    virtual bool send_cmd(const QString &cmd);
    virtual bool send_content(const QByteArray &content);
    virtual bool send_content(const QString &content);
    virtual bool send_content_by_hex(const QString &content);
signals:

public slots:
    virtual void recv_data();
    virtual void connected();

private slots:
    void disconnected_status();
    void time_reconnect();



protected slots:


protected:
    QTcpSocket *m_tcp_socket;
    bool isFirstConnect;
    QTimer *m_timer;
    void re_connect();
    void reconnected();
};

#endif // DEVICE_CTRL_TCP_BASE_H
