#ifndef DEVICE_WORKER_H
#define DEVICE_WORKER_H

#include "data_info.h"
#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "device_ctrl_com.h"
#include "device_ctrl_udp_base.h"
#include <QObject>

class device_worker : public QObject
{
    Q_OBJECT
public:
    explicit device_worker(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);

    void start();
    void stop();

signals:
    void temperature_changed_signal(int temperature);
    void device_status_changed_signal(bool status);
    void mute_status_changed_signal(int index, bool status);
    void volume_value_signal(int index, int value);
    void device_ctrl_signal(QString cmd, int bit);
    void device_ctrl_signal2(QString cmd,data_info *data);
    void device_ctrl_param1_signal(QString cmd, int bit, int param1);
    void device_ctrl_signal_for_conf(QString cmd, int bit,QSharedPointer<acc_conf_info> conf,QString tempStr);

public slots:
    void device_ctrl_slot(QString cmd);
    void device_ctrl_param_1_slot(QString cmd, int index);
    void device_ctrl_param_2_slot(QString cmd, int index, int param1);
    void device_ctrl_param_3_slot(int o, int a,int multiple,int win_id);
private slots:
    void device_connected_slot();
    void device_status_changed_for_relay_slot(int bit, bool status);

    void device_status_changed_for_udp(QString msg);  //处理udp
    void init_device_status_changed_for_udp(QString msg);  //处理软件打开时返回udp的数据
    void device_status_changed_for_relay_slot2(QString bit, bool status);
private:
    data_info *m_data_info;
    QSharedPointer<acc_conf_info> m_acc_conf_info;
    QSharedPointer<device_ctrl_base> m_device_ctrl;
    QSharedPointer<device_ctrl_com> m_device_ctrl_com;
    QSharedPointer<device_ctrl_base> get_devcie_ctrl_tcp();
    bool is_relay();
    QSharedPointer<device_ctrl_udp_base> m_device_ctrl_udp;

};

#endif // DEVICE_WORKER_H
