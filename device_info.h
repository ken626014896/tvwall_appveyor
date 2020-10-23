#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include <QObject>
#include <QSharedPointer>

#include "data_info.h"
#include "device_thread.h"
#include "acc_conf_info.h"


class device_info : public data_info
{
    Q_OBJECT
public:
    explicit device_info(QObject *parent = nullptr);

    void start();
    void stop();

    void set_acc_conf_info(QSharedPointer<acc_conf_info> val);
signals:

public slots:
    void temperature_changed_slot(int temperature);
    void device_status_changed_slot(bool status);
    void mute_status_changed_slot(int index, bool status);
    void volume_value_slot(int index, int value);



private:
    device_thread *m_device_thread;
    QSharedPointer<acc_conf_info> m_acc_conf_info;
};

#endif // DEVICE_INFO_H
