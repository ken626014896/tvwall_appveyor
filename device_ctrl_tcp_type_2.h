#ifndef DEVICE_CTRL_TCP_TYPE_2_H
#define DEVICE_CTRL_TCP_TYPE_2_H

#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "data_info.h"
#include "device_ctrl_tcp_base.h"

#include <QObject>
#include <QTcpSocket>

class device_ctrl_tcp_type_2 : public device_ctrl_tcp_base
{
    Q_OBJECT
public:
    explicit device_ctrl_tcp_type_2(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);

signals:

public slots:
};

#endif // DEVICE_CTRL_TCP_TYPE_2_H
