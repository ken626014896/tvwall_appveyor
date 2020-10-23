#include "device_ctrl_tcp_type_5_static.h"
#include "device_ctrl_tcp_type_5.h"


#include <QMutexLocker>


QMutex   g_mutex;
QHash<QString, QSharedPointer<device_ctrl_base>> g_hash_ctrl;


QSharedPointer<device_ctrl_base> get_device_ctrl(const QString key, data_info *data, QSharedPointer<acc_conf_info> conf)
{
    QMutexLocker locker(&g_mutex);
    QSharedPointer<device_ctrl_base> ctrl = g_hash_ctrl.value(key);
    if(!ctrl.isNull()){
        qDebug()<<data->get_name();
        return ctrl;
    }
    ctrl = QSharedPointer<device_ctrl_base>(new device_ctrl_tcp_type_5(data, conf));
    g_hash_ctrl.insert(key, ctrl);
    return ctrl;
}
QMutex   u_mutex;
QHash<QString, QSharedPointer<device_ctrl_udp_base>> u_hash_ctrl;


QSharedPointer<device_ctrl_udp_base> get_device_ctrl2(const QString key,data_info *data){

    QMutexLocker locker(&u_mutex);
//    qDebug()<<key;
    QSharedPointer<device_ctrl_udp_base> ctrl = u_hash_ctrl.value(key);
    QStringList temp=key.split("-");
    if(!ctrl.isNull()){


        return ctrl;
    }
    ctrl = QSharedPointer<device_ctrl_udp_base>(new device_ctrl_udp_base(data->get_local_ip_address(),data->getPortRcv(),data->get_ip_address(),data->get_port()));

    u_hash_ctrl.insert(key, ctrl);
    return ctrl;

}

void clear_device_ctrl()
{
    g_hash_ctrl.clear();
    u_hash_ctrl.clear();
}
