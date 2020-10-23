#include "acc_conf_info.h"
#include <QDebug>
acc_conf_info::acc_conf_info():
    m_device_type(0),
    m_net_type(0),
    m_control_bus(0),
    m_name(QString()),
    m_cmd_type(QString()),
    m_sort(-1),
    m_modelName(QString()),
    m_model(QString())
{

}

int acc_conf_info::get_device_type()
{
    return m_device_type;
}

void acc_conf_info::set_device_type(int val)
{
    m_device_type = val;
}

int acc_conf_info::get_net_type()
{
    return m_net_type;
}

void acc_conf_info::set_net_type(int val)
{
    m_net_type = val;
}

int acc_conf_info::get_control_bus()
{
    return  m_control_bus;
}

void acc_conf_info::set_control_bus(int val)
{
    m_control_bus=val;
}

QString acc_conf_info::get_name()
{
    return m_name;
}

void acc_conf_info::set_name(QString val)
{
    m_name = val;
}

QString acc_conf_info::get_cmd_type()
{
    return m_cmd_type;
}

void acc_conf_info::set_cmd_type(QString val)
{
    m_cmd_type = val;
}

QString acc_conf_info::get_modelName()
{
      return m_modelName;
}

void acc_conf_info::set_modelName(QString val)
{
    m_modelName=val;
}

QString acc_conf_info::get_model()
{
    return  m_model;
}

void acc_conf_info::set_model(QString val)
{
    m_model=val;
}

QHash<QString, QSharedPointer<cmd_info> > acc_conf_info::get_cmds()
{
    return m_cmds;
}

void acc_conf_info::add_cmd(QSharedPointer<cmd_info> val)
{
//    if(get_device_type()==5){

//        qDebug()<<val->get_cmd();
//        qDebug()<<val->get_send();
//    }
    m_cmds.insert(val->get_cmd(), val);
}

QSharedPointer<cmd_info> acc_conf_info::get_cmd_info(QString val)
{
//       qDebug()<<get_device_type();
//    if(get_device_type()==5){

//            qDebug()<<val;
//            qDebug()<<m_cmds.value(val)->get_cmd();
//        }
//    qDebug()<<val;
    if(m_cmds.value(val))
      return m_cmds.value(val);
    else {
          qDebug()<<"cmd error";
        return temp;

    }
}

int acc_conf_info::get_sort()
{
    return m_sort;
}

void acc_conf_info::set_sort(int val)
{
    m_sort = val;
}
