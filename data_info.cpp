#include "data_info.h"
#include "sy_utils.h"
#include "sy_logger.h"
#include <qmath.h>
#include <QCoreApplication>
#include <QDir>
#include <QtDebug>
#include <QVariantMap>

data_info::data_info(QObject *parent) : QObject(parent),
    m_name(QString()),
    m_ico_path(QString()),
    m_image(QPixmap()),
    m_first_image(QImage()),
    m_phy_wall_row(2),
    m_phy_wall_column(2),
    m_device_air_slider(false),
    m_device_air_switch(false),
    m_device_type(0),
    m_device_status(false),
    m_ctrl_progress(QString("50")),
    m_display_x(0),
    m_display_y(0),
    m_display_width(0),
    m_display_height(0),
    m_display_x_old(0),
    m_display_y_old(0),
    m_display_column(-1),
    m_display_column_num(-1),
    m_display_row(-1),
    m_display_row_num(-1),
    m_display_width_old(0),
    m_display_height_old(0),
    m_logical_wall_row(2),
    m_logical_wall_column(2),
    m_wall_width(1920),
    m_wall_height(1080),
    m_win_id(0),
    m_index(-1),
    m_port(6801),
    m_portSend(6801),
    m_portRcv(6801),
    m_channel_no(0),
    m_sort(-1),
    m_user_name(QString()),
    m_group_name(QString()),
    m_user_pwd(QString()),
    m_isMe(false),
    m_polling_hint(-1),
    m_isModeChanged(false),
    m_isModeChanged_hint(0),
    m_volume(50),
    m_audio_id(0),
    m_audio_channel(0),
    m_modelName(QString()),
    m_model(QString()),
    m_cmd_type(QString()),
    m_local_ip_address(QString()),
     m_is_led(false)

{
    memset(m_signal_source_cmd, 0, sizeof(m_signal_source_cmd));
}


data_info::~data_info()
{
    stop();
}

QString data_info::get_name()
{
    return m_name;
}

void data_info::set_name(QString val)
{
    m_name = val;
}

QString data_info::get_modelName()
{
    return  m_modelName;
}

void data_info::set_modelName(QString val)
{
    m_modelName=val;
}

int data_info::get_index()
{
    return m_index;
}

void data_info::set_index(int val)
{
    m_index = val;
}

QString data_info::get_model() //对应相同设备不同的牌子
{
    return m_model;
}

void data_info::set_model(QString val)
{
    m_model=val;
}

QString data_info::get_ico_path()
{
    return m_ico_path;
}

void data_info::set_ico_path(QString val)
{
    m_ico_path = val;
}

QPixmap data_info::get_image()
{
    return m_image;
}

void data_info::set_image(const QPixmap &val)
{
    m_image = val;
    emit image_changed_signal(m_index);
}

QImage data_info::get_first_image()
{
    return m_first_image;
}

void data_info::set_first_image(const QImage &val)
{
    m_first_image = val;
}

void data_info::add_polling(QSharedPointer<data_info> val)
{
    m_list_polling.append(val);
}

QVariantList data_info::get_list_polling()
{
    QVariantList list;
    for(int i=0;i<m_list_polling.count(); i++) {
        QSharedPointer<data_info> info = m_list_polling.at(i);
        if(info.isNull()){
            continue;
        }
        QVariantMap map;
        map.insert("dmName", info->get_name());

        list.append(map);
    }

    return list;
}

void data_info::clear_polling()
{
    m_list_polling.clear();
}

void data_info::send_stop_polling_event(int index)
{
//    qDebug()<<"stop"<<index;
    emit stop_polling(index);
}
void data_info::send_open_polling_event(int index)
{
//    qDebug()<<"open"<<index;
    emit open_polling(index);
}
void data_info::send_stop_and_open_polling_event(int o_index, int s_index)
{
//    qDebug()<<"stop"<<s_index<<","<<"open"<<o_index;

    emit stop_and_open_polling(o_index,s_index);
}

