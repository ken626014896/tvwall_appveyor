#ifndef DEVICE_CTRL_TCP_H
#define DEVICE_CTRL_TCP_H

#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "data_info.h"


#include <QObject>
#include <QTcpSocket>

class device_ctrl_tcp : public device_ctrl_base
{
    Q_OBJECT
public:
    explicit device_ctrl_tcp(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);

    void start();
    void stop();
    bool send_cmd(const QString &cmd);
signals:

public slots:

private slots:
    void recv_data();
    void connected();
private:
    QTcpSocket *m_tcp_socket;
    int m_temperature;
    bool btn_on();
    bool btn_off();
    void set_cmd(char *cmd_s, unsigned char func, unsigned char data);
    bool send_cmd_ex(const QString &cmd);
};

#endif // DEVICE_CTRL_TCP_H
