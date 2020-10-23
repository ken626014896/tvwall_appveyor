#include "device_worker.h"
#include "device_ctrl_com.h"
#include "device_ctrl_tcp.h"
#include "sy_logger.h"
#include "device_ctrl_tcp_type_0.h"
#include "device_ctrl_tcp_type_1.h"
#include "device_ctrl_tcp_base.h"
#include "device_ctrl_com_base.h"
#include "device_ctrl_tcp_type_2.h"
#include "device_ctrl_tcp_type_5_static.h"
#include "device_ctrl_tcp_type_5.h"
#include "device_ctrl_tcp_type_6.h"
#include "device_ctrl_tcp_type_7.h"
#include "device_ctrl_tcp_type_8.h"
device_worker::device_worker(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) : QObject(parent),
    m_data_info(data),  //该设备对象
    m_acc_conf_info(conf)  //该设备对应的指令对象     conf是acc_conf_info类的对象  包含多个指令对象 即多个cmd_info 对象
{

}
QSharedPointer<device_ctrl_base> device_worker::get_devcie_ctrl_tcp()
{
    if(m_data_info == nullptr || m_acc_conf_info.isNull() || m_data_info->get_ip_address().isNull()) {
        return QSharedPointer<device_ctrl_base>();
    }
    QSharedPointer<device_ctrl_base> ctrl;
    switch (m_data_info->get_device_type()) {  //区别不同的设备
    case 0:   //空调
        ctrl =  QSharedPointer<device_ctrl_base>(new device_ctrl_tcp_type_0(m_data_info, m_acc_conf_info));
        break;
    case 8:   //会议室摄像头
    {

 //       ctrl = get_device_ctrl(QString("%1:%2:%3:%4").arg(m_data_info->get_ip_address()).arg(m_data_info->get_port()).arg(m_data_info->get_device_type()).arg(m_data_info->get_model()), m_data_info, m_acc_conf_info);
        ctrl = get_device_ctrl(QString("%1:%2").arg(m_data_info->get_ip_address()).arg(m_data_info->get_port()), m_data_info, m_acc_conf_info);
            if(!ctrl.isNull()) {
            connect(this, &device_worker::device_ctrl_signal, ctrl.data(), &device_ctrl_base::device_ctrl_slot);
            connect(this, &device_worker::device_ctrl_signal_for_conf, ctrl.data(), &device_ctrl_base::device_ctrl_slot2);

            connect(ctrl.data(), &device_ctrl_base::device_connected_signal, this, &device_worker::device_connected_slot);
        }
    }

    break;
    case 7:   //遥控器
        ctrl =  QSharedPointer<device_ctrl_base>(new device_ctrl_tcp_type_8(m_data_info, m_acc_conf_info));
        break;
    case 2:    //窗帘
        ctrl =  QSharedPointer<device_ctrl_base>(new device_ctrl_tcp_type_2(m_data_info, m_acc_conf_info));
        break;
    case 1:
    case 4:
    case 5:   //屏幕和电源开关和灯光  ??这里为什么要用锁   答这三个设备如果ip+sort相同则使用同一个ctrl对象 也用同一个tcp发送指令
 //       ctrl = get_device_ctrl(QString("%1:%2:%3:%4").arg(m_data_info->get_ip_address()).arg(m_data_info->get_port()).arg( '->get_device_type()).arg(m_data_info->get_model()), m_data_info, m_acc_conf_info);
        ctrl = get_device_ctrl(QString("%1:%2").arg(m_data_info->get_ip_address()).arg(m_data_info->get_port()), m_data_info, m_acc_conf_info);

        if(!ctrl.isNull()) {
            connect(this, &device_worker::device_ctrl_signal, ctrl.data(), &device_ctrl_base::device_ctrl_slot);
            connect(this, &device_worker::device_ctrl_signal_for_conf, ctrl.data(), &device_ctrl_base::device_ctrl_slot2);

            connect(ctrl.data(), &device_ctrl_base::device_connected_signal, this, &device_worker::device_connected_slot);
        }
//        ctrl =  QSharedPointer<device_ctrl_base>(new device_ctrl_tcp_type_5(m_data_info, m_acc_conf_info));
        break;
    case 6:    //音响

        ctrl =  QSharedPointer<device_ctrl_base>(new device_ctrl_tcp_type_6(m_data_info, m_acc_conf_info));
        break;
    default:  //无纸化终端

        ctrl =  QSharedPointer<device_ctrl_base>(new device_ctrl_tcp_base(m_data_info, m_acc_conf_info));
        break;
    }

    return ctrl;
}

