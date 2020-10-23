#include "device_ctrl_com_base.h"
#include "sy_logger.h"
#include <QSerialPortInfo>
#include <QTimer>
#include <QDebug>
device_ctrl_com_base::device_ctrl_com_base(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) :
    device_ctrl_com(data, conf, parent)
{
    serial=new QSerialPort(); //串口类

//    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
//        {

//            QSerialPort serial;	//在循环语句内部实例化类，因为可用串口不一定只就一个
//            serial.setPort(info); //设置能够用的串口端口

//            if(serial.open(QIODevice::ReadWrite))	//用ReadWrite可读写的方式打开串口
//            {
//               qDebug()<<info.portName();
//               serial.close();	//只为了查找串口可以用不可用
//            }

//        }


    serial->setBaudRate(9600);//设置波特率

    serial->setDataBits(QSerialPort::Data8);//设置数据位

    serial->setStopBits(QSerialPort::OneStop);//设置停止位

    serial->setParity(QSerialPort::OddParity);//设置是否有奇偶校验
    timer=new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(on_noStopSend()));
}

void device_ctrl_com_base::start()
{
    QString com="COM"+QString::number(m_data_info->get_com());
    serial->setPortName("COM1");
    if(serial->open(QIODevice::ReadWrite) == true){
               qDebugx(qtr("串口打开成功"));
      }else {
           qDebugx(qtr("串口打开失败"));
             return;
      }
    device_ctrl_com::start();
}

void device_ctrl_com_base::stop()
{
    if(serial != nullptr){
        serial->close();
        delete serial;
        serial = nullptr;
    }
    delete  timer;
    device_ctrl_com::stop();
}

bool device_ctrl_com_base::operation_direction(int o, int a)
{
    if (serial->isOpen()){
        if(a==0){
            switch(o){
                case 1:
                   cmd="81 01 06 01 HH VV 03 01 FF\",\"81 01 06 01 10 10 03 01 FF\"";
                   break;
                case 2:
                   cmd="81 01 06 01 HH VV 03 02 FF\",\"81 01 06 01 10 10 03 02 FF\"";
                   break;
                case 3:
                   cmd="81 01 06 01 HH VV 01 03 FF\",\"81 01 06 01 10 10  01 03 FF\"";
                   break;
                case 4:
                   cmd="81 01 06 01 HH VV 02 03 FF\",\"81 01 06 01 10 10  02 03 FF\"";
                   break;
                default:
    //               qDebug()<<"cmd nor";
                break;
                               }

            timer->start(200);

        }


        else{

            timer->stop();
        }
        return true;

    }
    else {
        qDebugx(qtr("串口没有打开"));
        return false;
    }
}

bool device_ctrl_com_base::operation_zoom(int o, int num)
{
    if (serial->isOpen()){

        if(o==5) //缩小
            switch(num){
                case 1:
                   cmd="81  01 04 47 00 00  00 00 FF";
                   break;
                case 2:
                   cmd="81  01 04 47 01 09  0D 04 FF";
                   break;
                case 3:
                   cmd="81  01 04 47 02 04  0F 03 FF";
                   break;
                case 4:
                   cmd="81  01 04 47 02 0C  0C 0D FF";
                   break;
                case 5:
                   cmd="81  01 04 47 03 01  0B 01 FF";
                   break;
                case 6:
                   cmd="81  01 04 47 03 05  03 0C FF";
                   break;

                default:

                break;
                       }
        else if(o==6)  //放大
            switch(num){
                case 1:
                   cmd="81  01 04 47 00 00  00 00 FF";
                   break;
                case 2:
                   cmd="81  01 04 47 01 09  0D 04 FF";
                   break;
                case 3:
                   cmd="81  01 04 47 02 04  0F 03 FF";
                   break;
                case 4:
                   cmd="81  01 04 47 02 0C  0C 0D FF";
                   break;
                case 5:
                   cmd="81  01 04 47 03 01  0B 01 FF";
                   break;
                case 6:
                   cmd="81  01 04 47 03 05  03 0C FF";
                   break;

                default:

                break;
                       }
        serial->write(QByteArray::fromHex(cmd.toLatin1()));

    }
    else {
        qDebug()<<"com is not open";
        qDebugx(qtr("串口没有打开"));
        return false;
    }
}

void device_ctrl_com_base::on_noStopSend()
{
       serial->write(QByteArray::fromHex(cmd.toLatin1()));
}


