#ifndef DEVICE_CTRL_COM_BASE_H
#define DEVICE_CTRL_COM_BASE_H

#include <QObject>
#include "acc_conf_info.h"
#include "device_ctrl_com.h"
#include "data_info.h"
#include <QSerialPort>
class device_ctrl_com_base:public device_ctrl_com
{
public:
    device_ctrl_com_base(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);
    virtual void start();
    virtual void stop();

    virtual bool operation_direction(int o,int a);
    virtual bool operation_zoom(int o,int num);

//signals:
//    void canSend(QByteArray);
private slots:
    virtual void on_noStopSend();
private:
    QSerialPort *serial;
    QString cmd;
    QTimer *timer;


};

#endif // DEVICE_CTRL_COM_BASE_H