QSharedPointer<data_info> data_info::get_polling_by_id(int index)
{
    foreach(QSharedPointer<data_info> info, m_list_polling){
//        qDebugxxxxxx(qtr("查询信号源"), m_list_signal_source.count(), id, info->get_id(), info->get_device_type(), info->get_name());
        if(info->get_index() == index){
//            qDebugxxxxx(qtr("找到信号源"), id, info->get_id(), info->get_device_type(), info->get_name());
            return info;
        }


        }


    return QSharedPointer<data_info>();
}

void data_info::send_change_signal_source_to_wall_signal_event(QSharedPointer<data_info> val)
{
    if(val.isNull()){
        return;
    }
    emit change_signal_source_to_wall_signal(val);
}

void data_info::send_change_signal_source_to_wall_signal_by_old_event(QSharedPointer<data_info> val)
{
    if(m_old_data_info.isNull()){
        qDebugx(qtr("未设置原始数据"));
        return;
    }
    //m_old_data_info是指c区的信号源 即b区的信号
    m_old_data_info->send_change_signal_source_to_wall_signal_event(val);
}

bool data_info::get_isMe()
{
//    qDebug()<<m_isMe;
    return m_isMe;
}

void data_info::set_isMe(bool val)
{
    m_isMe=val;
}

void data_info::send_caption_signal_event(int hint)
{
    emit caption_signal(hint);
}

int data_info::getPolling_hint() const
{
    return m_polling_hint;
}

void data_info::setPolling_hint(int polling_hint)
{
    m_polling_hint = polling_hint;
}



void data_info::append_signal_source(QSharedPointer<data_info> &val)
{
    m_list_signal_source.append(val);
}

void data_info::append_signal_source(QList<QSharedPointer<data_info> > &val)
{
    m_list_signal_source = val;
}

void data_info::append_signal_mode(QSharedPointer<data_info> &val)
{
    m_list_signal_mode.append(val);
}

void data_info::append_signal_mode(QList<QSharedPointer<data_info> > &val)
{
    m_list_signal_mode = val;
}

void data_info::append_device(QSharedPointer<data_info> &val)
{
    m_list_device.append(val);
}

void data_info::append_device(QList<QSharedPointer<data_info> > &val)
{
    m_list_device = val;
}

void data_info::append_device_in_e(QSharedPointer<data_info> &val)
{
    if(val.isNull()){
           return;
       }
       bool s_insert = false;
       for (int j=0;j<m_list_device_in_e.count();j++) {
           QSharedPointer<data_info> ii = m_list_device_in_e.at(j);
           if(ii.isNull()){
               continue;
           }
           if(val->get_device_type() == ii->get_device_type()){
               m_list_device_in_e.insert(j, val);
               s_insert = true;
               break;
           }
       }
       if(!s_insert){
           m_list_device_in_e.insert(m_list_device_in_e.count(), val);
       }
   //    m_list_device_in_e.append(val);
}

void data_info::append_device_in_e(QList<QSharedPointer<data_info> > &val)
{
    m_list_device_in_e=val;
}

QList<QSharedPointer<data_info> > data_info::get_list_device_in_e()
{
    return m_list_device_in_e;
}

QList<QSharedPointer<data_info> > data_info::get_list_device()
{
    return m_list_device;
}

QList<QSharedPointer<data_info> > data_info::get_list_signal_source()
{
    return m_list_signal_source;
}

QList<QSharedPointer<data_info> > data_info::get_list_signal_mode()
{
    return m_list_signal_mode;
}

QList<QSharedPointer<data_info> > data_info::get_list_signal_source_in_tv_wall()
{
    return m_list_signal_source_in_tv_wall;
}

int data_info::get_phy_wall_row()
{
    return m_phy_wall_row;
}

void data_info::set_phy_wall_row(int val)
{
    m_phy_wall_row = val;
    emit phy_wall_row_changed();
}

int data_info::get_phy_wall_column()
{
    return m_phy_wall_column;
}

void data_info::set_phy_wall_column(int val)
{
    m_phy_wall_column = val;
    emit phy_wall_column_changed();
}

int data_info::get_logical_wall_row()
{
    return m_logical_wall_row;
}

void data_info::set_logical_wall_row(int val)
{
    m_logical_wall_row = val;
    emit logical_wall_row_changed();
}