void device_worker::start()
{
    if(m_data_info == nullptr || m_acc_conf_info.isNull()) {
        return;
    }
    qDebugxx(qtr("启动设备控制"), m_data_info->get_name());
    if(!m_device_ctrl.isNull()) {
        m_device_ctrl->start();
        return;
    }
    if(!m_device_ctrl_udp.isNull()) {
        m_device_ctrl_udp->start();
        return;
    }
    if(m_data_info->get_model()!="-1"){  //tcp或串口连接

        switch (m_acc_conf_info->get_net_type()) { //目前都是0   0是tcp 1是串口 2是udp
        case 0:
            m_device_ctrl = get_devcie_ctrl_tcp();
            break;
        case 1:
    //        m_device_ctrl = QSharedPointer<device_ctrl_base>(new device_ctrl_com(m_data_info, m_acc_conf_info));
            break;

        case 2:{
//            m_device_ctrl_udp = get_device_ctrl2(QString("123"),m_data_info->get_ip_address(),m_data_info->get_port());

            break;
        }

        default:
            break;
        }

        if(m_device_ctrl.isNull()) {
            qWarnxxx(qtr("不支持的设备类型"), m_data_info->get_device_type(), m_acc_conf_info->get_net_type());
            return;
        }
        //发送完指令后的回调信号
        connect(m_device_ctrl.data(), &device_ctrl_base::temperature_changed_signal, this, &device_worker::temperature_changed_signal); //空调的回调信号
        connect(m_device_ctrl.data(), &device_ctrl_base::device_status_changed_signal, this, &device_worker::device_status_changed_signal);
        connect(m_device_ctrl.data(), &device_ctrl_base::mute_status_changed_signal, this, &device_worker::mute_status_changed_signal);
        connect(m_device_ctrl.data(), &device_ctrl_base::volume_value_signal, this, &device_worker::volume_value_signal);
        connect(m_device_ctrl.data(), &device_ctrl_base::device_status_changed_for_relay_signal, this, &device_worker::device_status_changed_for_relay_slot);
        connect(m_device_ctrl.data(), &device_ctrl_base::device_status_changed_for_relay_signal2, this, &device_worker::device_status_changed_for_relay_slot2);

        m_device_ctrl->start();

    }

    else if (m_data_info->get_model()=="-1") {   //udp连接
         //基本公用一个udp

        m_device_ctrl_udp = get_device_ctrl2(QString("%1%2").arg(m_data_info->get_local_ip_address()).arg(m_data_info->getPortRcv()),m_data_info);
//        m_device_ctrl_udp=QSharedPointer<device_ctrl_udp_base>(new device_ctrl_udp_base(m_data_info->get_local_ip_address(),m_data_info->getPortRcv(),m_data_info->get_ip_address(),m_data_info->get_port()));
//        qDebug()<< m_device_ctrl_udp;
        if(!m_device_ctrl_udp.isNull()) {
             connect(this, &device_worker::device_ctrl_signal2, m_device_ctrl_udp.data(), &device_ctrl_udp_base::device_ctrl_slot);
             connect(m_device_ctrl_udp.data(), &device_ctrl_udp_base::device_number_return_information, this,&device_worker::device_status_changed_for_udp);
             connect(m_device_ctrl_udp.data(), &device_ctrl_udp_base::init_device_number_return_information, this,&device_worker::init_device_status_changed_for_udp);
         }
//         connect(this, &device_worker::device_ctrl_signal2, m_device_ctrl_udp.data(), &device_ctrl_udp_base::device_ctrl_slot);
//         connect(m_device_ctrl_udp.data(), &device_ctrl_udp_base::device_number_return_information, this,&device_worker::device_status_changed_for_udp);
//         connect(m_device_ctrl_udp.data(), &device_ctrl_udp_base::init_device_number_return_information, this,&device_worker::init_device_status_changed_for_udp);

         if(m_data_info->get_ip_address().isEmpty() || m_data_info->get_port() <= 0){
             qDebugxxx(qtr("参数错误2"), m_data_info->get_ip_address(), m_data_info->get_port());
             return;
         }
         qDebugxxxx(qtr("启动设备连接"), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());

         m_device_ctrl_udp->start();
    }





}

