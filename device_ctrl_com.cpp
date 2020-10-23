#include "device_ctrl_com.h"

device_ctrl_com::device_ctrl_com(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) :QObject(parent),
    m_acc_conf_info(conf),
    m_data_info(data)
{

}

void device_ctrl_com::start()
{

}

void device_ctrl_com::stop()
{

}

bool device_ctrl_com::operation_direction(int o, int a)
{
    return true;
}

bool device_ctrl_com::operation_zoom(int o, int num)
{
    return true;
}

void device_ctrl_com::on_noStopSend()
{

}