int data_info::get_logical_wall_column()
{
    return m_logical_wall_column;
}

void data_info::set_logical_wall_column(int val)
{
    m_logical_wall_column = val;
    emit logical_wall_column_changed();
}

bool data_info::get_device_air_slider()
{
    return m_device_air_slider;
}

void data_info::set_device_air_slider(bool val)
{
    m_device_air_slider = val;
}

bool data_info::get_device_air_switch()
{
    return m_device_air_switch;
}

void data_info::set_device_air_switch(bool val)
{
    m_device_air_switch = val;
}

QString data_info::get_device_ctrl_top()
{
    return m_device_ctrl_top;
}

void data_info::set_device_ctrl_top(QString val)
{
    m_device_ctrl_top = val;
}

QString data_info::get_device_ctrl_hor_left()
{
    return m_device_ctrl_hor_left;
}

void data_info::set_device_ctrl_hor_left(QString val)
{
    m_device_ctrl_hor_left = val;
}

QString data_info::get_device_ctrl_hor_center()
{
    return m_device_ctrl_hor_center;
}

void data_info::set_device_ctrl_hor_center(QString val)
{
    m_device_ctrl_hor_center = val;
}

QString data_info::get_device_ctrl_hor_right()
{
    return m_device_ctrl_hor_right;
}

void data_info::set_device_ctrl_hor_right(QString val)
{
    m_device_ctrl_hor_right = val;
}

QString data_info::get_ctrl_progress()
{
    return m_ctrl_progress;
}

void data_info::set_ctrl_progress(QString val)
{
    m_ctrl_progress = val;
}

int data_info::get_device_type()
{
    return m_device_type;
}

QString data_info::get_ico_path(const QString &path)
{
    QDir appdir = directoryOf(qApp->applicationDirPath());
    appdir.cdUp();
    QString icoFile = appdir.absoluteFilePath(QString("%1").arg(path));
    if(QFile::exists(icoFile)){
        return QString("file:/%1").arg(icoFile);
    }
    return QString(":/%1").arg(path);
}

void data_info::set_device_type(int val)
{
    m_device_type = val;
    if(val == 0){
        m_device_air_slider = true;
    }
    else if(val == 1){
        m_device_air_switch = true;
    }
}

QString data_info::get_device_type_str()
{
    return QString("%1").arg(m_device_type);
}

double data_info::get_display_width()
{
    return m_display_width;
}

void data_info::set_display_width(double val)
{
    m_display_width = val;
}

double data_info::get_display_height()
{
    return m_display_height;
}

void data_info::set_display_height(double val)
{
    m_display_height = val;
}

double data_info::get_display_x()
{
    return m_display_x;
}

void data_info::set_display_x(double val)
{
    m_display_x = val;
}

double data_info::get_display_y()
{
    return m_display_y;
}

void data_info::set_display_y(double val)
{
    m_display_y = val;
}

double data_info::get_display_width_old()
{
    return m_display_width_old;
}

void data_info::set_display_width_old(double val)
{
    m_display_width_old = val;
}

double data_info::get_display_height_old()
{
    return m_display_height_old;
}

void data_info::set_display_height_old(double val)
{
    m_display_height_old = val;
}

double data_info::get_display_x_old()
{
    return m_display_x_old;
}

void data_info::set_display_x_old(double val)
{
    m_display_x_old = val;
}

double data_info::get_display_y_old()
{
    return m_display_y_old;
}

void data_info::set_display_y_old(double val)
{
    m_display_y_old = val;
}

int data_info::get_display_column()
{
    return m_display_column;
}

void data_info::set_display_column(int val)
{
    m_display_column = val;
}

int data_info::get_display_column_num()
{
    return m_display_column_num;
}

void data_info::set_display_column_num(int val)
{
    m_display_column_num = val;
}

int data_info::get_display_row()
{
    return m_display_row;
}

void data_info::set_display_row(int val)
{
    m_display_row = val;
}

int data_info::get_display_row_num()
{
    return m_display_row_num;
}

void data_info::set_display_row_num(int val)
{
    m_display_row_num = val;
}

void data_info::set_echo_ip(QList<QString> val)
{
    m_list_echo_ip = val;
}