void device_worker::stop()
{

    clear_device_ctrl();

    if(!m_device_ctrl.isNull()) {
        if(m_device_ctrl->is_same_device(m_data_info)) {
            m_device_ctrl->stop();        }
//        m_device_ctrl->stop();
    }
    if(!m_device_ctrl_com.isNull()) {

        m_device_ctrl_com->stop();

    }
    if(!m_device_ctrl_udp.isNull()) {
//       qDebug()<<"'ssssssssssssss";
        m_device_ctrl_udp->stop();

    }
}

void device_worker::device_ctrl_slot(QString cmd)   //发送指令
{

    // 2020-04-23 二期后期要区分是tcp还是udp
    if(m_data_info->get_model()!="-1"){

        if(m_device_ctrl.isNull()) {
            qDebugx(qtr("未能创建支持的设备控制"));
            return;
        }

        if(is_relay()) { //灯光 ，屏幕 ，电源走这里
            QString tempStr=QString("%1:%2:%3").arg(m_data_info->get_channel_no()).arg(m_data_info->get_model()).arg(m_data_info->get_device_type());

             emit device_ctrl_signal_for_conf(cmd, m_data_info->get_channel_no(),m_acc_conf_info,tempStr);

            //emit device_ctrl_signal(cmd, m_data_info->get_channel_no());  //信号指向device_ctrl_base这个类   tcp连接在device_ctrl_tcp_type_5这个类
        }
        else if(!m_device_ctrl->send_cmd(cmd)) { //空调走这里 其他待定
            qWarnxx(qtr("设备控制失败"), cmd);
        }
    }

    else if (m_data_info->get_model()=="-1") {

        if(m_device_ctrl_udp.isNull()) {
            qDebugx(qtr("未能创建支持的设备控制"));
            return;
        }
         QSharedPointer<cmd_info> info = m_data_info->get_udp_cmds_info(cmd);
        if(is_relay()) { //灯光 ，屏幕 ，电源走这里
            emit device_ctrl_signal2(info->get_send(), m_data_info);  //udp连接在device_ctrl_udp_base这个类
        }

        else { //除了音频和会议室摄像头，都走这里
            m_device_ctrl_udp->send_cmd(info->get_send(),m_data_info);
            qWarnxx(qtr("设备控制失败"), cmd);
        }
    }


}

bool device_worker::is_relay()
{
    if(m_data_info == nullptr) {
        return false;
    }
    if(m_data_info->get_device_type() == 1 ||
            m_data_info->get_device_type() == 4 ||
            m_data_info->get_device_type() == 5||
            m_data_info->get_device_type() == 8) {
        return true;
    }
    return false;
}

void device_worker::device_ctrl_param_1_slot(QString cmd, int index)  //音频
{
    qDebugxxx(qtr("带参数的控制"), cmd, index);
    if(m_data_info->get_model()!="-1"){
        if(m_device_ctrl.isNull()) {
            qDebugx(qtr("未能创建支持的设备控制"));
            return;
        }

        if(!m_device_ctrl->send_cmd(cmd, index)) {
            qWarnxxx(qtr("设备控制失败"), cmd, index);
        }

    }
    else if (m_data_info->get_model()=="-1") {
     //暂时没有

    }


}

