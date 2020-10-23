  #include "device_ctrl_tcp_type_1.h"
#include "sy_logger.h"

device_ctrl_tcp_type_1::device_ctrl_tcp_type_1(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) :
    device_ctrl_tcp_base(data, conf, parent)
{

}

void device_ctrl_tcp_type_1::recv_data()
{
    if(m_tcp_socket == nullptr){
        return;
    }
    qDebugxxxxx(qtr("收到灯光响应"), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port(), QString(m_tcp_socket->readAll()));

}