QList<QString> data_info::get_echo_ip()
{
    return m_list_echo_ip;
}

QString data_info::get_echo_ip_by_index(int index)
{
    if(index < 0 || index >= m_list_echo_ip.count()){
        return QString();
    }
    return m_list_echo_ip.value(index);
}

int data_info::get_wall_width()
{
    return m_wall_width;
}

void data_info::set_wall_width(int val)
{
    m_wall_width = val;
}

int data_info::get_wall_height()
{
    return m_wall_height;
}

void data_info::set_wall_height(int val)
{
    m_wall_height = val;
}

void data_info::set_num_grid(int val)
{
    int tmp = (int)sqrt(val);
    set_logical_wall_row(tmp);
    set_logical_wall_column(tmp);
}

bool data_info::get_device_status()
{
    return m_device_status;
}

void data_info::set_device_status(bool val)
{
    m_device_status = val;
    emit status_chanaged(val); //不知道 传到哪里
}

QString data_info::get_ip_address()
{
    return m_ip_address;
}

void data_info::set_ip_address(QString val)
{
    m_ip_address = val;
}

QString data_info::get_local_ip_address()
{
     return m_local_ip_address;
}

void data_info::set_local_ip_address(QString val)
{
     m_local_ip_address = val;
}

QString data_info::get_user_name()
{
    return m_user_name;
}

void data_info::set_user_name(QString val)
{
    m_user_name = val;
}

QString data_info::get_group_name()
{
    return m_group_name;
}

void data_info::set_group_name(QString val)
{
    m_group_name = val;
}

unsigned char * data_info::get_signal_source_cmd()
{
    return m_signal_source_cmd;
}

void data_info::set_signal_source_cmd(const unsigned char *val)
{
    memcpy(m_signal_source_cmd, val, sizeof(m_signal_source_cmd));
}

void data_info::sort_signal_source(QList<QSharedPointer<data_info> > &list)
{
    for(int i=0; i < list.count() - 1; i++){
        QSharedPointer<data_info> d_i = list.at(i);
        for(int j=0; j<list.count(); j++){
            QSharedPointer<data_info> d_j = list.at(j);
            if(d_i->get_name() > d_j->get_name()){
                list[i] = d_j;
                list[j] = d_i;
            }
        }
        if(d_i->get_device_type() != 0x07){
            continue;
        }
        d_i->sort_signal_source();
    }
}

bool data_info::getIs_led() const
{
    return m_is_led;
}

void data_info::setIs_led(bool is_led)
{
    m_is_led = is_led;
}

QList<QSharedPointer<data_info> > data_info::headler_device()
{

    QList<int > list3;
    QList<QSharedPointer<data_info> > list;
    foreach(QSharedPointer<data_info> info, m_list_device){
        if(info.isNull()){
            continue;
        }
        int type=info->get_device_type();
        if(!list3.contains(type)){
            list3.append(type);
        }
    }

    foreach(QSharedPointer<data_info> info, m_list_device_in_e){
        if(info.isNull()){
            continue;
        }
        int type=info->get_device_type();
        if(list3.contains(type)){
            list.append(info);
        }
   }
    return list;
}

void data_info::send_get_wall_signal_event()
{
    emit get_wall_signal();
}

void data_info::send_write_to_serial_signal_event(int o, int a)
{
    emit write_to_serial_signal(o,a);
}

void data_info::send_close_serial_signal_event()
{
    emit close_serial_signal();
}

int data_info::get_com() const
{
    return m_com;
}

void data_info::set_com(int com)
{
    m_com = com;
}

QHash<QString, QSharedPointer<cmd_info> > data_info::get_udp_cmds()
{
    return m_udp_cmds;
}

void data_info::add_udp_cmds(QSharedPointer<cmd_info> val)
{
    m_udp_cmds.insert(val->get_cmd(), val);
}

QSharedPointer<cmd_info> data_info::get_udp_cmds_info(QString val)
{
//    qDebug()<<val;
    if(m_udp_cmds.value(val))
      return m_udp_cmds.value(val);
    else {
          qDebug()<<"cmd error";
        return temp;

    }
}

