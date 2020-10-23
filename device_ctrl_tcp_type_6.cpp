#include "device_ctrl_tcp_type_6.h"
#include "sy_utils.h"
#include "sy_logger.h"

#ifdef WIN32
#include <qt_windows.h>
#else
#include <arpa/inet.h>
#endif

device_ctrl_tcp_type_6::device_ctrl_tcp_type_6(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) :
    device_ctrl_tcp_base(data, conf, parent),m_accept_msg_hint(false)
{

}

bool device_ctrl_tcp_type_6::send_cmd(const QString &cmd)
{
    qDebugxx(qtr("音响控制"), cmd);
    return device_ctrl_tcp_base::send_cmd(cmd);
}

bool device_ctrl_tcp_type_6::send_cmd(const QString &cmd, int index)
{
//    qDebugxxx(qtr("音响控制"), cmd, index);

    if(cmd == "setEffect"){
        return send_content_by_hex(QString("A5 C3 3C 5A FF 36 02 01 %1 EE").arg(QString("%1").arg(index, 2, 16, QLatin1Char('0'))).replace(" ", ""));
    }
    else if (cmd == "turnVolHigh") {

        return send_content_by_hex(QString("A5 C3 3C 5A FF 36 05 04 %1 %2 00 0A EE").arg(QString("%1").arg(m_data_info->get_channel_type(index - 1), 2, 16, QLatin1Char('0'))).arg(QString("%1").arg(m_data_info->get_channel_flag(index - 1), 2, 16, QLatin1Char('0'))).replace(" ", "").toUpper());
    }
    else if (cmd == "turnVolLow") {

        return send_content_by_hex(QString("A5 C3 3C 5A FF 36 05 04 %1 %2 01 0A EE").arg(QString("%1").arg(m_data_info->get_channel_type(index - 1), 2, 16, QLatin1Char('0'))).arg(QString("%1").arg(m_data_info->get_channel_flag(index - 1), 2, 16, QLatin1Char('0'))).replace(" ", "").toUpper());
    }
    else if (cmd == "muteActive") {
        return send_content_by_hex(QString("A5 C3 3C 5A FF 36 03 03 %1 %2 01 EE").arg(QString("%1").arg(m_data_info->get_channel_type(index - 1), 2, 16, QLatin1Char('0'))).arg(QString("%1").arg(m_data_info->get_channel_flag(index - 1), 2, 16, QLatin1Char('0'))).replace(" ", "").toUpper());
    }
    else if (cmd == "muteIdle") {
        return send_content_by_hex(QString("A5 C3 3C 5A FF 36 03 03 %1 %2 00 EE").arg(QString("%1").arg(m_data_info->get_channel_type(index - 1), 2, 16, QLatin1Char('0'))).arg(QString("%1").arg(m_data_info->get_channel_flag(index - 1), 2, 16, QLatin1Char('0'))).replace(" ", "").toUpper());
    }
    else if (cmd == "getMute") {
        return send_content_by_hex(QString("A5 C3 3C 5A FF 63 03 02 %1 %2 EE").arg(QString("%1").arg(m_data_info->get_channel_type(index - 1), 2, 16, QLatin1Char('0'))).arg(QString("%1").arg(m_data_info->get_channel_flag(index - 1), 2, 16, QLatin1Char('0'))).replace(" ", "").toUpper());
    }
    else if (cmd == "getVol") {
        return send_content_by_hex(QString("A5 C3 3C 5A FF 63 04 02 %1 %2 EE").arg(QString("%1").arg(m_data_info->get_channel_type(index - 1), 2, 16, QLatin1Char('0'))).arg(QString("%1").arg(m_data_info->get_channel_flag(index - 1), 2, 16, QLatin1Char('0'))).replace(" ", "").toUpper());
    }
    return false;
}
//上下两个方法都是生成目标指令 真正发送在device_ctrl_tcp_base这个类
bool device_ctrl_tcp_type_6::send_cmd_by_param1(const QString &cmd, int index, int param1)
{

    qDebugxxxx(qtr("发送音响指令"), cmd, index, param1);
    if(cmd == "setVol"){
        qDebug()<<QString("%1").arg(htons((unsigned short)(param1*10 )), 4, 16, QLatin1Char('0'));
        return send_content_by_hex(QString("A5 C3 3C 5A FF 36 04 04 %1 %2 %3 EE").arg(QString("%1").arg(m_data_info->get_channel_type(index - 1), 2, 16, QLatin1Char('0'))).arg(QString("%1").arg(m_data_info->get_channel_flag(index - 1), 2, 16, QLatin1Char('0'))).replace(" ", "").arg(QString("%1").arg(htons((unsigned short)(param1*10 )), 4, 16, QLatin1Char('0'))).replace(" ", "").toUpper());
    }
    return false;
}

