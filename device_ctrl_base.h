#ifndef DEVICE_CTRL_BASE_H
#define DEVICE_CTRL_BASE_H

#include "acc_conf_info.h"
#include "data_info.h"


#include <QObject>


class device_ctrl_base : public QObject
{
    Q_OBJECT
public:
    explicit device_ctrl_base(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);

    virtual bool send_cmd(const QString &cmd);
    virtual bool send_cmd(const QString &cmd, int index);
    virtual bool send_cmd_by_param1(const QString &cmd, int index, int param1);
    virtual bool send_cmd_7(int o, int a,int multiple,int win_id);
    virtual void start();
    virtual void stop();
    bool is_same_device(data_info *data);
    virtual void set_acc_conf_info(QSharedPointer<acc_conf_info> conf);
signals:
    void temperature_changed_signal(int temperature);
    void device_status_changed_signal(bool status);
    void device_status_changed_for_relay_signal(int bit, bool status);
    void device_status_changed_for_relay_signal2(QString bit, bool status);

    void mute_status_changed_signal(int index, bool status);
    void volume_value_signal(int index, int value);
    void device_connected_signal();

public slots:
    virtual void device_ctrl_slot(QString cmd, int bit);
    virtual void device_ctrl_slot2(QString cmd, int bit,QSharedPointer<acc_conf_info> conf,QString tempStr);

protected:
    data_info *m_data_info;
    QSharedPointer<acc_conf_info> m_acc_conf_info;
};

#endif // DEVICE_CTRL_BASE_H