QString data_info::get_cmd_from_msg(QString val)
{
    QString find_cmd="";
    QList<QSharedPointer<cmd_info>> temp_list=m_udp_cmds.values();
    foreach(QSharedPointer<cmd_info> info, temp_list){
     if(info->get_suc()==val)
         find_cmd=info->get_cmd();
   }
    return  find_cmd;
}

bool data_info::is_in_cmd(QString val)
{
    bool is_find=false;
    QList<QSharedPointer<cmd_info>> temp_list=m_udp_cmds.values();
    foreach(QSharedPointer<cmd_info> info, temp_list){
     if(info->get_suc()==val)
         is_find=true;
   }
    return  is_find;
}

int data_info::getPortSend() const
{
    return m_portSend;
}

void data_info::setPortSend(int portSend)
{
    m_portSend = portSend;
}

int data_info::getPortRcv() const
{
    return m_portRcv;
}

void data_info::setPortRcv(int portRcv)
{
    m_portRcv = portRcv;
}

QList<QSharedPointer<data_info> > data_info::getList_akong() const
{
    return m_list_akong;
}

void data_info::setList_akong(const QList<QSharedPointer<data_info> > &list_akong)
{
    m_list_akong = list_akong;
}
void data_info::sort_signal_source()
{
    //    qDebugx(qtr("开始分组排序"));
    for(int i=0; i < m_list_signal_source.count() - 1; i++){
        for(int j=i+1; j<m_list_signal_source.count(); j++){
            QSharedPointer<data_info> d_i = m_list_signal_source.at(i);
            QSharedPointer<data_info> d_j = m_list_signal_source.at(j);
//            qDebugxxxxx(qtr("比较"), i, j, d_i->get_name(), d_j->get_name());
            if(d_i->get_name() > d_j->get_name()){
//                qDebugxxx(qtr("交换"), d_i->get_name(), d_j->get_name());
                m_list_signal_source[i] = d_j;
                m_list_signal_source[j] = d_i;
            }
        }
    }
    for(int i=0; i < m_list_signal_source.count(); i++){
        QSharedPointer<data_info> group = m_list_signal_source.at(i);
        if(group->get_device_type() == 0x07){
//            qDebugxx(qtr("子分组排序"), group->get_name());
            group->sort_signal_source();
        }
    }
//     qDebug()<<m_list_signal_source.length();
//    qDebugx(qtr("结束分组排序"));
}



QString data_info::get_user_pwd()
{
    return m_user_pwd;
}

void data_info::set_user_pwd(QString val)
{
    m_user_pwd = val;
}

QString data_info::get_id()
{
    return m_id;
}

void data_info::set_id(QString val)
{
    m_id = val;
}

unsigned int data_info::get_win_id()
{
    return m_win_id;
}

void data_info::set_win_id( unsigned int val)
{
    m_win_id = val;
}

int data_info::get_port()
{
    return m_port;
}

void data_info::set_port(int val)
{
    m_port = val;
}

int data_info::get_sort()
{
    return m_sort;
}

void data_info::set_sort(int val)
{
    m_sort = val;
}



int data_info::get_channel_no()
{
    return m_channel_no;
}

void data_info::set_channel_no(int val)
{
    m_channel_no = val;
}

int data_info::get_volume()
{
    return m_volume;
}

void data_info::set_volume(int vol)
{
    m_volume=vol;
}

int data_info::get_audio_id()
{
    return m_audio_id;
}

void data_info::set_audio_id(int id)
{
    m_audio_id=id;
}

QString data_info::get_cmd_type()
{
     return m_cmd_type;
}

void data_info::set_cmd_type(QString val)
{
    m_cmd_type=val;
}
int data_info::get_audio_channel()
{
    return m_audio_channel;
}

void data_info::set_audio_channel(int id)
{
    m_audio_channel=id;
}
void data_info::send_signal_source_changed_event()
{
    emit signal_source_changed(get_index());
}

void data_info::send_signal_source_in_wall_changed_event()
{
    emit signal_source_in_wall_changed(get_index());
}