int device_ctrl_tcp_type_6::getVolNum(QByteArray vol)
{
        QString left_num_str=vol.toHex().left(2);
        bool ok;
        int left_num = left_num_str.toInt(&ok, 16);

        int right_num=vol.at(1)<<8;
#ifdef Q_OS_ANDROID
    QString right_num_str=vol.toHex().right(2).toLower();
    if(right_num_str=="fe")
        right_num=-512;
    else if (right_num_str=="ff")
        right_num=-256;
    else if (right_num_str=="00")
        right_num=0;
    else if (right_num_str=="01")
        right_num=256;
#endif
#ifdef Q_OS_IOS
    QString right_num_str=vol.toHex().right(2).toLower();
    if(right_num_str=="fe")
        right_num=-512;
    else if (right_num_str=="ff")
        right_num=-256;
    else if (right_num_str=="00")
        right_num=0;
    else if (right_num_str=="01")
        right_num=256;

#endif
        int result=(left_num+right_num)/10;
        return result;
}

void device_ctrl_tcp_type_6::recv_data()
{
//    if(m_accept_msg_hint){
//        QByteArray res=m_tcp_socket->readAll();
//        //要修两个地方的数据。1，qml中的数据 2，c++中的数据
//         qDebug()<<"tcp6 sync cmd"<<res.toHex();
//         QVariantList list_status;
//         QVariantList list = m_data_info->get_music_sliders();
//         for (int i=0; i<list.count();i++) {
//             QVariantMap map = list.at(i).toMap();
//             if(map.isEmpty()){
//                 continue;
//             }

//                     map["dmStatus"] = true;
//                     map["dmVol"] =  50;


//             list_status.append(map);
//         }
//         m_data_info->set_music_sliders(list_status);

//         emit volume_value_signal(1, res.toHex().toInt());
//         emit mute_status_changed_signal(1, res.toHex().toInt() == 1?true:false);
//    }
//   QByteArray data = safeRecvbyTcp(m_tcp_socket, 13);

//   if(data.length() != 13){
//       qWarnxx(qtr("查询音量接收数据错误"), QString(data.toHex()));

//   }
//   QByteArray vol = data.mid(10, 2);
//   qDebug()<<vol.toHex();
//    int iVol = (vol.at(1) << 8 | vol.at(0)) / 10;
//   qDebug()<<iVol;


//    emit volume_value_signal(1, iVol);

}

void device_ctrl_tcp_type_6::connected()  //查询状态
{

    if(m_data_info == nullptr || m_tcp_socket == nullptr){
        return ;
    }
    QVariantList list_status;
    QVariantList list = m_data_info->get_music_sliders();
    qDebug()<<list.count();
    for (int i=0; i<list.count();i++) {
        QVariantMap map = list.at(i).toMap();
        if(map.isEmpty()){
            continue;
        }
        do{
            if(send_cmd("getMute", i+1)){
                QByteArray data = safeRecvbyTcp(m_tcp_socket, 12);
                if(data.length() != 12){
                    qWarnxx(qtr("查询静音状态接收数据错误"), QString(data.toHex()));
                    break;
                }
                map["dmStatus"] = data.at(10) == 1?true:false;
                qDebugxxxx(qtr("查询到静音状态"), m_data_info->get_name(), i, data.at(10) == 1?true:false);
                emit mute_status_changed_signal(i, data.at(10) == 1?true:false);
//                emit mute_status_changed_signal(i, true);
            }
        }while(0);
        do{
            if(send_cmd("getVol", i+1)){
                QByteArray data = safeRecvbyTcp(m_tcp_socket, 13);
//                map["dmVol"]=50;
                if(data.length() != 13){
                    qWarnxx(qtr("查询音量接收数据错误"), QString(data.toHex()));
                    break;
                }
                QByteArray vol = data.mid(10, 2);
//              int iVol = (vol.at(1) << 8 | vol.at(0)) ;
                int iVol=getVolNum(vol);

                qDebugxxxxx(qtr("查询到音量"), m_data_info->get_name(), i, iVol, QString(vol.toHex()));
                map["dmVol"] =  iVol;
                emit volume_value_signal(i, iVol);
            }
        }while(0);

        list_status.append(map);
    }
    m_data_info->set_music_sliders(list_status);
    m_accept_msg_hint=true;
}
