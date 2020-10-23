#include "device_ctrl_base.h"

device_ctrl_base::device_ctrl_base(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) : QObject(parent),
    m_acc_conf_info(conf),
    m_data_info(data)
{

}

bool device_ctrl_base::send_cmd(const QString &cmd)
{
    return true;
}

bool device_ctrl_base::send_cmd(const QString &cmd, int index)
{
    return true;
}

bool device_ctrl_base::send_cmd_by_param1(const QString &cmd, int index, int param1)
{
    return true;
}
bool device_ctrl_base::send_cmd_7(int o, int a,int multiple,int win_id)
{
    return true;
}
void device_ctrl_base::start()
{

}

void device_ctrl_base::stop()
{

}

bool device_ctrl_base::is_same_device(data_info *data)
{
    if(data == m_data_info){
        return true;
    }
    return false;
}

void device_ctrl_base::set_acc_conf_info(QSharedPointer<acc_conf_info> conf)
{

}

void device_ctrl_base::device_ctrl_slot(QString cmd, int bit)
{

}

void device_ctrl_base::device_ctrl_slot2(QString cmd, int bit, QSharedPointer<acc_conf_info> conf, QString tempStr)
{

}