void data_info::send_image_incoming_event(QImage img)
{
    if(img.isNull()){
        return;
    }
    emit image_incoming_signal(get_id(), img);
}

void data_info::cloneEx(QSharedPointer<data_info> &val)
{
    set_id(val->get_id());
    set_name(val->get_name());
    set_device_type(val->get_device_type());
    set_port(val->get_port());
    set_index(val->get_index());
    set_ip_address(val->get_ip_address());
}

int data_info::get_channel_flag(int index)
{
    QVariantMap map = m_music_slides.value(index).toMap();
    return map.value("flag").toInt();
}

int data_info::get_channel_type(int index)
{
    QVariantMap map = m_music_slides.value(index).toMap();
    return map.value("dmMusicIOType") == "IN"?0x01:0x02;
}


void data_info::clone(QSharedPointer<data_info> &val)
{
    set_id(val->get_id());
    set_name(val->get_name());
    set_device_type(val->get_device_type());
    set_port(val->get_port());
    set_index(val->get_index());
    set_ip_address(val->get_ip_address());
    set_display_x(val->get_display_x());
    set_display_y(val->get_display_y());
    set_display_width(val->get_display_width());
    set_display_height(val->get_display_height());
    set_display_height_old(val->get_display_height_old());
    set_display_width_old(val->get_display_width_old());
    set_display_x_old(val->get_display_x_old());
    set_display_y_old(val->get_display_y_old());
    set_wall_height(val->get_wall_height());
    set_wall_width(val->get_wall_width());
}

void data_info::set_old_data_info(QSharedPointer<data_info> val)
{
    m_old_data_info = val;
}

QSharedPointer<data_info> data_info::get_old_data_info()
{
    return m_old_data_info;
}

void data_info::remove_mode(QSharedPointer<data_info> val)
{
    m_list_signal_mode.removeOne(val);
}

void data_info::remove_signal_source(QSharedPointer<data_info> val)
{
    m_list_signal_source.removeOne(val);
}

void data_info::send_add_signal_source_to_wall_signal_event(QSharedPointer<data_info> val)
{
    emit add_signal_source_to_wall_signal(val);
}

void data_info::send_update_signal_source_to_wall_signal_event(QSharedPointer<data_info> val)
{
    if(val.isNull()){
        return;
    }
    emit update_signal_source_to_wall_signal(val);
}

void data_info::send_update_signal_source_to_wall_signal_by_old_event(QSharedPointer<data_info> val)
{
    if(m_old_data_info.isNull()){
        qDebugx(qtr("未设置原始数据"));
        return;
    }
    //m_old_data_info是指c区的信号源 即b区的信号
    m_old_data_info->send_update_signal_source_to_wall_signal_event(val);
}

void data_info::send_remove_signal_source_to_wall_signal_by_old_event(QSharedPointer<data_info> val)
{
    if(m_old_data_info.isNull()){
        qDebugx(qtr("未设置原始数据"));
        return;
    }
    m_old_data_info->send_remove_signal_source_to_wall_signal_event(val);
}

void data_info::send_remove_signal_source_to_wall_signal_event(QSharedPointer<data_info> val)
{
    if(val.isNull()){
        return;
    }
    emit remove_signal_source_to_wall_signal(val);
}

void data_info::send_clear_window_signal_event()
{
    emit clear_window_signal();
}

void data_info::send_device_ctrl_signal_event(const QString &cmd)
{
    emit device_ctrl_signal(cmd);
}

void data_info::send_device_ctrl_signal_event(const QString &cmd, int channel)
{
//    qDebug() << "send_device_ctrl_signal_event:" << cmd;
    emit device_ctrl_param_1_signal(cmd, channel);
}

void data_info::send_device_ctrl_signal_event(const QString &cmd, int channel, int param1)
{
    emit device_ctrl_param_2_signal(cmd, channel, param1);
}

void data_info::send_device_ctrl_signal_event(int operate, int action ,int multiple,int win_id)
{
    emit device_ctrl_param_3_signal(operate, action,multiple,win_id);
}



void data_info::send_remove_signal_source_signal_event(QSharedPointer<data_info> val)
{
    if(val.isNull()){
        return;
    }
    emit remove_signal_source_signal(val);
}

