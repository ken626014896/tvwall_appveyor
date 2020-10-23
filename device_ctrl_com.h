#ifndef DEVICE_CTRL_COM_H
#define DEVICE_CTRL_COM_H

#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "data_info.h"

#include <QObject>

class device_ctrl_com : public QObject
{
    Q_OBJECT
public:
    explicit device_ctrl_com(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);
    virtual void start();
    virtual void stop();

    virtual bool operation_direction(int o,int a);
    virtual bool operation_zoom(int o,int num);
signals:

private slots:
    virtual void on_noStopSend();
protected:
    data_info *m_data_info;
    QSharedPointer<acc_conf_info> m_acc_conf_info;
};

#endif // DEVICE_CTRL_COM_H
