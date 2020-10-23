#include "device_ctrl_tcp_type_0.h"
#include "sy_utils.h"
#include "sy_logger.h"


#pragma pack(push)
#pragma pack(1)
typedef struct{
    unsigned char begin;
    unsigned char address;
    unsigned char func;
    unsigned short reg_address;
    unsigned char data;
    unsigned short crc;
    unsigned char end;
}RACC_CMD,*PRACC_CMD;

typedef struct{
    unsigned char begin;
    unsigned char address;
    unsigned char func;
    unsigned short temp;
    unsigned short crc;
}RACC_TEMP,*PRACC_TEMP;
#pragma pack(pop)


device_ctrl_tcp_type_0::device_ctrl_tcp_type_0(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) :
    device_ctrl_tcp_base(data, conf, parent),
    m_temperature(20)
{

}

bool device_ctrl_tcp_type_0::send_cmd(const QString &cmd)   //空调的发送指令
{
    if(m_tcp_socket == nullptr || m_acc_conf_info.isNull() || !m_tcp_socket->isOpen()){
        return false;
    }
    if(cmd == "btnRight"){
        if(device_ctrl_tcp_base::send_cmd(QString("btn%1").arg(m_temperature - 1))){
            m_temperature -= 1;
            emit temperature_changed_signal(m_temperature);
            return true;
        }
    }
    else if(cmd == "btnLeft"){
        if(device_ctrl_tcp_base::send_cmd(QString("btn%1").arg(m_temperature + 1))){
            m_temperature += 1;
            emit temperature_changed_signal(m_temperature);
            return true;
        }
    }
    else if(cmd == "btnBottomCenterActive"){   //空调的开
        if(device_ctrl_tcp_base::send_cmd(cmd)){
            safeRecvbyTcp(m_tcp_socket, 7);
            if(device_ctrl_tcp_base::send_cmd(QString("btn20"))){
                m_temperature = 20;
                qDebugxx(qtr("查询到温度"), m_temperature);
                emit temperature_changed_signal(m_temperature);
                return true;
            }
        }
    }
    else{  //空调的加冷和加热 和空调的关  和其他命令
        return device_ctrl_tcp_base::send_cmd(cmd);
    }
    return false;
}

void device_ctrl_tcp_type_0::recv_data()
{

}

void device_ctrl_tcp_type_0::connected()
{
    qDebugxxxxx(qtr("收到空调响应"), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port(), QString(m_tcp_socket->readAll()));
    if(!send_cmd("getCurrent")){     //获取空调当前状态 是开还是关
        qWarnxx(qtr("获取当前状态失败"), m_data_info->get_name());
        return;
    }
    if(!m_tcp_socket->waitForReadyRead(3000)){
        qWarnxx(qtr("获取当前状态无返回"), m_data_info->get_name());
        return;
    }
    QSharedPointer<cmd_info> info = m_acc_conf_info->get_cmd_info("getCurrent");
    if(info.isNull()){
        qWarnxxxxx(qtr("不支持的指令"), "getCurrent", m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());
        return;
    }
    QString rv = QString(m_tcp_socket->readAll().toHex());
    if(info->get_suc() == rv){
        m_data_info->set_device_status(true);
    }
    else {
        qWarnxx(qtr("空调状态返回"), rv);
        m_data_info->set_device_status(false);
    }
    emit device_status_changed_signal(m_data_info->get_device_status());
}