void data_info::send_add_signal_source_signal_event(QSharedPointer<data_info> val)
{
    if(val.isNull()){
        return;
    }
    emit add_signal_source_signal(val);
}


void data_info::start()
{

}

void data_info::stop()
{

}

QVariantList data_info::get_music_buttons()
{
//    QVariantMap map;
//    map.insert("dmMusicName", "for teset");
//    map.insert("dmMusicIco", "/images/add-active.png");
//    QVariantList list;
//    list.append(map);
    return m_music_buttons;
}

void data_info::set_music_buttons(const QVariantList &val)
{
    m_music_buttons = val;
}


QVariantList data_info::get_group_childs()
{
//    qDebug() << "get_group_childs" << get_name() << m_list_signal_source.count();
    QVariantList list;

    for(int i=0;i<m_list_signal_source.count(); i++) {
        QSharedPointer<data_info> info = m_list_signal_source.at(i);
        if(info.isNull()){
            continue;
        }
        QVariantMap map;
        map.insert("dmName", info->get_name());
        map.insert("dmVideoId", info->get_id());
        map.insert("dmDeviceType", info->get_device_type());
        map.insert("dmIndex", info->get_index());
        map.insert("dmGroupImageActive", "/images/group/group-active.png");
        map.insert("dmGroupImageIdle", "/images/group/group-idle.png");
//        qDebugxxx(qtr("获取设备"), info->get_name(), info->get_index());
        if(info->get_device_type() == 0x07){
            qDebug() << "find childs";
            if(info->get_list_signal_source().count() > 0){
                QVariantList list = info->get_group_childs(); //分组中的信号列表
                map.insert("dmGroupChilds", list);
                map.insert("dmCount", list.count());

            }
            else{
                map.insert("dmGroupChilds", QVariantList());
                map.insert("dmCount", 1);
            }
        }
        else{
            map.insert("dmGroupChilds", QVariantList());
            map.insert("dmCount", 1);
        }
        list.append(map);
    }

    return list;
}

QVariantList data_info::get_music_sliders()
{
//    QVariantMap map;
//    map.insert("dmMusicName", "for teset");
//    map.insert("dmMusicIco", "/images/add-active.png");
//    map.insert("dmMusicImage", "/images/add-active.png");
//    QVariantList list;
//    list.append(map);
    return m_music_slides;
}

void data_info::set_music_sliders(const QVariantList &val)
{
    m_music_slides = val;
}

void data_info::get_signal_source()
{

}

void data_info::get_scence()
{

}

QSharedPointer<data_info> data_info::get_signal_source_by_id(const QString &id, const QList<QSharedPointer<data_info> > &list)
{
    foreach(QSharedPointer<data_info> info, list){
        if(info->get_id() == id){
            return info;
        }
        if(info->get_device_type() != 0x07){
            continue;
        }
        QList<QSharedPointer<data_info> > list = info->get_list_signal_source();
        if(list.count() == 0){
            continue;
        }
        QSharedPointer<data_info> data = get_signal_source_by_id(id, list);
        if(data.isNull()){
            continue;
        }
        return data;
    }
    return QSharedPointer<data_info>();
}

void data_info::print_signal_source()
{
    qDebugx(qtr("打印一个分组开始"));
    foreach(QSharedPointer<data_info> info, m_list_signal_source){
        qDebugxxxxx(qtr("打印信号源"), m_list_signal_source.count(), info->get_id(), info->get_device_type(), info->get_name());
        if(info->get_device_type() == 0x07){
            info->print_signal_source();
        }
    }
    qDebugx(qtr("打印一个分组结束"));
}

void data_info::reset_signal_source_status()
{
    foreach(QSharedPointer<data_info> info, m_list_signal_source){
        if(info.isNull()){
            continue;
        }
        info->set_device_status(false);
        if(info->get_device_type() != 0x07){
            continue;
        }
        info->reset_signal_source_status();
    }
}

void data_info::send_win_level_changed_event(QSharedPointer<data_info> val)
{
    if(val.isNull()){
        return;
    }
    emit win_level_changed(val);
}

void data_info::send_start_image_request_event()
{
    emit start_image_request_signal();
}

