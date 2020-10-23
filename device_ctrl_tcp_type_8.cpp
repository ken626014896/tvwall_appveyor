#include "device_ctrl_tcp_type_8.h"
#include "sy_utils.h"
#include "sy_logger.h"
device_ctrl_tcp_type_8::device_ctrl_tcp_type_8(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) :
    device_ctrl_tcp_base(data, conf, parent)

{
}

bool device_ctrl_tcp_type_8::send_cmd(const QString &cmd)
{
    if(device_ctrl_tcp_base::send_cmd(cmd)) {
        //发送成功
        return true;
    } else {
        qDebug() << "send error";
        return false;
    }
}

void device_ctrl_tcp_type_8::recv_data()
{
//    qDebug()<<m_tcp_socket->readAll();
}

void device_ctrl_tcp_type_8::connected()
{
//   qDebug()<<qtr("已连接连接到遥控器");
}
