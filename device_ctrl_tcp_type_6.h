#ifndef DEVICE_CTRL_TCP_TYPE_6_H
#define DEVICE_CTRL_TCP_TYPE_6_H

#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "data_info.h"
#include "device_ctrl_tcp_base.h"

#include <QObject>
#include <QTcpSocket>


class device_ctrl_tcp_type_6 : public device_ctrl_tcp_base
{
    Q_OBJECT
public:
    explicit device_ctrl_tcp_type_6(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);

    bool send_cmd(const QString &cmd);
    bool send_cmd(const QString &cmd, int index);
    bool send_cmd_by_param1(const QString &cmd, int index, int param1);
    int getVolNum(QByteArray vol);

signals:

public slots:
protected slots:
    void recv_data();
    void connected();
private:
    bool m_accept_msg_hint;
};

#endif // DEVICE_CTRL_TCP_TYPE_6_H
