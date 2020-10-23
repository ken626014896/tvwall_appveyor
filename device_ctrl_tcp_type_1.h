#ifndef DEVICE_CTRL_TCP_TYPE_1_H
#define DEVICE_CTRL_TCP_TYPE_1_H

#include <QObject>
#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "data_info.h"
#include "device_ctrl_tcp_base.h"

#include <QObject>
#include <QTcpSocket>



class device_ctrl_tcp_type_1 : public device_ctrl_tcp_base
{
    Q_OBJECT
public:
    explicit device_ctrl_tcp_type_1(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);


signals:

public slots:

protected slots:
    void recv_data();
};

#endif // DEVICE_CTRL_TCP_TYPE_1_H
