#ifndef ACC_CONF_INFO_H
#define ACC_CONF_INFO_H

#include "cmd_info.h"

#include <QHash>
#include <QSharedPointer>


class acc_conf_info //包含多个cmd_info类对象的类
{
public:
    acc_conf_info();

    int get_device_type();
    void set_device_type(int val);


    int get_net_type();
    void set_net_type(int val);

    int get_control_bus();
    void set_control_bus(int val);

    QString get_name();
    void set_name(QString val);

    QString get_cmd_type();
    void set_cmd_type(QString val);
    QString get_modelName();
    void set_modelName(QString val);
    QString get_model();
    void set_model(QString val);

    QHash<QString, QSharedPointer<cmd_info>> get_cmds();
    void add_cmd(QSharedPointer<cmd_info> val);
    QSharedPointer<cmd_info> get_cmd_info(QString val);

    int get_sort();
    void set_sort(int val);

private:
    QHash<QString, QSharedPointer<cmd_info>> m_cmds;
    int m_device_type;//设备类型：0-空调；1-灯光；2-窗帘；3-无纸化终端；4-遥控器；5-麦克风
    int m_net_type;//网络类型：0-TCP/IP；1-串口 ;2-udp
    int m_control_bus;
    QString m_name;
    QString m_cmd_type;
    int m_sort;
    QString m_modelName;
    QString m_model;

    QSharedPointer<cmd_info> temp;
};

#endif // ACC_CONF_INFO_H
