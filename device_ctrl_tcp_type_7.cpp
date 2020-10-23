#include "device_ctrl_tcp_type_7.h"
#include "sy_utils.h"
#include "sy_logger.h"
#pragma pack(push, 1)
typedef struct {
    int len;
    int id;
    int cmd;
} CMD_HEADER;



#pragma pack(pop)
device_ctrl_tcp_type_7::device_ctrl_tcp_type_7(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) :
    device_ctrl_tcp_base(data, conf, parent), m_packet_id(0)
{
}

bool device_ctrl_tcp_type_7::send_cmd_7(int o, int a, int multiple, int win_id)
{
    if(m_tcp_socket == nullptr || m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return false;
    }
    char temp = 0x00;
    char oper = (char)o;
    char act = (char)a;
    QByteArray data;
    add_cmd_header(data, 0x2a, 4 + 4 + 4 + 1 + 3 + 1 + 3);
    add_data_to_byte_array(data, (char *)&multiple, sizeof(int));
    add_data_to_byte_array(data, (char *)&oper, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&act, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
    add_data_to_byte_array(data, (char *)&temp, sizeof(char));
//    m_tcp_socket->write(data);
//    QByteArray res = get_response();
//    PSET_POLLING_RESPONSE rv = (PSET_POLLING_RESPONSE)res.constData();
//    if(rv->status != 0){
//        qWarnxx(qtr("停止轮巡失败"), rv->status);
//        return;
//    }
    if(device_ctrl_tcp_base::send_content(data)) {
        //发送成功
        return true;
    } else {
        return false;  //证明没连上,开始用串口通信替代
    }
}

void device_ctrl_tcp_type_7::recv_data()
{
}

void device_ctrl_tcp_type_7::connected()
{
}

void device_ctrl_tcp_type_7::add_cmd_header(QByteArray &data, int cmd, int len)
{
    CMD_HEADER cmd_header;
    cmd_header.len = len;
    cmd_header.id = m_packet_id ++;
    cmd_header.cmd = cmd;
    add_data_to_byte_array(data, (char *)&cmd_header, sizeof(CMD_HEADER));
}
