#ifndef DEVICE_CTRL_TCP_TYPE_0_H
#define DEVICE_CTRL_TCP_TYPE_0_H

#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "data_info.h"
#include "device_ctrl_tcp_base.h"

#include <QObject>
#include <QTcpSocket>


class device_ctrl_tcp_type_0 : public device_ctrl_tcp_base
{
    Q_OBJECT
public:
    explicit device_ctrl_tcp_type_0(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);

    bool send_cmd(const QString &cmd);
signals:

public slots:
protected slots:
    void recv_data();
    void connected();

private:
    int m_temperature;

};

#endif // DEVICE_CTRL_TCP_TYPE_0_H