void device_worker::device_ctrl_param_2_slot(QString cmd, int index, int param1)  //音频
{
   if(m_data_info->get_model()!="-1"){
       if(m_device_ctrl.isNull()) {
           qDebugx(qtr("未能创建支持的设备控制"));
           return;
       }
       if(!m_device_ctrl->send_cmd_by_param1(cmd, index, param1)) {
           qWarnxxxx(qtr("设备控制失败"), cmd, index, param1);
       }
   }
   else if (m_data_info->get_model()=="-1") {
    //暂时没有

   }

}

void device_worker::device_ctrl_param_3_slot(int o, int a,int multiple,int win_id)   //发送会议室摄像头相关指令
{
    if(m_device_ctrl.isNull()) {
        qDebugx(qtr("未能创建支持的设备控制"));
        return;
    }

    if(!m_device_ctrl->send_cmd_7(o, a, multiple,win_id)) { //tcp发送失败时，用串口发送
        qWarnxxxx(qtr("设备控制失败"), o, a, multiple);

        if(o<=4) {
            if(!m_device_ctrl_com->operation_direction(o,a))
                qWarnxxxx(qtr("通过串口设备控制失败1"), o, a, multiple);

        }
        else if(o>=5&&a==0) {
            if(!m_device_ctrl_com->operation_zoom(o,multiple))
            {
                qWarnxxxx(qtr("通过串口设备控制失败2"), o, a, multiple);

            }

        }


    }
}

void device_worker::device_connected_slot()
{
    if(m_data_info == nullptr || m_device_ctrl.isNull()) {
        return;
    }
    emit device_ctrl_signal("read", m_data_info->get_channel_no());
}

void device_worker::device_status_changed_for_relay_slot(int bit, bool status)  //查询继电器状态的回调槽
{

    if(m_data_info == nullptr || m_device_ctrl.isNull() || m_data_info->get_channel_no() != bit) {
        return;
    }
    qDebugxxxx(qtr("继电器状态"), m_data_info->get_name(), bit, status);
    emit device_status_changed_signal(status);
}

void device_worker::device_status_changed_for_udp(QString msg) //处理单个设备的返回信息
{
    qDebug()<<msg;
    if(m_data_info == nullptr || m_device_ctrl_udp.isNull() ||! m_data_info->is_in_cmd(msg)) {
        return;
    }
     qDebug()<<msg;
    QString cmd=m_data_info->get_cmd_from_msg(msg);
    if(cmd.contains("CenterActive"))
       emit device_status_changed_signal(true);
    else if(cmd.contains("CenterIdle"))
       emit device_status_changed_signal(false);

}

void device_worker::init_device_status_changed_for_udp(QString msg) //打开软件时，处理多个设备的返回信息

{
//    qDebug()<<"init"<<msg;
    if(m_data_info == nullptr || m_device_ctrl_udp.isNull() ) {
        return;
    }
    QHash<QString, QSharedPointer<cmd_info> > temp_hash=m_data_info->get_udp_cmds();


      foreach(QSharedPointer<cmd_info> cmd_obj, temp_hash.values()) {
          if(msg.contains(cmd_obj->get_suc())){

              if(cmd_obj->get_cmd().contains("CenterActive"))
                  emit device_status_changed_signal(true);
              else if(cmd_obj->get_cmd().contains("CenterIdle"))
                 emit device_status_changed_signal(false);
          }

      }

}

void device_worker::device_status_changed_for_relay_slot2(QString bit, bool status)
{
    QString tempStr=QString("%1:%2:%3").arg(m_data_info->get_channel_no()).arg(m_data_info->get_model()).arg(m_data_info->get_device_type());
        if(m_data_info == nullptr || m_device_ctrl.isNull() || tempStr != bit) {
            return;
        }
        qDebugxxxx(qtr("继电器状态"), m_data_info->get_name(), bit, status);
        emit device_status_changed_signal(status);
}


