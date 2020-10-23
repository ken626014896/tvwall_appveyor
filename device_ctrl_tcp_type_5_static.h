#ifndef DEVICE_CTRL_TCP_TYPE_5_STATIC_H
#define DEVICE_CTRL_TCP_TYPE_5_STATIC_H

#include "device_ctrl_base.h"
#include "acc_conf_info.h"
#include "device_ctrl_udp_base.h"
#include <QObject>
#include <QMutex>
#include <QHash>
#include <QSharedPointer>

extern QMutex   g_mutex;
extern QHash<QString, QSharedPointer<device_ctrl_base>> g_hash_ctrl;



extern QSharedPointer<device_ctrl_base> get_device_ctrl(const QString key, data_info *data, QSharedPointer<acc_conf_info> conf);

extern QMutex   u_mutex;
extern QHash<QString, QSharedPointer<device_ctrl_udp_base>> u_hash_ctrl;



extern QSharedPointer<device_ctrl_udp_base> get_device_ctrl2(const QString key,data_info *data);
extern void clear_device_ctrl();


#endif // DEVICE_CTRL_TCP_TYPE_5_STATIC_H
