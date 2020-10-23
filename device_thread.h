#ifndef DEVICE_THREAD_H
#define DEVICE_THREAD_H

#include "data_info.h"
#include "acc_conf_info.h"

#include <QObject>
#include <QThread>

class device_thread : public QThread
{
    Q_OBJECT
public:
    explicit device_thread(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);

signals:
    void device_ctrl_signal(QString cmd);
    void device_ctrl_param_1_signal(QString cmd, int index);
    void device_ctrl_param_2_signal(QString cmd, int index, int param1);
    void device_ctrl_param_3_signal(int o, int a,int multiple,int win_id);

    void temperature_changed_signal(int temperature);
    void device_status_changed_signal(bool status);
    void mute_status_changed_signal(int index, bool status);
    void volume_value_signal(int index, int value);


public slots:

protected:
    void run();

private:
    data_info *m_data_info;
    QSharedPointer<acc_conf_info> m_acc_conf_info;
};

#endif // DEVICE_THREAD_H
