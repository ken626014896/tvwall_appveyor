#ifndef DEVICE_CTRL_TCP_TYPE_7_H
#define DEVICE_CTRL_TCP_TYPE_7_H

#include <QObject>
#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "data_info.h"
#include "device_ctrl_tcp_base.h"

#include <QTcpSocket>
class device_ctrl_tcp_type_7 : public device_ctrl_tcp_base
{
    Q_OBJECT
public:
    explicit device_ctrl_tcp_type_7(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);

    bool send_cmd_7(int o, int a,int multiple,int win_id);
signals:

public slots:
protected slots:
    void recv_data();
    void connected();
private:
    void add_cmd_header(QByteArray &data, int cmd, int len);
    int m_packet_id;
};

#endif // DEVICE_CTRL_TCP_TYPE_7_H
