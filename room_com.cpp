#include "room_com.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QThread>
room_com::room_com(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this); //串口类
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QSerialPort serial;	//在循环语句内部实例化类，因为可用串口不一定只就一个
        serial.setPort(info); //设置能够用的串口端口
        if(serial.open(QIODevice::ReadWrite)) {	//用ReadWrite可读写的方式打开串口
            qDebug() << info.portName();
            serial.close();	//只为了查找串口可以用不可用
        }
    }
    serial->setBaudRate(9600);//设置波特率
    serial->setDataBits(QSerialPort::Data8);//设置数据位
    serial->setStopBits(QSerialPort::OneStop);//设置停止位
    serial->setParity(QSerialPort::OddParity);//设置是否有奇偶校验
//    connect(serial,SIGNAL(readyRead()),this,SLOT(serialPort_readyRead()));
}

void room_com::operation_direction(int o, int a)  //转动方向
{
//    qDebug()<<"send thread"<<QThread::currentThreadId();
    if(o >= 5) { //因为同一个信号绑定了两个槽
        return;
    }
    if (!serial->isOpen()) {
        serial->setPortName("COM1");
        if(serial->open(QIODevice::ReadWrite) == true) {
//                qDebug()<<"open success";
        } else {
            return;
        }//
    }
    QString cmd;
    switch(o) {
        case 1:
            cmd = "81 01 06 01 HH VV 03 01 FF\",\"81 01 06 01 10 10 03 01 FF\"";
            break;
        case 2:
            cmd = "81 01 06 01 HH VV 03 02 FF\",\"81 01 06 01 10 10 03 02 FF\"";
            break;
        case 3:
            cmd = "81 01 06 01 HH VV 01 03 FF\",\"81 01 06 01 10 10  01 03 FF\"";
            break;
        case 4:
            cmd = "81 01 06 01 HH VV 02 03 FF\",\"81 01 06 01 10 10  02 03 FF\"";
            break;
        default:
            qDebug() << "cmd nor";
            break;
    }
    serial->write(QByteArray::fromHex(cmd.toLatin1()));
}

void room_com::operation_zoom(int o, int num)
{
    if(o < 5) {
        return;
    }
    if (!serial->isOpen()) {
        serial->setPortName("COM1");
        if(serial->open(QIODevice::ReadWrite) == true) {
            qDebug() << "open success";
        } else {
            return;
        }//
    }
    QString cmd;
    if(o == 5) //缩小
        switch(num) {
            case 1:
                cmd = "x1";
                break;
            case 2:
                cmd = "x2";
                break;
            case 3:
                cmd = "x3";
                break;
            case 4:
                cmd = "x4";
                break;
            case 5:
                cmd = "x5";
                break;
            case 6:
                cmd = "x6";
                break;
            default:
                qDebug() << "cmd nor";
                break;
        } else if(o == 6) //放大
        switch(num) {
            case 1:
                cmd = "x1";
                break;
            case 2:
                cmd = "x2";
                break;
            case 3:
                cmd = "x3";
                break;
            case 4:
                cmd = "x4";
                break;
            case 5:
                cmd = "x5";
                break;
            case 6:
                cmd = "x6";
                break;
            default:
                qDebug() << "cmd nor";
                break;
        }
    serial->write(QByteArray::fromHex(cmd.toLatin1()));
}

void room_com::closeSerialPort()
{
//     qDebug()<<"close thread"<<QThread::currentThreadId();
    if (serial->isOpen()) {
        serial->close();
    }
}