QSharedPointer<data_info> data_info::get_signal_source_by_id(const QString &id)
{
    foreach(QSharedPointer<data_info> info, m_list_signal_source){
//        qDebugxxxxxx(qtr("查询信号源"), m_list_signal_source.count(), id, info->get_id(), info->get_device_type(), info->get_name());
        if(info->get_id() == id){
//            qDebugxxxxx(qtr("找到信号源"), id, info->get_id(), info->get_device_type(), info->get_name());
            return info;
        }
        if(info->get_device_type() != 0x07){
            continue;
        }
        QSharedPointer<data_info> data = info->get_signal_source_by_id(id);
        if(data.isNull()){
            continue;
        }
        return data;
    }
    return QSharedPointer<data_info>();
}

bool data_info::exists_signal_source(const QString &id)
{
    foreach(QSharedPointer<data_info> info, m_list_signal_source){
        if(info->get_id() == id){
            return true;
        }
    }
    return false;
}

void data_info::add_signal_source(QSharedPointer<data_info> val)
{
    if(val.isNull() || m_list_signal_source.contains(val)){
        return;
    }

    QString id = val->get_id();
    if(exists_signal_source(id)){
        return;
    }
    m_list_signal_source.append(val);
}

void data_info::clear_signal_source_in_tv_wall()
{
    m_list_signal_source_in_tv_wall.clear();
}

void data_info::add_signal_source_in_tv_wall(QSharedPointer<data_info> val)
{
    m_list_signal_source_in_tv_wall.append(val);
}

void data_info::add_signal_source_in_tv_wall(QList<QSharedPointer<data_info> > val)
{
    clear_signal_source_in_tv_wall();
    foreach(QSharedPointer<data_info> info, val){
        if(info.isNull()){
            continue;
        }
        QSharedPointer<data_info> data(new data_info);
        data->clone(info);
        data->set_old_data_info(info->get_old_data_info());
        m_list_signal_source_in_tv_wall.append(data);
    }

}
void data_info::remove_signal_source_in_tv_wall(int index)
{
    if(index < 0 || index >= m_list_signal_source_in_tv_wall.count()) {
        return;
    }


    m_list_signal_source_in_tv_wall.removeAt(index);
}

void data_info::send_fresh_win_signal_event()
{
    emit fresh_win_signal();
}

void data_info::send_polling_changed_event()
{
    emit polling_changed(get_index());
}

void data_info::send_mode_changed_event()
{
    emit mode_changed();
}

bool data_info::getIsModeChanged()
{
    --m_isModeChanged_hint;
    if(m_isModeChanged_hint<=0)

       return false;
    else {
       return true;
    }

}

void data_info::setIsModeChanged(int  hint)
{
    m_isModeChanged_hint = hint;
}

void data_info::send_changed_mode_signal_event(QString index)
{
//    qDebug()<<11;
    emit changed_mode_signal(index);
}

void data_info::send_changed_mode_signal_event(int index)
{
    emit changed_mode_signal_int(index);
}

void data_info::send_add_mode_signal_event(QSharedPointer<data_info> val)
{
    emit add_mode_signal(val);
}
void data_info::send_edit_mode_signal_event(QSharedPointer<data_info> val)
{
    emit edit_mode_signal(val);
}

void data_info::send_ptz_signal_event(int o, int a ,int win_id)
{
    emit ptz_signal(o,a,win_id);
}

void data_info::send_open_audio_signal_event(QString sig_id, int Channel, int vol)
{
    emit open_audio_signal(sig_id,Channel,vol);
}
void data_info::send_close_audio_signal_event(int audio_id)
{
    emit close_audio_signal(audio_id);
}

void data_info::send_set_audio_signal_event(int audio_id, int vol)
{
    emit set_audio_signal(audio_id,vol);
}

void data_info::add_audio_list(QSharedPointer<data_info> val)
{
       m_list_audio.append(val);
}

QList<QSharedPointer<data_info> >  data_info::get_list_audio()
{
    return m_list_audio;
}
void data_info::send_remove_mode_signal_event(QString id)
{
    emit remove_mode_signal(id);
}

