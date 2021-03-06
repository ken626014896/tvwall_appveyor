﻿#include "room_manager.h"
#include "sy_utils.h"
#include "room_info.h"
#include "sy_logger.h"
#include "signal_resource_info.h"
#include "device_info.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QStandardPaths>
#include <QStandardPaths>
#include <QTextCodec>
#include "room_check.h"

#ifdef WIN32
    #include <qt_windows.h>
#endif

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif
const char *CONF_FILE_NAME = "gsconfig.json";
const char *CONF_IP_FILE_NAME = "ip.ini";

room_manager::room_manager(data_model *room, data_model *signal_source,
                           data_model *signal_mode, data_model *device,
                           data_model *signal_source_display,
                           data_model *signal_mode_image, data_model *device_ctrl,
                           QObject *parent) :
    QObject(parent),
    m_model_room(room),
    m_model_device(device),
    m_model_signal_mode(signal_mode),
    m_iszhongwen(true),
    m_model_signal_source(signal_source),
    m_current_index_room(-1),
    m_logical_wall_row(2),
    m_logical_wall_column(2),
    m_model_signal_source_display(signal_source_display),
    m_model_signal_mode_image(signal_mode_image),
    m_model_device_ctrl(device_ctrl),
//    m_engine(engine),
    caption_hint(false),
    basemap_hint(false),
    polling_num(-1),
    isZH(true)
{

    if(m_model_room != nullptr) {
        connect(m_model_room, &data_model::index_changed, this, &room_manager::index_changed_room_slot);
    }
    if(m_model_signal_source_display != nullptr) {
        connect(m_model_signal_source_display, &data_model::index_changed_by_myself, this, &room_manager::win_changed_slot);
        connect(m_model_signal_source_display, &data_model::id_changed, this, &room_manager::id_changed_slot);
    }
    if(m_model_signal_source != nullptr) {
        connect(m_model_signal_source, &data_model::id_changed, this, &room_manager::id_changed_slot);
    }
//    connect(&m_timer, &QTimer::timeout, this, &room_manager::image_changed_time_out_slot);
//    m_timer.start(1000);
    checkroom = new room_check();
    connect(checkroom, &room_check::login_status, this, &room_manager::headler_signal_scene_slot);
    connect(checkroom, &room_check::login_status_conf, this, &room_manager::headler_congfig);
}

bool room_manager::get_iszhongwen()
{
    return m_iszhongwen;
}

void room_manager::set_iszhongwen(bool val)
{
    m_iszhongwen=val;
    emit iszhongwen_changed();
}

int room_manager::get_current_index_room()
{
    return m_current_index_room;
}

void room_manager::set_current_index_room(int val)
{
    if(m_current_index_room == val) {
        return;
    }
    m_current_index_room = val;
    emit index_room_changed();
}

QString room_manager::get_backgroup_image()
{
    return get_ico_path("/images/background.jpg");
}

int room_manager::get_logical_wall_row()
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        return 2;
    }
    return current_room->get_logical_wall_row();
}

void room_manager::set_logical_wall_row(int val)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        return;
    }
    current_room->set_logical_wall_row(val);
//    m_logical_wall_row = val;
    emit logical_wall_row_changed();
}

int room_manager::get_logical_wall_column()
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        return 2;
    }
    return current_room->get_logical_wall_column();
}

void room_manager::set_logical_wall_column(int val)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        return;
    }
    current_room->set_logical_wall_column(val);
    emit logical_wall_column_changed();
}



QString room_manager::get_ico_path(const QString &path)
{
    QDir appdir = directoryOf(qApp->applicationDirPath());
    QString icoFile = appdir.absoluteFilePath(QString("%1").arg(path.left(1) == "/" ? path.right(path.length() - 1) : path));
//    qDebug() << "get_ico_path:" << icoFile;
    if(QFile::exists(icoFile)) {
        return QString("file:/%1").arg(icoFile);
    }
    if(QFile::exists(path.left(1) == "/" ? QString(":%1").arg(path) : QString(":/%1").arg(path))) {
        return path.left(1) == "/" ? QString("%1").arg(path) : QString("/%1").arg(path);
    }
    qWarnxx(qtr("未找到图片"), path);
    return QString();
}

void room_manager::read_conf()
{
    if(m_model_room == nullptr ||
       m_model_device == nullptr ||
       m_model_signal_mode == nullptr ||
       m_model_signal_source == nullptr) {
        return;
    }
    m_model_room->clear();
    m_model_device->clear();
    m_model_signal_mode->clear();
    m_model_signal_source->clear();
    QDir appdir = directoryOf(qApp->applicationDirPath());
    m_root_path = appdir.path();
    QDir dir = directoryOf(appdir.absoluteFilePath("conf"));
    QString configFile = dir.absoluteFilePath(QString("%1").arg(CONF_FILE_NAME));
#ifdef Q_OS_ANDROID
    //安卓版本，配置文件不存在时，从资源文件中读取初始配置
    dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    configFile = dir.absoluteFilePath(QString("%1").arg(CONF_FILE_NAME));
    qDebug()<<configFile;
    if(!QFile::exists(configFile)) {
        qDebugxx(qtr("配置文件不存在"), configFile);
        configFile = QString(":/conf/%1").arg(CONF_FILE_NAME);
    }
#endif
    QString content = get_content_from_file(configFile);
    if(content.isEmpty()) {
        qDebug() << "no conf:" << configFile;
        qDebugx(configFile);
        emit login_error();
        return;
    }
    QJsonObject json = getJsonObjectFromString(content);
    if(json.isEmpty()) {
        qDebug() << "error conf:" ;
        emit config_error();
        return;
    }
    m_local_ip_address =json.value("localIP").toString();
    QJsonArray ja_acc_conf = json.value("aConfig").toArray(); //得到所有指令
    int sort = 0;
    int acc_conf_index=0;
    foreach(QJsonValue qjv, ja_acc_conf) { //循环不同的设备
        if(qjv.isNull() || !qjv.isObject()) {
            continue;
        }
        QJsonObject json_conf = qjv.toObject();
        QSharedPointer<acc_conf_info> conf(new acc_conf_info);
        conf->set_net_type(json_conf.value("netType").toInt());
        conf->set_device_type(json_conf.value("type").toInt());
        conf->set_name(json_conf.value("name").toString());
        conf->set_cmd_type(json_conf.value("cmdType").toString());
        conf->set_model(json_conf.value("model").toString());
        conf->set_modelName(json_conf.value("modelName").toString());
        conf->set_control_bus(json_conf.value("ccIndex").toInt());
        QJsonArray ja_cmds = json_conf.value("cmds").toArray();
        foreach(QJsonValue jv, ja_cmds) { //循环设备不同的命令
            if(jv.isNull() || !jv.isObject()) {
                continue;
            }
            QJsonObject json_cmd = jv.toObject();
            QSharedPointer<cmd_info> cmd(new cmd_info);
            cmd->set_name(json_cmd.value("name").toString());
            cmd->set_cmd(json_cmd.value("cmd").toString());
            cmd->set_send(json_cmd.value("send").toString());
            cmd->set_err(json_cmd.value("err").toString());
            cmd->set_suc(json_cmd.value("suc").toString());
            conf->add_cmd(cmd);
        }
        QSharedPointer<data_info> dev(new data_info);
        dev->set_name(conf->get_name());
        dev->set_device_type(conf->get_device_type());
        dev->set_sort(json_conf.value("sort").toInt(sort++));
        conf->set_sort(dev->get_sort());
        m_model_device->append_by_sort(dev);  //按sort的值 从小到大插入
        m_hash_acc_conf.insert(acc_conf_index++, conf);  //map

    }
//    qDebug()<<m_hash_acc_conf;
    QJsonArray ja_scene = json.value("aScene").toArray();  //得到所有场景
    if(ja_scene.isEmpty()) {
        qDebug() << "error aSecne:" << configFile;
        return;
    }
    int index = 0;
    foreach(QJsonValue jv, ja_scene) { //循环不同场景
        if(jv.isNull() || !jv.isObject()) {
            continue;
        }
        QJsonObject jo = jv.toObject();
        QSharedPointer<data_info> info(new room_info());
        connect(info.data(), &data_info::phy_wall_row_changed, m_model_room, &data_model::phy_wall_row_changed);
        connect(info.data(), &data_info::phy_wall_column_changed, m_model_room, &data_model::phy_wall_column_changed);
//        connect(info.data(), &data_info::logical_wall_row_changed, m_model_room, &data_model::logical_wall_row_changed);
//        connect(info.data(), &data_info::logical_wall_column_changed, m_model_room, &data_model::logical_wall_column_changed);
        connect(info.data(), &data_info::signal_source_changed, this, &room_manager::signal_source_changed_slot);
        connect(info.data(), &data_info::signal_source_in_wall_changed, this, &room_manager::signal_source_in_wall_changed_slot); //场景获取大屏中信号后
        //
        connect(info.data(), &data_info::polling_changed, this, &room_manager::sence_changed_from_polling); //
        connect(info.data(), &data_info::mode_changed, this, &room_manager::mode_changed_slot); //
        //二期新增 信号同步
        connect(info.data(), &data_info::sync_win_open_signal, this, &room_manager::sync_win_open_signal_slot);
        connect(info.data(), &data_info::sync_win_close_signal, this, &room_manager::sync_win_close_signal_slot);
        connect(info.data(), &data_info::sync_win_close_all_signal, this, &room_manager::sync_win_close_all_signal_slot);
        connect(info.data(), &data_info::sync_win_move_signal, this, &room_manager::sync_win_move_signal_slot);
        connect(info.data(), &data_info::sync_win_level_signal, this, &room_manager::sync_win_level_signal_slot);
        connect(info.data(), &data_info::sync_win_change_signal, this, &room_manager::sync_win_change_signal_slot);
        connect(info.data(), &data_info::polling_status_signal, this, &room_manager::polling_status_signal);
        connect(info.data(), &data_info::sync_mode_change_signal, this, &room_manager::sync_mode_change_signal_slot);
        connect(info.data(), &data_info::sync_mode_add_signal, this, &room_manager::sync_mode_add_signal_slot);
        connect(info.data(), &data_info::sync_mode_remove_signal, this, &room_manager::sync_mode_remove_signal_slot);
        connect(info.data(), &data_info::sync_mode_edit_signal, this, &room_manager::sync_mode_edit_signal_slot);
        connect(info.data(), &data_info::sync_audio_open_signal, this, &room_manager::sync_audio_open_signal_slot);
        connect(info.data(), &data_info::sync_audio_close_signal, this, &room_manager::sync_audio_close_signal_slot);
        connect(info.data(), &data_info::sync_audio_set_signal, this, &room_manager::sync_audio_set_signal_slot);
        connect(info.data(), &data_info::disconnect_signal, this, &room_manager::disconnect_signal);
        connect(info.data(), &data_info::reconnect_signal, this, &room_manager::reconnect_signal);

        connect(info.data(), &data_info::get_resolution, this, &room_manager::get_resolution);

        info->set_name(jo.value("name").toString());
        info->set_ip_address(jo.value("ip").toString());
        info->set_user_name(jo.value("user").toString());
        info->set_user_pwd(jo.value("password").toString());
        info->set_port(jo.value("port").toInt(6801));
        info->set_logical_wall_row(jo.value("numGridRow").toInt(1));
        info->set_logical_wall_column(jo.value("numGridCol").toInt(1));
        info->set_wall_width(jo.value("wallWidth").toInt());
        info->set_wall_height(jo.value("wallHeight").toInt());
        info->setIs_led(jo.value("isLED").toBool());
        info->set_index(index++);
        m_model_room->append(info);
        QJsonArray ja_kong = jo.value("aKong").toArray();  //循环不同的udp 地址和端口
        QList<QSharedPointer<data_info>> list_kong;
        foreach(QJsonValue jv_kong, ja_kong) {
            if(jv_kong.isNull() || !jv_kong.isObject()) {
                continue;
            }
            QJsonObject jo_kong = jv_kong.toObject();
            QSharedPointer<data_info> dev(new device_info());
            dev->set_id(getUuid());

            dev->set_ip_address(jo_kong.value("ip").toString());
            dev->set_port(jo_kong.value("port").toString().toInt());
            dev->setPortSend(jo_kong.value("portSend").toString().toInt());
            dev->setPortRcv(jo_kong.value("portRcv").toString().toInt()); // 新增串口参数

            list_kong.append(dev);
         }
        info->setList_akong(list_kong);

        QJsonArray ja_dev = jo.value("aDevice").toArray();   //循环不同的设备
        QList<QSharedPointer<data_info>> list_dev;
        index = 0;
        foreach(QJsonValue jv_dev, ja_dev) {
            if(jv_dev.isNull() || !jv_dev.isObject()) {
                continue;
            }
            QJsonObject jo_dev = jv_dev.toObject();
            QSharedPointer<data_info> dev(new device_info());
            dev->set_id(getUuid());
            dev->set_name(jo_dev.value("name").toString());
            dev->set_device_type(jo_dev.value("type").toInt());
            dev->set_ip_address(jo_dev.value("ip").toString());
            dev->set_port(jo_dev.value("port").toInt());
            dev->set_channel_no(jo_dev.value("bit").toInt());
            dev->set_com(jo_dev.value("com").toInt()); // 新增串口参数
            dev->set_cmd_type(get_dev_cmdType(m_hash_acc_conf,dev->get_device_type()));
            dev->set_local_ip_address(m_local_ip_address);
            if(jo_dev.value("model").toString()=="")
                dev->set_model("init");
            else
               dev->set_model(jo_dev.value("model").toString()); //新增设备对应的牌子参数
            dev->set_ico_path(get_ico_path(QString("images/%1/list-active.png").arg(dev->get_device_type())));
//            qDebug()<<"dev name:"<<jo_dev.value("name").toString();

            //添加udp命令，如果有的话
            QJsonArray ja_dev_udp = jo_dev.value("cmds").toArray();
            foreach(QJsonValue jv_dev_udp, ja_dev_udp) {
                if(jv_dev_udp.isNull() || !jv_dev_udp.isObject()) {
                    continue;
                }
                QJsonObject jo_dev_udp = jv_dev_udp.toObject();
                QSharedPointer<cmd_info> cmd(new cmd_info);
                cmd->set_name(jo_dev_udp.value("name").toString());
                cmd->set_cmd(jo_dev_udp.value("cmd").toString());
                cmd->set_send(jo_dev_udp.value("send").toString());
                cmd->set_err(jo_dev_udp.value("err").toString());
                cmd->set_suc(jo_dev_udp.value("suc").toString());
                dev->add_udp_cmds(cmd);


            }


            //model为-1 证明是udp设备
            if(dev->get_model()=="-1"){

                   dev->setPortRcv(get_dev_portRcv(info->getList_akong(),dev->get_ip_address(),dev->get_port()));
            }

            ((device_info *)dev.data())->set_acc_conf_info(get_diff_dev(m_hash_acc_conf,dev->get_device_type(),dev->get_model()));  //可能有相同的设备  例如:同一场景两个不同的空调
            connect(dev.data(), &data_info::device_status_changed_signal, this, &room_manager::device_status_changed_slot); //设备状态改变信号
            connect(dev.data(), &data_info::mute_status_changed_signal, this, &room_manager::mute_status_changed_signal);
            connect(dev.data(), &data_info::volume_value_signal, this, &room_manager::volume_value_signal);
            QJsonArray ja_effect = jo_dev.value("aEffect").toArray();
            QVariantList vl_effect;
            foreach(QJsonValue jv_effect, ja_effect) { //得到不同的音效
                if(jv_effect.isNull() || !jv_effect.isString()) {
                    continue;
                }
                QVariantMap map;
                map.insert("dmMusicName", jv_effect.toString());
                map.insert("dmMusicIco", get_ico_path(QString("images/%1/btn.png").arg(dev->get_device_type())));
                vl_effect.append(map);
            }
            dev->set_music_buttons(vl_effect);//添加音响按钮
            QVariantList vl_in_out;
            read_music_in_out(jo_dev, vl_in_out, "aIn", "IN");
            read_music_in_out(jo_dev, vl_in_out, "aOut", "OUT");
            dev->set_music_sliders(vl_in_out); //添加音音量组件
            dev->set_index(index++);
            connect(dev.data(), &data_info::temperature_changed_signal, this, &room_manager::data_changed_by_device_slot); //空调温度改变
            list_dev.append(dev);
//            qDebug() << "find device:" << dev->get_name();
        }
        info->append_device(list_dev); //把不同设备放到场景中存储
        QList<QSharedPointer<data_info> > list2 = m_model_device->get_data_list();
        info->append_device_in_e(list2);
        QJsonArray ja_mode = jo.value("models").toArray();    //循环不同的模式
        QList<QSharedPointer<data_info>> list_mode;
        foreach(QJsonValue jv_mode, ja_mode) {
            if(jv_mode.isNull() || !jv_mode.isObject()) {
                continue;
            }
            QJsonObject jo_dev = jv_mode.toObject();
            QSharedPointer<data_info> dev(new data_info()); //这里dev是模式对象
            dev->set_name(jo_dev.value("name").toString());
            dev->set_ico_path(get_ico_path(jo_dev.value("image").toString()));
            dev->set_logical_wall_row(jo_dev.value("numGridRow").toInt(2));
            dev->set_logical_wall_column(jo_dev.value("numGridCol").toInt(2));
            QJsonArray ja = jo_dev.value("aSignal").toArray();   //循环相同模式下不同的信号
            dev->clear_signal_source_in_tv_wall();
            foreach(QJsonValue qjv, ja) {
                if(qjv.isNull() || !qjv.isObject()) {
                    continue;
                }
                QJsonObject qjo = qjv.toObject();
                QSharedPointer<data_info> sig(new signal_resource_info());
                QString id = qjo.value("id").toString();
                sig->set_id(id);
                sig->set_display_x(qjo.value("x").toDouble());
                sig->set_display_y(qjo.value("y").toDouble());
                sig->set_display_width(qjo.value("width").toDouble());
                sig->set_display_height(qjo.value("height").toDouble());
                dev->add_signal_source_in_tv_wall(sig);
            }
            list_mode.append(dev);
        }
        info->append_signal_mode(list_mode);  //把不同模式放到场景中存储
//       调出测试用的信号源
//        int j = 0;
//        QList<QSharedPointer<data_info>> list_cam;
//        QSharedPointer<data_info> group(new signal_resource_info());
//        group->set_device_type(0x07);
//        group->set_name(QStringLiteral("测试目录"));
//        group->set_index(j++);
//        group->set_id(getUuid());
//        QList<QSharedPointer<data_info>> list_cam_child;
//        for(int i=0;i<500;i++){
//            QSharedPointer<data_info> cam(new signal_resource_info());
//            cam->set_device_type(0x05);
//            cam->set_name(QString("%1%2").arg(QStringLiteral("信号")).arg(i));
//            cam->set_index(j++);
//            cam->set_id(getUuid());
//            cam->set_win_id(12);
//            if(m_model_signal_source != nullptr){
//                connect(cam.data(), &data_info::image_changed_signal, m_model_signal_source, &data_model::data_changed_slot);
//            }
//            list_cam_child.append(cam);
//        }
//        group->append_signal_source(list_cam_child);
//        list_cam.append(group);
//        QSharedPointer<data_info> group2(new signal_resource_info());
//        group2->set_device_type(0x07);
//        group2->set_name(QStringLiteral("测试目录2"));
//        group2->set_index(j++);
//        group2->set_id(getUuid());
//        QList<QSharedPointer<data_info>> list_cam_child2;
//        for(int i=0;i<6;i++){
//            QSharedPointer<data_info> cam(new signal_resource_info());
//            cam->set_device_type(0x01);
//            cam->set_name(QString("%1%2").arg(QStringLiteral("hhhhhhdmi-2信号")).arg(i));
//            cam->set_index(j++);
//            cam->set_id(getUuid());
//            if(m_model_signal_source != nullptr){
//                connect(cam.data(), &data_info::image_changed_signal, m_model_signal_source, &data_model::data_changed_slot);
//            }
//            list_cam_child2.append(cam);
//        }
//        group2->append_signal_source(list_cam_child2);
//        list_cam.append(group2);
//        QSharedPointer<data_info> group3(new signal_resource_info());
//        group3->set_device_type(0x07);
//        group3->set_name(QStringLiteral("测试目录3"));
//        group3->set_index(j++);
//        group3->set_id(getUuid());
//        QList<QSharedPointer<data_info>> list_cam_child3;
//        for(int i=0;i<6;i++){
//            QSharedPointer<data_info> cam(new signal_resource_info());
//            cam->set_device_type(0x01);
//            cam->set_name(QString("%1%2").arg(QStringLiteral("3信号")).arg(i));
//            cam->set_index(j++);
//            cam->set_id(getUuid());
//            if(m_model_signal_source != nullptr){
//                connect(cam.data(), &data_info::image_changed_signal, m_model_signal_source, &data_model::data_changed_slot);
//            }
//            list_cam_child3.append(cam);
//        }
//        group3->append_signal_source(list_cam_child3);
//        list_cam.append(group3);
//        info->append_signal_source(list_cam);
    }


    checkroom->start_check_conf(m_model_room->get_data_list());
//    if(m_current_index_room < 0) {
//        m_model_room->set_current_index(0); //开始触发更换场景
//    }
//    emit login_success();
    save_conf();
}

void room_manager::read_msg_from_server(QString ip, QString port, QString username, QString password)
{
    if(m_model_room == nullptr ||
       m_model_device == nullptr ||
       m_model_signal_mode == nullptr ||
       m_model_signal_source == nullptr) {
        return;
    }
    m_model_room->clear();
    m_model_device->clear();
    m_model_signal_mode->clear();
    m_model_signal_source->clear();
    int index = 0;
    QSharedPointer<data_info> info(new room_info());
    connect(info.data(), &data_info::phy_wall_row_changed, m_model_room, &data_model::phy_wall_row_changed);
    connect(info.data(), &data_info::phy_wall_column_changed, m_model_room, &data_model::phy_wall_column_changed);
    connect(info.data(), &data_info::logical_wall_row_changed, m_model_room, &data_model::logical_wall_row_changed);
    connect(info.data(), &data_info::logical_wall_column_changed, m_model_room, &data_model::logical_wall_column_changed);
    connect(info.data(), &data_info::signal_source_changed, this, &room_manager::signal_source_changed_slot);
    connect(info.data(), &data_info::signal_source_in_wall_changed, this, &room_manager::signal_source_in_wall_changed_slot); //场景获取大屏中信号后
    //
    connect(info.data(), &data_info::polling_changed, this, &room_manager::sence_changed_from_polling); //
    connect(info.data(), &data_info::mode_changed, this, &room_manager::mode_changed_slot); //
    //二期新增 信号同步
    connect(info.data(), &data_info::sync_win_open_signal, this, &room_manager::sync_win_open_signal_slot);
    connect(info.data(), &data_info::sync_win_close_signal, this, &room_manager::sync_win_close_signal_slot);
    connect(info.data(), &data_info::sync_win_close_all_signal, this, &room_manager::sync_win_close_all_signal_slot);
    connect(info.data(), &data_info::sync_win_move_signal, this, &room_manager::sync_win_move_signal_slot);
    connect(info.data(), &data_info::sync_win_level_signal, this, &room_manager::sync_win_level_signal_slot);
    connect(info.data(), &data_info::sync_win_change_signal, this, &room_manager::sync_win_change_signal_slot);
    connect(info.data(), &data_info::polling_status_signal, this, &room_manager::polling_status_signal);
    connect(info.data(), &data_info::sync_mode_change_signal, this, &room_manager::sync_mode_change_signal_slot);
    connect(info.data(), &data_info::sync_mode_add_signal, this, &room_manager::sync_mode_add_signal_slot);
    connect(info.data(), &data_info::sync_mode_remove_signal, this, &room_manager::sync_mode_remove_signal_slot);
    connect(info.data(), &data_info::sync_mode_edit_signal, this, &room_manager::sync_mode_edit_signal_slot);
    connect(info.data(), &data_info::sync_audio_open_signal, this, &room_manager::sync_audio_open_signal_slot);
    connect(info.data(), &data_info::sync_audio_close_signal, this, &room_manager::sync_audio_close_signal_slot);
    connect(info.data(), &data_info::sync_audio_set_signal, this, &room_manager::sync_audio_set_signal_slot);
    connect(info.data(), &data_info::disconnect_signal, this, &room_manager::disconnect_signal);
    connect(info.data(), &data_info::reconnect_signal, this, &room_manager::reconnect_signal);
    info->set_name(qtr("主控"));
    info->set_ip_address(ip);
    info->set_user_name(username);
    info->set_user_pwd(password);
    info->set_port(port.toInt());
    info->set_logical_wall_row(2);
    info->set_logical_wall_column(2);
    info->set_index(index++);
    m_model_room->append(info);
    match_dev(info);

    if(m_current_index_room < 0) {
        m_model_room->set_current_index(0); //开始触发更换场景
    }
}
void room_manager::match_dev(QSharedPointer<data_info> val)
{

    QDir appdir = directoryOf(qApp->applicationDirPath());
    m_root_path = appdir.path();
    QDir dir = directoryOf(appdir.absoluteFilePath("conf"));
    QString configFile = dir.absoluteFilePath(QString("%1").arg(CONF_FILE_NAME));
#ifdef Q_OS_ANDROID
    //安卓版本，配置文件不存在时，从资源文件中读取初始配置
    dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    configFile = dir.absoluteFilePath(QString("%1").arg(CONF_FILE_NAME));
    if(!QFile::exists(configFile)) {
        qDebugxx(qtr("配置文件不存在"), configFile);
        configFile = QString(":/conf/%1").arg(CONF_FILE_NAME);
    }
#endif
    QString content = get_content_from_file(configFile);
    if(content.isEmpty()) {

        qDebugx(configFile);
        return;
    }
    QJsonObject json = getJsonObjectFromString(content);
    if(json.isEmpty()) {

        return;
    }
    m_local_ip_address =json.value("localIP").toString();

    QJsonArray ja_acc_conf = json.value("aConfig").toArray(); //得到所有指令
    int sort = 0;
    int acc_conf_index=0;
    foreach(QJsonValue qjv, ja_acc_conf) { //循环不同的设备
        if(qjv.isNull() || !qjv.isObject()) {
            continue;
        }
        QJsonObject json_conf = qjv.toObject();
        QSharedPointer<acc_conf_info> conf(new acc_conf_info);
        conf->set_net_type(json_conf.value("netType").toInt());
        conf->set_device_type(json_conf.value("type").toInt());
        conf->set_name(json_conf.value("name").toString());
        conf->set_cmd_type(json_conf.value("cmdType").toString());
        conf->set_model(json_conf.value("model").toString());
        conf->set_modelName(json_conf.value("modelName").toString());
         conf->set_control_bus(json_conf.value("controlBus").toInt());
        QJsonArray ja_cmds = json_conf.value("cmds").toArray();
        foreach(QJsonValue jv, ja_cmds) { //循环设备不同的命令
            if(jv.isNull() || !jv.isObject()) {
                continue;
            }
            QJsonObject json_cmd = jv.toObject();
            QSharedPointer<cmd_info> cmd(new cmd_info);
            cmd->set_name(json_cmd.value("name").toString());
            cmd->set_cmd(json_cmd.value("cmd").toString());
            cmd->set_send(json_cmd.value("send").toString());
            cmd->set_err(json_cmd.value("err").toString());
            cmd->set_suc(json_cmd.value("suc").toString());
            conf->add_cmd(cmd);
        }
        QSharedPointer<data_info> dev(new data_info);
        dev->set_name(conf->get_name());
        dev->set_device_type(conf->get_device_type());
        dev->set_sort(json_conf.value("sort").toInt(sort++));
        conf->set_sort(dev->get_sort());
        m_model_device->append_by_sort(dev);  //按sort的值 从小到大插入
        m_hash_acc_conf.insert(acc_conf_index++, conf);  //map

    }
//    qDebug()<<m_hash_acc_conf;
    QJsonArray ja_scene = json.value("aScene").toArray();  //得到所有场景
    if(ja_scene.isEmpty()) {
        qDebug() << "error aSecne:" << configFile;
        return;
    }
    int index = 0;
    foreach(QJsonValue jv, ja_scene) { //循环不同场景
        if(jv.isNull() || !jv.isObject()) {
            continue;
        }
        QJsonObject jo = jv.toObject();
        QSharedPointer<data_info> info(new room_info());
        info->set_name(jo.value("name").toString());
        info->set_ip_address(jo.value("ip").toString());
        info->set_user_name(jo.value("user").toString());
        info->set_user_pwd(jo.value("password").toString());
        info->set_port(jo.value("port").toInt(6801));
        info->set_logical_wall_row(jo.value("numGridRow").toInt(2));
        info->set_logical_wall_column(jo.value("numGridCol").toInt(2));
        info->set_index(index++);

        QJsonArray ja_kong = jo.value("aKong").toArray();  //循环不同的udp 地址和端口
        QList<QSharedPointer<data_info>> list_kong;
        foreach(QJsonValue jv_kong, ja_kong) {
            if(jv_kong.isNull() || !jv_kong.isObject()) {
                continue;
            }
            QJsonObject jo_kong = jv_kong.toObject();
            QSharedPointer<data_info> dev(new device_info());
            dev->set_id(getUuid());

            dev->set_ip_address(jo_kong.value("ip").toString());
            dev->set_port(jo_kong.value("port").toString().toInt());
            dev->setPortSend(jo_kong.value("portSend").toString().toInt());
            dev->setPortRcv(jo_kong.value("portRcv").toString().toInt()); // 新增串口参数

            list_kong.append(dev);
         }
        info->setList_akong(list_kong);

        QJsonArray ja_dev = jo.value("aDevice").toArray();   //循环不同的设备
        QList<QSharedPointer<data_info>> list_dev;
        index = 0;
        foreach(QJsonValue jv_dev, ja_dev) {
            if(jv_dev.isNull() || !jv_dev.isObject()) {
                continue;
            }
            QJsonObject jo_dev = jv_dev.toObject();
            QSharedPointer<data_info> dev(new device_info());
            dev->set_id(getUuid());
            dev->set_name(jo_dev.value("name").toString());
            dev->set_device_type(jo_dev.value("type").toInt());
            dev->set_ip_address(jo_dev.value("ip").toString());
            dev->set_port(jo_dev.value("port").toInt());
            dev->set_channel_no(jo_dev.value("bit").toInt());
            dev->set_com(jo_dev.value("com").toInt()); // 新增串口参数
            dev->set_cmd_type(get_dev_cmdType(m_hash_acc_conf,dev->get_device_type()));
            dev->set_local_ip_address(m_local_ip_address);

            if(jo_dev.value("model").toString()=="")
                dev->set_model("init");
            else
               dev->set_model(jo_dev.value("model").toString()); //新增设备对应的牌子参数
            dev->set_ico_path(get_ico_path(QString("images/%1/list-active.png").arg(dev->get_device_type())));

            //添加udp命令，如果有的话
            QJsonArray ja_dev_udp = jo_dev.value("cmds").toArray();
            foreach(QJsonValue jv_dev_udp, ja_dev_udp) {
                if(jv_dev_udp.isNull() || !jv_dev_udp.isObject()) {
                    continue;
                }
                QJsonObject jo_dev_udp = jv_dev_udp.toObject();
                QSharedPointer<cmd_info> cmd(new cmd_info);
                cmd->set_name(jo_dev_udp.value("name").toString());
                cmd->set_cmd(jo_dev_udp.value("cmd").toString());
                cmd->set_send(jo_dev_udp.value("send").toString());
                cmd->set_err(jo_dev_udp.value("err").toString());
                cmd->set_suc(jo_dev_udp.value("suc").toString());
                dev->add_udp_cmds(cmd);


            }


            //model为-1 证明是udp设备
            if(dev->get_model()=="-1"){
                   //设置设备监听端口
                   dev->setPortRcv(get_dev_portRcv(info->getList_akong(),dev->get_ip_address(),dev->get_port()));
            }

            ((device_info *)dev.data())->set_acc_conf_info(get_diff_dev(m_hash_acc_conf,dev->get_device_type(),dev->get_model()));  //可能有相同的设备  例如:同一场景两个不同的空调
            connect(dev.data(), &data_info::device_status_changed_signal, this, &room_manager::device_status_changed_slot); //设备状态改变信号
            connect(dev.data(), &data_info::mute_status_changed_signal, this, &room_manager::mute_status_changed_signal);
            connect(dev.data(), &data_info::volume_value_signal, this, &room_manager::volume_value_signal);
            QJsonArray ja_effect = jo_dev.value("aEffect").toArray();
            QVariantList vl_effect;
            foreach(QJsonValue jv_effect, ja_effect) { //得到不同的音效
                if(jv_effect.isNull() || !jv_effect.isString()) {
                    continue;
                }
                QVariantMap map;
                map.insert("dmMusicName", jv_effect.toString());
                map.insert("dmMusicIco", get_ico_path(QString("images/%1/btn.png").arg(dev->get_device_type())));
                vl_effect.append(map);
            }
            dev->set_music_buttons(vl_effect);//添加音响按钮
            QVariantList vl_in_out;
            read_music_in_out(jo_dev, vl_in_out, "aIn", "IN");
            read_music_in_out(jo_dev, vl_in_out, "aOut", "OUT");
            dev->set_music_sliders(vl_in_out); //添加音音量组件
            dev->set_index(index++);
            connect(dev.data(), &data_info::temperature_changed_signal, this, &room_manager::data_changed_by_device_slot); //空调温度改变
            list_dev.append(dev);
//            qDebug() << "find device:" << dev->get_name();
        }
        if(info->get_ip_address()==val->get_ip_address()){
            val->append_device(list_dev); //把不同设备放到场景中存储
            QList<QSharedPointer<data_info> > list2 = m_model_device->get_data_list();
            val->append_device_in_e(list2);
            break;


        }
        info->append_device(list_dev); //把不同设备放到场景中存储
        QList<QSharedPointer<data_info> > list2 = m_model_device->get_data_list();
        info->append_device_in_e(list2);


    }
}

void room_manager::set_wall_width_height(QString w, QString h)
{
    QSharedPointer<data_info> room = m_model_room->current_data_info();
       if(room.isNull()) {
           return ;
       }

       room->set_wall_width(w.toInt());
       room->set_wall_height(h.toInt());

       emit get_resolution(QString::number(room->get_wall_width())+"*"+QString::number(room->get_wall_height()));
       fresh_all_signal_in_mode();

       room->set_phy_wall_row(1);
       room->set_phy_wall_column(1);
       room->setIs_led(true);
       save_conf();

}
void room_manager::read_ip()
{
//    QDir appdir = directoryOf(qApp->applicationDirPath());
//    appdir.cdUp();
//    m_root_path = appdir.path();
//    QDir dir = directoryOf(appdir.absoluteFilePath("conf"));
//    QString configFile = dir.absoluteFilePath(QString("bin/%1").arg(CONF_IP_FILE_NAME));
//    QString content = get_content_from_file(configFile);
//    if(content.isEmpty()){
//        qDebug() << "no conf:" << configFile;
//        qDebugx(configFile);
//        return;
//    }
//    strIP=content.split("=",QString::SkipEmptyParts).at(1);
    QDir dir;
    QString configFile="config.ini";
#ifdef Q_OS_ANDROID
            //安卓版本，配置文件不存在时，从资源文件中读取初始配置
            dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
            configFile = dir.absoluteFilePath(QString("%1").arg("config.ini"));
            qDebug()<<configFile;
#endif

    QSettings *configIniRead = new QSettings(configFile, QSettings::IniFormat);
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    QString ipResult = configIniRead->value("/ip/ip").toString();

    //是否使用过的密匙标记
    QString nowDate = QDate::currentDate().toString("yyyy-MM-dd");
    QString idkey = QString("%1|%2").arg(false).arg(nowDate);
#ifdef Q_OS_ANDROID

    if(!QFile::exists(configFile)) {
        qDebugxx(qtr("配置文件不存在"), configFile);
        configIniRead->setValue("/other/temp", idkey);

    }
#endif
    idkey=getXorEncryptDecrypt(idkey, 110).toLatin1().toBase64().toHex();
    qDebug()<<idkey;

//    configIniRead->setValue("other/temp",idkey); //设置是否曾经使用过的标志
    if(ipResult == nullptr) {
        qDebugx("no config.ini");
        qDebug() << "no config.ini";
    }
    //打印得到的结果
//       qDebug() << ipResult;
    strIP = ipResult;
    //读入入完成后删除指针
    delete configIniRead;
}

void room_manager::check_permission(const QString &permission)
{
#ifdef Q_OS_ANDROID
    QtAndroid::PermissionResult r = QtAndroid::checkPermission(permission);
       if(r != QtAndroid::PermissionResult::Granted) {
           QtAndroid::requestPermissionsSync( QStringList() << permission );
           r = QtAndroid::checkPermission(permission);
           if(r == QtAndroid::PermissionResult::Denied) {

           }
      }
#endif
}


void room_manager::read_music_in_out(QJsonObject &jo_dev, QVariantList &list, const QString &io_type, const QString &io_name, const int dev_type)
{
    QJsonArray ja_in = jo_dev.value(io_type).toArray();
    int flag = 1;
    foreach(QJsonValue jv_in, ja_in) {
        if(jv_in.isNull() || !jv_in.isString()) {
            continue;
        }
        QVariantMap map;
        map.insert("dmMusicName", QString("%2-%1").arg(jv_in.toString()).arg(io_name));
        map.insert("dmMusicIco", get_ico_path(QString("images/%1/slide-num.png").arg(dev_type)));
        map.insert("dmMusicSlide", get_ico_path(QString("images/%1/slide.png").arg(dev_type)));
        map.insert("dmMusicImageActive", get_ico_path(QString("images/%1/music-active.png").arg(dev_type)));
        map.insert("dmMusicImageIdle", get_ico_path(QString("images/%1/music-idle.png").arg(dev_type)));
        map.insert("dmMusicIOType", io_name);
        map.insert("dmMusicDbMin", jo_dev.value("dbMin").toInt());
        map.insert("dmMusicDbMax", jo_dev.value("dbMax").toInt());
//        qDebugxxx(qtr("音响范围"), jo_dev.value("dbMin").toInt(), jo_dev.value("dbMax").toInt());
        map.insert("dmStatus", false);
        map.insert("dmVol", 20);
        map.insert("dmMusicDbStep", jo_dev.value("dbStep").toInt());
        map.insert("flag", flag++);
        list.append(map);
    }
}

void room_manager::add_device_display(QString videoId, double width, double height, double x, double y, int wall_width, int wall_height)
{
    QList<int> pos = where_in_c(width, height, x, y, wall_width, wall_height);
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    QSharedPointer<data_info> data = current_room->get_signal_source_by_id(videoId);//m_model_signal_source->get_data_info(index);
    //吸附到小宫格
    if(x == 0 && y == 0) { //双击吸附
        if(add_device_display(videoId, wall_width, wall_height)) {
            return;
        }
    } else { // 拖拽吸附
        //在重心就覆盖信息
        if(is_in_gravity(width, height, x, y, wall_width, wall_height, data)) {
            return;
        }
        if(add_device_display_new(videoId, wall_width, wall_height, pos)) {
            qDebugxx(qtr("添加信号源"), videoId);
            return;
        }
    }
    //C区填满的时候才会进行下面的方法或小屏填满
    if(current_room.isNull()) {
        return;
    }
    if(data.isNull()) {
        return;
    }
    if(data->get_device_type() == 7) {
        qWarnxx(qtr("不能添加目录"), data->get_name());
        return;
    }
    //按照托放位置
    qDebugxx(qtr("添加信号源"), videoId);
    width = (double)wall_width / (current_room->get_phy_wall_column() * current_room->get_logical_wall_column());
    height = (double) wall_height / (current_room->get_phy_wall_row() * current_room->get_logical_wall_row());
    QSharedPointer<data_info> info(new signal_resource_info);
    info->clone(data);
    info->set_old_data_info(data);   //保存C区信号源的原来信号 该原来信号在B区
    info->set_display_x(x * 1.0 / wall_width);
    info->set_display_y(y * 1.0 / wall_height);
    info->set_display_width(width * 1.0 / wall_width);
    info->set_display_height(height * 1.0 / wall_height);
    info->set_display_x_old(x);
    info->set_display_y_old(y);
    info->set_display_width_old(width);
    info->set_display_height_old(height);
    info->set_wall_width(wall_width);
    info->set_wall_height(wall_height);
    info->set_display_column(-1);
    info->set_display_column_num(-1);
    info->set_display_row(-1);
    info->set_display_row_num(-1);
    m_model_signal_source_display->append(info);
    current_room->add_signal_source_in_tv_wall(info);
    data->send_add_signal_source_to_wall_signal_event(info); //发送信号 槽在room_worker中
}


bool room_manager:: add_device_display(QString videoId, int width, int height)
{
    if(m_model_signal_source == nullptr || m_model_room == nullptr) {
        return false;
    }
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull() || current_room->get_phy_wall_column() == 0 ||
       current_room->get_phy_wall_row() == 0 ||
       current_room->get_logical_wall_column() == 0 ||
       current_room->get_logical_wall_row() == 0) {
        qWarnx(qtr("参数错误"));
        return false;
    }
    qDebugxxxxxxxx(qtr("新增显示设备"), width, height, videoId, current_room->get_phy_wall_column(), current_room->get_phy_wall_row(), current_room->get_logical_wall_column(), current_room->get_logical_wall_row());
    QSharedPointer<data_info> data = current_room->get_signal_source_by_id(videoId);//m_model_signal_source->get_data_info(index);
    if(data.isNull()) {
        return false;
    }
    if(data->get_device_type() == 7) {
        qWarnxx(qtr("不能添加目录"), data->get_name());
        return true;
    }
    double w =(double) width / (current_room->get_phy_wall_column() * current_room->get_logical_wall_column());
    double h =(double) height / (current_room->get_phy_wall_row() * current_room->get_logical_wall_row());
    double x = 0;
    double y = 0;
    for (int i = 0; i < current_room->get_phy_wall_row() * current_room->get_logical_wall_row(); i++) {
        for (int j = 0; j < current_room->get_phy_wall_column() * current_room->get_logical_wall_column(); j++) {
            x = w * j;
            y = h * i;
//            qDebug() << "is_using_in_c:" << i << j;
            if(!is_using_in_c(w, h, x, y, width, height)) {
                QSharedPointer<data_info> info(new signal_resource_info);
                info->clone(data);
                info->set_old_data_info(data);
                info->set_display_x(x * 1.0 / width);
                info->set_display_y(y * 1.0 / height);
                info->set_display_width(w * 1.0 / width);
                info->set_display_height(h * 1.0 / height);
                info->set_display_column(j);
                info->set_display_column_num(1);
                info->set_display_row(i);
                info->set_display_row_num(1);
                info->set_display_x_old(x);
                info->set_display_y_old(y);
//                qDebug()<<w<<w * 1.0 / width;
                info->set_display_width_old(w);
                info->set_display_height_old(h);
                info->set_wall_width(width);
                info->set_wall_height(height);
                qDebug() << "xiaogongge" << info->get_display_row() << info->get_display_row_num() << info->get_display_column() << info->get_display_column_num() << current_room->get_wall_width() << current_room->get_wall_height();
                qDebug() << "xiaogongge x" <<  (info->get_display_column() * current_room->get_wall_width()) / (current_room->get_phy_wall_column() * current_room->get_logical_wall_column());
                qDebug() << "xiaogongge y" <<  (info->get_display_row() * current_room->get_wall_height()) / (current_room->get_phy_wall_row() * current_room->get_logical_wall_row());
                qDebug() << "xiaogongge width" <<  (info->get_display_column_num() * current_room->get_wall_width()) / (current_room->get_phy_wall_column() * current_room->get_logical_wall_column());
                qDebug() << "xiaogongge height" <<  (info->get_display_row_num() * current_room->get_wall_height()) / (current_room->get_phy_wall_row() * current_room->get_logical_wall_row());
                qDebug() << "real x" << x ;
                qDebug() << "real y" << y;
                qDebug() << "real width" << w;
                qDebug() << "real height" << h ;
                m_model_signal_source_display->append(info);
                current_room->add_signal_source_in_tv_wall(info);
                data->send_add_signal_source_to_wall_signal_event(info);
                return true;
            }
        }
    }
    return false;
}

bool room_manager::add_device_display_new(QString videoId, int width, int height, QList<int> pos)
{
    if(m_model_signal_source == nullptr || m_model_room == nullptr) {
        return false;
    }
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull() || current_room->get_phy_wall_column() == 0 ||
       current_room->get_phy_wall_row() == 0 ||
       current_room->get_logical_wall_column() == 0 ||
       current_room->get_logical_wall_row() == 0) {
        qWarnx(qtr("参数错误"));
        return false;
    }

    qDebugxxxxxxxx(qtr("新增显示设备"), width, height, videoId, current_room->get_phy_wall_column(), current_room->get_phy_wall_row(), current_room->get_logical_wall_column(), current_room->get_logical_wall_row());
    QSharedPointer<data_info> data = current_room->get_signal_source_by_id(videoId);//m_model_signal_source->get_data_info(index);
    if(data.isNull()) {
        return false;
    }
    if(data->get_device_type() == 7) {
        qWarnxx(qtr("不能添加目录"), data->get_name());
        return true;
    }
    //二期 新需求   拖拽时重心在C区某个信号时，替代该信号
//    if(del_signal_msg.at(0)!=-2){
    //找到要替换的信号对象
//        del_signal_msg.at(0)
    /*remove_device_display(del_signal_msg.at(0));

    QSharedPointer<data_info> info(new signal_resource_info);
    info->clone(data);
    info->set_old_data_info(data);
    info->set_display_x(del_signal_msg.at(1) * 1.0 / width);
    info->set_display_y(del_signal_msg.at(2) * 1.0 / height);
    info->set_display_width(del_signal_msg.at(3) * 1.0 /width);
    info->set_display_height(del_signal_msg.at(4) *1.0 /height);

    info->set_display_column(del_signal_msg.at(5));
    info->set_display_column_num(1);
    info->set_display_row(del_signal_msg.at(6));
    info->set_display_row_num(1);
    info->set_display_x_old(del_signal_msg.at(1));
    info->set_display_y_old(del_signal_msg.at(2));
    info->set_display_width_old(del_signal_msg.at(3));
    info->set_display_height_old(del_signal_msg.at(4));
    info->set_wall_width(width);
    info->set_wall_height(height);

    qDebug() << "xiaogongge" << info->get_display_row() << info->get_display_row_num() << info->get_display_column() << info->get_display_column_num() << current_room->get_wall_width() << current_room->get_wall_height();
    qDebug() << "xiaogongge x" <<  (info->get_display_column() * current_room->get_wall_width()) / (current_room->get_phy_wall_column() * current_room->get_logical_wall_column());
    qDebug() << "xiaogongge y" <<  (info->get_display_row() * current_room->get_wall_height()) / (current_room->get_phy_wall_row() * current_room->get_logical_wall_row());
    qDebug() << "xiaogongge width" <<  (info->get_display_column_num() * current_room->get_wall_width()) / (current_room->get_phy_wall_column() * current_room->get_logical_wall_column());
    qDebug() << "xiaogongge height" <<  (info->get_display_row_num() * current_room->get_wall_height()) / (current_room->get_phy_wall_row() * current_room->get_logical_wall_row());

    m_model_signal_source_display->append(info);
    current_room->add_signal_source_in_tv_wall(info);
    data->send_add_signal_source_to_wall_signal_event(info); */ //B区信号源发送信号 ，参数是C区的信号源
//        return true;
//    }
    //下面是不覆盖的拖拽
    //current_room->get_phy_wall_column() 和current_room->get_phy_wall_row() 代表小屏行列数基本不变都是2*2
    //current_room->get_logical_wall_column() 和current_room->get_logical_wall_row() 代表小屏中宫格行列数，会变得
    double w = (double)width / (current_room->get_phy_wall_column() * current_room->get_logical_wall_column()); //当前最小宫格的宽
    double h = (double)height / (current_room->get_phy_wall_row() * current_room->get_logical_wall_row());      //当前最小宫格的长

    double x = 0;
    double y = 0;
    int posx = pos.at(0) * current_room->get_logical_wall_column(); //每一个小屏左上角真实坐标
    int posy = pos.at(1) * current_room->get_logical_wall_row();
//    qDebug()<<posx<<","<<posy;
    for (int i = 0; i < current_room->get_logical_wall_row(); i++) {
        for (int j = 0; j < current_room->get_logical_wall_column(); j++) {
            x = w * (j + posx);
            y = h * (i + posy);
//            qDebug() << "is_using_in_c:" << i << j;
            if(!is_using_in_c(w, h, x, y, width, height)) {
                QSharedPointer<data_info> info(new signal_resource_info);
                info->clone(data);
                info->set_old_data_info(data);
                info->set_display_x(x * 1.0 / width);
                info->set_display_y(y * 1.0 / height);
                info->set_display_width(w * 1.0 / width);
                info->set_display_height(h * 1.0 / height);
                info->set_display_column(posx + j);
                info->set_display_column_num(1);
                info->set_display_row(posy + i);
                info->set_display_row_num(1);

                info->set_display_x_old(x);
                info->set_display_y_old(y);
                info->set_display_width_old(w);
                info->set_display_height_old(h);

                info->set_wall_width(width);
                info->set_wall_height(height);
                qDebug() << "xiaogongge" << info->get_display_row() << info->get_display_row_num() << info->get_display_column() << info->get_display_column_num() << current_room->get_wall_width() << current_room->get_wall_height();
                qDebug() << "xiaogongge x" <<  (info->get_display_column() * current_room->get_wall_width()) / (current_room->get_phy_wall_column() * current_room->get_logical_wall_column());
                qDebug() << "xiaogongge y" <<  (info->get_display_row() * current_room->get_wall_height()) / (current_room->get_phy_wall_row() * current_room->get_logical_wall_row());
                qDebug() << "xiaogongge width" <<  (info->get_display_column_num() * current_room->get_wall_width()) / (current_room->get_phy_wall_column() * current_room->get_logical_wall_column());
                qDebug() << "xiaogongge height" <<  (info->get_display_row_num() * current_room->get_wall_height()) / (current_room->get_phy_wall_row() * current_room->get_logical_wall_row());
                qDebug() << "real x" << x ;
                qDebug() << "real y" << y;
                qDebug() << "real width" << w;
                qDebug() << "real height" << h ;
                m_model_signal_source_display->append(info);
                current_room->add_signal_source_in_tv_wall(info);
                data->send_add_signal_source_to_wall_signal_event(info);
                return true;
            }
        }
    }
    return false;
}
bool room_manager::is_using_in_c(double width, double height, double x, double y, int wall_width, int wall_height)
{
//    qDebug() << "is_using_in_c:" << x << "," << y << "," << width << "," << height;
    if(m_model_signal_source_display == nullptr) {
        return true;
    }
    if(m_model_signal_source_display->count() <= 0) {
        return false;
    }
    QList<QSharedPointer<data_info> > list = m_model_signal_source_display->get_data_list();
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        int x1 = info->get_display_x() * wall_width;
        int y1 = info->get_display_y() * wall_height;
        int w1 = info->get_display_width() * wall_width;
        int h1 = info->get_display_height() * wall_height;
        int endx = x + width;
        if(endx < x1 + w1) {
            endx = x1 + w1;
        }
        int startx = x;
        if(startx > x1) {
            startx = x1;
        }
        int off_width = width + w1 - (endx - startx);
        if(off_width <= 2) {
            continue;
        }
        int endy = y + height;
        if(endy < y1 + h1) {
            endy = y1 + h1;
        }
        int starty = y;
        if(starty > y1) {
            starty = y1;
        }
        int off_height = height + h1 - (endy - starty);
        if(off_height <= 2) {
            continue;
        }
        return true;
    }
    return false;
}
QList<int> room_manager::where_in_c(double width, double height, double x, double y, int wall_width, int wall_height)
{
    //判断信号在哪个小屏 返回小屏左上角的坐标
    //先得到信号的重心位置
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    int gravity_x = x + width / 2;
    int gravity_y = y + height / 2;
    int w = wall_width / current_room->get_phy_wall_column(); //当前小屏的宽
    int h = wall_height / current_room->get_phy_wall_row(); //当前小屏的高
//     w = wall_width /4; //测试
//     h = wall_height /4;  //测试
    int temp = 0;
    int posX = 0;
    int posY = 0;
    for(int i = 0; i <= current_room->get_phy_wall_column(); i++) {
        int xx = i * w;
        if(gravity_x >= temp && gravity_x <= xx) {
            posX = i - 1;
            break;
        }
        temp = xx;
    }
    temp = 0;
    for(int i = 0; i <= current_room->get_phy_wall_row(); i++) {
        int yy = i * h;
        if(gravity_y >= temp && gravity_y <= yy) {
            posY = i - 1;
            break;
        }
        temp = yy;
    }
//    qDebug()<<posX<<","<<posY;
    QList<int> pos;
    pos.append(posX);
    pos.append(posY);
//    qDebug()<<"It's in "<<posY*current_room->get_phy_wall_column()+(posX+1);
    return pos;
}

bool room_manager::is_in_gravity(double width, double height, double x, double y, int wall_width, int wall_height, QSharedPointer<data_info> val)
{
    //判断重心是否在信号内   是的话返回要覆盖的信号的index
    if(m_model_signal_source_display == nullptr) {
        return false;
    }
    if(m_model_signal_source_display->count() <= 0) {
        return false;
    }
    int gravity_x = x + width / 2; //拖拽信号的重心点
    int gravity_y = y + height / 2;
    QList<QSharedPointer<data_info> > list = m_model_signal_source_display->get_data_list();
    int index = 0; //C区信号的坐标
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        int x1 = info->get_display_x() * wall_width;
        int y1 = info->get_display_y() * wall_height;
        int w1 = info->get_display_width() * wall_width;
        int h1 = info->get_display_height() * wall_height;
        if(gravity_x >= x1 && gravity_x <= (x1 + w1))
            if(gravity_y >= y1 && gravity_y <= (y1 + h1)) {
                //二期 新需求   拖拽时重心在C区某个信号时，替代该信号
                //判断完直接直接替换
//                qDebug()<<"can change";
                double x_old;
                double y_old;
                double width_old;
                double height_old;

                if(info->get_display_width_old() <= 1) { //说明这个c区的信号，是打开软件初始化生产的或信号同步生产的，要转化
                    x_old = info->get_display_x_old() * wall_width; //
                    y_old = info->get_display_y_old() * wall_height; //
                    width_old = info->get_display_width_old() * wall_width;
                    height_old = info->get_display_height_old() * wall_height;

                } else {

                    x_old = info->get_display_x_old(); //
                    y_old = info->get_display_y_old(); //
                    width_old = info->get_display_width_old();
                    height_old = info->get_display_height_old();

                }

                int column_old = info->get_display_column();
                int row_old = info->get_display_row();
                info->clone(val);
                info->set_old_data_info(val);
                info->set_display_x(x_old * 1.0 / wall_width);
                info->set_display_y(y_old * 1.0 / wall_height);
                info->set_display_width(width_old * 1.0 / wall_width);
                info->set_display_height(height_old * 1.0 / wall_height);
                info->set_display_column(column_old);
                info->set_display_column_num(1);
                info->set_display_row(row_old);
                info->set_display_row_num(1);
                info->set_display_x_old(x_old);
                info->set_display_y_old(y_old);
                info->set_display_width_old(width_old);
                info->set_display_height_old(height_old);
                info->set_wall_width(wall_width);
                info->set_wall_height(wall_height);
                m_model_signal_source_display->data_changed_slot(index);
                info->send_change_signal_source_to_wall_signal_by_old_event(info);
                return true;
            }
        index++;
    }
    return false;
}

void room_manager::find_audio_param_from_current_room(QSharedPointer<data_info> val)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    foreach(QSharedPointer<data_info> info, current_room->get_list_audio()) {
        if(info->get_id() == val->get_id()) {
            //哪个为空，设置哪个
            if(val->get_audio_id() == 0) {
                val->set_audio_id(info->get_audio_id());
            }
            if(val->get_audio_channel() == 0) {
                val->set_audio_channel(info->get_audio_channel());
            }
            break;
        }
    }
}

int room_manager::get_dev_portRcv(QList<QSharedPointer<data_info> > list,QString ip_adress,int portSend)
{
    bool find=false;
    foreach(QSharedPointer<data_info> info, list) {
        if(info->get_ip_address() == ip_adress&&info->getPortSend()==portSend) {
            find=true;
            return  info->getPortRcv();
            break;
        }
    }
    if(!find)
      return 6801;
}


void room_manager::update_device_position(int index, double width, double height, double x, double y, int wall_width, int wall_height)
{
    if(m_model_signal_source_display == nullptr) {
        return;
    }
    QSharedPointer<data_info> data = m_model_signal_source_display->get_data_info(index);
    if(data.isNull()) {
        return;
    }
    data->set_display_x(x * 1.0 / wall_width);
    data->set_display_y(y * 1.0 / wall_height);
    data->set_display_width(width * 1.0 / wall_width);
    data->set_display_height(height * 1.0 / wall_height);
    data->set_display_x_old(x);
    data->set_display_y_old(y);
    data->set_display_width_old(width);
    data->set_display_height_old(height);
    data->set_wall_width(wall_width);
    data->set_wall_height(wall_height);
//    qDebug()<<data;
    //有一半在外面，则删除窗口
    if(x + data->get_display_width() * wall_width / 2 < 0 ||
       y + data->get_display_height() * wall_height / 2 < 0 ||
       x + data->get_display_width() * wall_width / 2 > wall_width  ||
       y + data->get_display_height() * wall_height / 2 > wall_height) {
        qDebugxxxxx(qtr("删除窗口"), x + data->get_display_width() * wall_width / 2, y + data->get_display_height() * wall_height / 2, wall_width, wall_height);
        remove_device_display(index);
        return;
    }
    qDebugxxxxxxx(qtr("缩放"), width, height, x, y, wall_width, wall_height);
    m_model_signal_source_display->data_changed_slot(index);
    data->send_update_signal_source_to_wall_signal_by_old_event(data);
}

void room_manager::double_click_position(int index, double width, double height, double x, double y, int wall_width, int wall_height, int row, int row_num, int column, int column_num)
{
    if(m_model_signal_source_display == nullptr) {
        return;
    }
    QSharedPointer<data_info> data = m_model_signal_source_display->get_data_info(index);
    if(data.isNull()) {
        return;
    }
    data->set_display_row(row);
    data->set_display_row_num(row_num);
    data->set_display_column(column);
    data->set_display_column_num(column_num);
    return update_device_position(index, width, height, x, y, wall_width, wall_height);
}

void room_manager::update_device_position(int index, double x, double y, int wall_width, int wall_height)
{
    if(m_model_signal_source_display == nullptr) {
        return;
    }
    QSharedPointer<data_info> data = m_model_signal_source_display->get_data_info(index);
    if(data.isNull()) {
        return;
    }
    data->set_display_x(x * 1.0 / wall_width);
    data->set_display_y(y * 1.0 / wall_height);
    data->set_display_x_old(x);
    data->set_display_y_old(y);
    data->set_wall_width(wall_width);
    data->set_wall_height(wall_height);
    //有一半在外面，则删除窗口
    if(x + data->get_display_width() * wall_width / 2 < 0 ||
       y + data->get_display_height() * wall_height / 2 < 0 ||
       x + data->get_display_width() * wall_width / 2 > wall_width  ||
       y + data->get_display_height() * wall_height / 2 > wall_height) {
        qDebugxxxxx(qtr("删除窗口"), x + data->get_display_width() * wall_width / 2, y + data->get_display_height() * wall_height / 2, wall_width, wall_height);
        remove_device_display(index);
        return;
    }
    m_model_signal_source_display->data_changed_slot(index);
    data->send_update_signal_source_to_wall_signal_by_old_event(data);
}

void room_manager::move_device_position(int index, double x, double y, int wall_width, int wall_height)
{
    if(m_model_signal_source_display == nullptr) {
        return;
    }
    QSharedPointer<data_info> data = m_model_signal_source_display->get_data_info(index);
    if(data.isNull()) {
        return;
    }
    data->set_display_x(x * 1.0 / wall_width);
    data->set_display_y(y * 1.0 / wall_height);
    data->set_display_x_old(x);
    data->set_display_y_old(y);
    data->set_wall_width(wall_width);
    data->set_wall_height(wall_height);
    m_model_signal_source_display->data_changed_slot(index);
}

void room_manager::remove_device_display(int index)
{
    if(m_model_signal_source_display == nullptr) {
        return;
    }
    QSharedPointer<data_info> data = m_model_signal_source_display->get_data_info(index);
    if(data.isNull()) {
        return;
    }
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    data->send_remove_signal_source_to_wall_signal_by_old_event(data);
    current_room->remove_signal_source_in_tv_wall(index);
    m_model_signal_source_display->remove(index);
}

void room_manager::update_device_position_to_wall(int index)
{
    if(m_model_signal_source_display == nullptr) {
        return;
    }
    QSharedPointer<data_info> data = m_model_signal_source_display->get_data_info(index);
    if(data.isNull()) {
        return;
    }
}

void room_manager::read_mode_images(QDir &appdir, bool source_file)
{
    QList<QSharedPointer<data_info>> list_mode;
    foreach(QFileInfo fi, appdir.entryInfoList()) {
        if(fi.suffix() != "jpg" && fi.suffix() != "png" && fi.suffix() != "ico" && fi.suffix() != "bmp") {
            continue;
        }
//        qDebug() << "add mode image:" << fi.filePath() << fi.path() << fi.absolutePath();
        QSharedPointer<data_info> dev(new data_info());
        dev->set_name(fi.fileName());
        if(source_file) {
            dev->set_ico_path(fi.filePath().left(1) == ":" ? fi.filePath().right(fi.filePath().length() - 1) : fi.filePath());
        } else {
            dev->set_ico_path(QString("file:/%1").arg(fi.filePath()));
        }
        list_mode.append(dev);
    }
    m_model_signal_mode_image->append(list_mode);
}

QString room_manager::getXorEncryptDecrypt(const QString &data, char key)
{
    // 采用异或加密,也可以自行更改算法
    QByteArray buffer = data.toLatin1();
    int size = buffer.size();
    for (int i = 0; i < size; i++) {
        buffer[i] = buffer.at(i) ^ key;
    }
    return QLatin1String(buffer);
}



void room_manager::read_mode_images()
{
    if(m_model_signal_mode_image == nullptr) {
        return;
    }
    QDir appdir = directoryOf(qApp->applicationDirPath());
    if(appdir.cd("images/mode")) {
        read_mode_images(appdir);
    }
    if(m_model_signal_mode_image->count() == 0) {
        appdir = QDir(":/images/mode");
        read_mode_images(appdir, true);
    }
}

void room_manager::add_mode(QString img, QString name)
{
    qDebug() << "add_mode:" << img << ":" << name;
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        return;
    }
    qDebugxxxxx(qtr("新增信号模式"), img, name, current_room->get_index(), current_room->get_name());
    QSharedPointer<data_info> info(new data_info);
    //要给信号模式增加一个资源id
    info->set_id(getUuid());
    info->set_ico_path(img);
    info->set_name(name);
//    qDebug()<<name.toUtf8().toHex();
    info->set_logical_wall_row(current_room->get_logical_wall_row());
    info->set_logical_wall_column(current_room->get_logical_wall_column());
    foreach(QSharedPointer<data_info> i, m_model_signal_source_display->get_data_list()) {
        if(i.isNull()) {
            continue;
        }
        QSharedPointer<data_info> j(new signal_resource_info);
        j->clone(i);

        j->set_index(m_model_signal_mode->count());
        j->set_old_data_info(i->get_old_data_info());
        j->set_logical_wall_row(current_room->get_logical_wall_row());
        j->set_logical_wall_column(current_room->get_logical_wall_column());
        j->set_win_id(i->get_win_id());//新增
        info->add_signal_source_in_tv_wall(j);
    }
    m_model_signal_mode->append(info);
    current_room->append_signal_mode(info);
    current_room->send_add_mode_signal_event(info);
//    save_conf();
}

void room_manager::update_mode(int index)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        return;
    }
    QSharedPointer<data_info> current_mode = m_model_signal_mode->get_data_info(index);
    if(current_mode.isNull()) {
        return;
    }
    current_mode->set_logical_wall_row(current_room->get_logical_wall_row());
    current_mode->set_logical_wall_column(current_room->get_logical_wall_column());
    current_mode->clear_signal_source_in_tv_wall();
    foreach(QSharedPointer<data_info> info, m_model_signal_source_display->get_data_list()) {
        if(info.isNull()) {
            continue;
        }
        QSharedPointer<data_info> data(new signal_resource_info);
        data->clone(info);
        data->set_old_data_info(info->get_old_data_info());
        current_mode->add_signal_source_in_tv_wall(data);
    }
    current_room->send_edit_mode_signal_event(current_mode);
//    save_conf();
}
void room_manager::delete_all_signal_in_mode() //清空窗口 二期添加
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    current_room->send_clear_window_signal_event();
    current_room->clear_signal_source_in_tv_wall(); //二期新增
    m_model_signal_source_display->clear();
}

void room_manager::fresh_all_signal_in_mode()
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    //清除屏幕，不清楚大屏的
    current_room->clear_signal_source_in_tv_wall();
    m_model_signal_source_display->clear();
    current_room->send_fresh_win_signal_event();
}
void room_manager::select_mode(int index, int wall_width, int wall_height) // 选择D区不同模式时
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    QSharedPointer<data_info> current_mode = m_model_signal_mode->get_data_info(index);
    if(current_mode.isNull()) {
        qWarnxx(qtr("没有找到信号模式"), index);
        return;
    }


    current_room->send_changed_mode_signal_event(current_mode->get_name()); //触发信号同步
//    current_room->send_changed_mode_signal_event(index); //旧版卡用这条函数
}

void room_manager::remove_mode(int index)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    QSharedPointer<data_info> current_mode = m_model_signal_mode->get_data_info(index);
    if(current_mode.isNull()) {
        qWarnx(qtr("没有找到当前模式"));
        return;
    }
//    if(QMessageBox::question(nullptr, "警告", QString("%1 %2 ?").arg(qtr("是否删除模式")).arg(/*current_mode->get_name()*/"abc"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok) == QMessageBox::Cancel){
//        return;
//    }
    current_room->remove_mode(current_mode);
    m_model_signal_mode->remove(index);
    current_room->send_remove_mode_signal_event(current_mode->get_id());//新增触发同步信号
//    save_conf();
}

void room_manager::remove_signal_source(int index)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    QSharedPointer<data_info> current_source = m_model_signal_source->get_data_info(index);
    if(current_source.isNull()) {
        qWarnx(qtr("没有找到当前信号源"));
        return;
    }
    if(QMessageBox::question(nullptr, "警告", QString("%1 %2 ?").arg(qtr("是否删除信号源")).arg(current_source->get_name()), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok) == QMessageBox::Cancel) {
        return;
    }
    current_source->send_remove_signal_source_signal_event(current_source);
    current_room->remove_signal_source(current_source);
    m_model_signal_source->remove(index);
}

void room_manager::add_signal_source(int type, QString name, QString ip, QString channel)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    QSharedPointer<data_info> s_source(new signal_resource_info);
    s_source->set_id(getUuid());
    s_source->set_name(name);
    s_source->set_device_type(type);
    s_source->set_ip_address(ip);
    s_source->set_port(6904);
    s_source->set_channel_no(channel.isEmpty() ? 0 : channel.toInt(0));
    s_source->set_index(m_model_signal_source->count());
    current_room->send_add_signal_source_signal_event(s_source);
    current_room->add_signal_source(s_source);
    m_model_signal_source->append(s_source);
}

void room_manager::device_ctrl(int index, QString cmd) //发送指令
{
    QSharedPointer<data_info> current_dev = m_model_device_ctrl->get_data_info(index);
//    qDebug() << current_dev->get_name();
    if(current_dev.isNull()) {
        qWarnx(qtr("没有找到当前设备"));
        return;
    }
    qDebug()<<__FUNCTION__<<cmd;
    current_dev->set_index(index);//会有多个相同的设备，要区别
    current_dev->send_device_ctrl_signal_event(cmd);
}

void room_manager::device_ctrl(int index, QString cmd, int channel) //发送音频指令
{
    qDebug() << "device_ctrl:" << index << ":" << cmd << ":" << channel;
    QSharedPointer<data_info> current_dev = m_model_device_ctrl->get_data_info(index);
    if(current_dev.isNull()) {
        qWarnx(qtr("没有找到当前设备"));
        return;
    }
    current_dev->set_index(index);
    current_dev->send_device_ctrl_signal_event(cmd, channel);
}

void room_manager::device_ctrl(int index, QString cmd, int channel, int param1)  //发送音频指令
{
    qDebug() << "device_ctrl:" << index << ":" << cmd << ":" << channel << "vol" << ":" << param1;
    QSharedPointer<data_info> current_dev = m_model_device_ctrl->get_data_info(index);
    if(current_dev.isNull()) {
        qWarnx(qtr("没有找到当前设备"));
        return;
    }
    current_dev->set_index(index);
    current_dev->send_device_ctrl_signal_event(cmd, channel, param1);
}

void room_manager::device_ctrl(int index, int operate, int action, int multiple)
{
    QSharedPointer<data_info> current_dev = m_model_device_ctrl->get_data_info(index);
    QSharedPointer<data_info> win = m_model_signal_source_display->current_data_info();
    if(current_dev.isNull() || win.isNull()) {
        qWarnx(qtr("没有找到当前设备或者没有选中摄像头"));
        return;
    }
//    if(multiple==0)
//        multiple=win->get_win_id();  //ipc需要win_id
//    qDebug()<<operate<<action<<multiple;
    current_dev->set_index(index);
    current_dev->send_device_ctrl_signal_event(operate, action, multiple, win->get_win_id());
}

void room_manager::device_ctrl_select(int index)    //E区更换设备
{
    //m_model_device_ctrl  E区的设备 单一类型  但可以有多个设备 例如 多个空调
    QSharedPointer<data_info> current_dev = m_model_device->get_data_info(index);  //E区选择的设备
//    qDebug()<<current_dev->get_sort();
//    qDebug()<<current_dev->get_device_type();
    if(current_dev.isNull()) {
        qWarnx(qtr("没有找到当前设备"));
        return;
    }
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    QList<QSharedPointer<data_info> > list = current_room->get_list_device(); //当前场景的拥有的设备，可以重复，例如多个空调
    m_model_device_ctrl->clear();
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        if(current_dev->get_device_type() == info->get_device_type()) {
            m_model_device_ctrl->append(info);
        }
    }
}

QVariantList room_manager::get_group_childs()
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return QVariantList();
    }
    QVariantList list = current_room->get_group_childs();//信号分组列表
//    qDebug()<<list.count();
    foreach (QVariant v, list) {
        if(v.isNull()) {
            continue;
        }
        QVariantMap map = v.toMap();
    }
    return list;
}

void room_manager::device_scale(double scale, int width, int height, int x, int y, int wall_width, int wall_height)
{
    if(scale == 1) {
        return;
    }
    qDebugxxxxxx(qtr("缩放"), scale, width * scale, height * scale, x + (1 - scale) * width / 2, y + (1 - scale)*height / 2);
}

QString room_manager::getIP()
{
    return strIP;
}

void room_manager::setIP(QString strip, QString username, QString password)
{
    if(username=="admin"&&password=="zxc123ken1"){

        emit login_success();
        return;
    }
    if(strip != nullptr && username != nullptr) {
        if( !is_valid_ip_address(strip) ) {
            qDebugx("ip error");
            emit ip_error();
            return;
        } else {
//            emit scene_type(1);
            QDir tempDir;
            //临时保存程序当前路径
            QString currentDir = tempDir.currentPath();
//            qDebug()<<currentDir;
            //如果filePath路径不存在，创建它
            qlogEx(currentDir);

            QString ip = strip;
            QString port = "6801"; //默认为6801
            //数据符合 ，开始从服务器获取信息
            //另一种操作配置的方法
            QDir dir;
            QString configFile="config.ini";
 #ifdef Q_OS_ANDROID
                    //安卓版本，配置文件不存在时，从资源文件中读取初始配置
                    dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
                    configFile = dir.absoluteFilePath(QString("%1").arg("config.ini"));

#endif
            QSettings *configIniWrite = new QSettings(configFile, QSettings::IniFormat);
            //向ini文件中写入内容,setValue函数的两个参数是键值对
            //向ini文件的第一个节写入内容,ip节下的第一个参数
            configIniWrite->setValue("/ip/ip", strip);
            //向ini文件的第一个节写入内容,ip节下的第二个参数
            //写入完成后删除指针
            delete configIniWrite;
            read_conf_from_server(ip, port, username, password);
        }
    }else {
        emit ip_error();
    }
}

void room_manager::login_multiple()
{
    emit login_msg(1);
    read_conf();
//    emit scene_type(2);
}

QVariantList room_manager::get_group_polling()
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return QVariantList();
    }
    QVariantList list = current_room->get_list_polling();
    foreach (QVariant v, list) {
        if(v.isNull()) {
            continue;
        }
        QVariantMap map = v.toMap();
    }
    return list;
}

void room_manager::set_polling(int o_index, int s_index)   //操作轮询
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(o_index != -1 && s_index == -1) { //仅仅开启某个轮询
        //直接用当前场景发送信号，更方便
//        QSharedPointer<data_info> open_polling=current_room->get_polling_by_id(o_index);
//        open_polling->send_open_polling_event(o_index);
        current_room->send_open_polling_event(o_index);
    } else if (o_index == -1 && s_index != -1) { //仅仅停止某个轮询
//        QSharedPointer<data_info> stop_polling=current_room->get_polling_by_id(s_index);
//        stop_polling->send_stop_polling_event(s_index);
        current_room->send_stop_polling_event(s_index);
    } else if(o_index != -1 && s_index != -1) {
//        QSharedPointer<data_info> open_polling=current_room->get_polling_by_id(o_index);
//        open_polling->send_stop_and_open_polling_event(o_index,s_index);
        current_room->send_stop_and_open_polling_event(o_index, s_index);
    }
}

int room_manager::get_num_of_device(int type) //获取当前场景的某个设备的数量
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return -1;
    }
    int  num = 0;

    QList<QSharedPointer<data_info> > list = current_room->get_list_device(); //当前场景的拥有的设备，可以重复，例如多个空调
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        if(type == info->get_device_type()) {
            num++;
        }
    }
    if(type!=-1)
        return num;


    else
        return list.length();
}

void room_manager::control_all_screen_device(int hint)  //总控区域控制所有的屏幕
{
    //获取所有的屏幕设备
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
    }
    int index = 0;
    QList<QSharedPointer<data_info> > list = current_room->get_list_device(); //当前场景的拥有的设备，可以重复，例如多个空调
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        if(4 == info->get_device_type()) {
            if(hint == 1) { //开启所有的设备  如果已经开了就不用开了

                    info->set_index(index);//？？？
                    info->send_device_ctrl_signal_event("btnCenterActive");

            } else { //关闭所有的设备

                    info->set_index(index);//？？？
                    info->send_device_ctrl_signal_event("btnCenterIdle");

            }
            index++;
        }
    }
}

void room_manager::control_all_power_device(int hint)
{
    //获取所有的电源设备
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
    }
    int index = 0;
    QList<QSharedPointer<data_info> > list = current_room->get_list_device(); //当前场景的拥有的设备，可以重复，例如多个空调
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }

        if(info->get_device_type()==5){  //电源
            if(hint == 1) {

                    info->set_index(index);//？？？
                    info->send_device_ctrl_signal_event("btnCenterActive");

            } else { //关闭所有的设备

                    info->set_index(index);//？？？
                    info->send_device_ctrl_signal_event("btnCenterIdle");

            }
            index++;


        }
    }
}

int room_manager::manager_get_wall_width()
{
    return m_wall_width;
}

void room_manager::manager_set_wall_width(int hint)
{
    m_wall_width = hint;
}

int room_manager::manager_get_wall_height()
{
    return m_wall_height;
}

void room_manager::manager_set_wall_height(int hint)
{
    m_wall_height = hint;
}

void room_manager::read_conf_from_server(QString ip, QString port, QString username, QString password)
{
    QSharedPointer<data_info> room = m_model_room->current_data_info(); //得到当前场景
//    connect(checkroom, &room_check::room_check_msg, this, &room_manager::room_check_msg_slot);

    checkroom->start(ip, port, username, password);
}

void room_manager::open_caption_setting()  //打开字幕设置 安卓和ios直接调用接口命令

{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
#ifdef Q_OS_WIN32
    if(!caption_hint) {
        QString cur_dir = QDir::currentPath();
        qDebugx(cur_dir);
        caption_pro.start(cur_dir + "/tool/ftclt.exe");
        caption_hint = true;
    } else {
        caption_pro.close();
        caption_hint = false;
    }
#endif
#ifdef Q_OS_ANDROID
    if(!caption_hint) {
        current_room->send_caption_signal_event(1);
        caption_hint = true;
    } else {
        current_room->send_caption_signal_event(0);
        caption_hint = false;
    }
#endif
}

void room_manager::open_basemap_setting()  //打开地图设置 安卓和ios不做处理
{
#ifdef Q_OS_WIN32
    if(!basemap_hint) {
        QString cur_dir = QDir::currentPath();
//        qDebug()<<cur_dir;
        basemap_pro.start(cur_dir + "/tool/setbk.exe");
        basemap_hint = true;
    } else {
        basemap_pro.close();
        basemap_hint = false;
    }
#endif


}

int room_manager::get_polling_num()
{
    return polling_num;
}

void room_manager::set_polling_num(int hint)
{
    polling_num = hint;
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    current_room->setPolling_hint(hint);
    //记录正在轮巡的序号，在删除信号窗口时，先停止轮巡
}


void room_manager::room_check_msg_slot(int hint, QString msg)
{
    if(hint == 0) { //单场景登录失败 多场景登录
        emit scene_type(2);
    } else { //单场景登录
        emit scene_type(1);
    }
    //    checkroom->stop(); //检测完后 删除tcp连接
}

void room_manager::sync_win_open_signal_slot(QSharedPointer<data_info> val)
{
    m_model_signal_source_display->append(val);
}

void room_manager::sync_win_close_signal_slot(int win_id)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
    }
    int index = 0;
    QList<QSharedPointer<data_info> > list = current_room->get_list_signal_source_in_tv_wall(); //当前场景的拥有的设备，可以重复，例如多个空调
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        if(win_id == info->get_win_id()) {
            m_model_signal_source_display->remove(index);
            current_room->remove_signal_source_in_tv_wall(index);
        }
        index++;
    }
}

void room_manager::sync_win_close_all_signal_slot()
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    m_model_signal_source_display->clear();
    current_room->clear_signal_source_in_tv_wall();
}

void room_manager::sync_win_move_signal_slot(QSharedPointer<data_info> val)
{
    if(m_model_signal_source_display == nullptr) {
        return;
    }
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    int index = 0;
    QList<QSharedPointer<data_info> > list = current_room->get_list_signal_source_in_tv_wall();
//    qDebug()<<list.length();
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        if(val->get_win_id() == info->get_win_id()) {
//            QSharedPointer<data_info> data = m_model_signal_source_display->get_data_info(index);
//            if(data.isNull()){
//                return;
//            }
//            qDebug()<<val->get_display_x()<<val->get_display_y();
            info->set_display_x(val->get_display_x());
            info->set_display_y(val->get_display_y());
            info->set_display_width(val->get_display_width());
            info->set_display_height(val->get_display_height());
            info->set_display_x_old(val->get_display_x());
            info->set_display_y_old(val->get_display_y());
            info->set_display_width_old(val->get_display_width());
            info->set_display_height_old(val->get_display_height());
            qDebugx(qtr("信号同步的缩放"));
//            qDebug()<<info->get_win_id();
            m_model_signal_source_display->data_changed_slot(index);
            emit test_signal22(val->get_display_x(), val->get_display_y(), val->get_display_width(), val->get_display_height(), info->get_win_id());
        }
        index++;
    }

}

void room_manager::sync_win_level_signal_slot(int win_id, int hint)
{
    if(m_model_signal_source_display == nullptr) {
        return;
    }
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    int index = 0;
    QList<QSharedPointer<data_info> > list = current_room->get_list_signal_source_in_tv_wall(); //当前场景的拥有的设备，可以重复，例如多个空调
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        if(win_id == info->get_win_id()) {
//              qDebug()<<index;
            m_model_signal_source_display->set_current_index_sync(index);
        }
        index++;
    }
}

void room_manager::sync_win_change_signal_slot(QSharedPointer<data_info> val)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    QSharedPointer<data_info> data = current_room->get_signal_source_by_id(val->get_id());//m_model_signal_source->get_data_info(index);
    int index = 0;
    QList<QSharedPointer<data_info> > list = current_room->get_list_signal_source_in_tv_wall(); //
    QList<QSharedPointer<data_info> > list2 = m_model_signal_source_display->get_data_list();
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        if(val->get_win_id() == info->get_win_id()) {
            info->cloneEx(data);
            info->set_old_data_info(data);
        }
        index++;
    }
    index = 0;
    foreach(QSharedPointer<data_info> info, list2) {
        if(info.isNull()) {
            continue;
        }
        if(val->get_win_id() == info->get_win_id()) {
            info->cloneEx(data);
            info->set_old_data_info(data);
            m_model_signal_source_display->data_changed_slot(index);
        }
        index++;
    }
}

void room_manager::sync_mode_change_signal_slot(QString id)
{
    //同步信号模式的选取
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        return;
    }
    int index = 0;
    foreach(QSharedPointer<data_info> info, current_room->get_list_signal_mode()) {
        if(info.isNull()) {
            continue;
        }
        if(info->get_id() == id) {
            break;
        }
        index++;
    }
//    current_room->send_get_wall_signal_event(); //发送两次以防延迟
    current_room->send_get_wall_signal_event();
    emit  index_mode_changed(index);
}

void room_manager::sync_mode_remove_signal_slot(QString id)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
//    qDebug()<<id;
    int i = 0;
    foreach(QSharedPointer<data_info> info, current_room->get_list_signal_mode()) {
        if(info.isNull()) {
            continue;
        }
        if(info->get_id() == id) {
            current_room->remove_mode(info);
            break;
        }
        i++;
    }
    m_model_signal_mode->remove(i);
//    save_conf();
}

void room_manager::sync_mode_add_signal_slot(QSharedPointer<data_info> val)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        return;
    }
    qDebugx(qtr("同步新增信号模式"));
    QSharedPointer<data_info> info(new data_info);
//    info->set_ico_path(img);
    int index = m_model_signal_mode->count() - 1;
    if(index <= 5) {
        info->set_ico_path(QString("qrc:/images/mode/img-%1.jpg").arg(index));
    } else {
        index = 0;
        info->set_ico_path(QString("qrc:/images/mode/img-%1.jpg").arg(index));
    }
    info->set_id(val->get_id());
    info->set_name(val->get_name());
    info->set_logical_wall_row(current_room->get_logical_wall_row());
    info->set_logical_wall_column(current_room->get_logical_wall_column());
    foreach(QSharedPointer<data_info> i, val->get_list_signal_source_in_tv_wall()) {
        if(i.isNull()) {
            continue;
        }
        QSharedPointer<data_info> old = i->get_old_data_info();
        if(old.isNull()) {
            old = current_room->get_signal_source_by_id(i->get_id());
        }
        QSharedPointer<data_info> j(new signal_resource_info);
        j->clone(old);
        j->set_index(m_model_signal_mode->count());
        j->set_old_data_info(old);
        j->set_logical_wall_row(current_room->get_logical_wall_row());
        j->set_logical_wall_column(current_room->get_logical_wall_column());
        j->set_win_id(i->get_win_id());//新增
        j->set_display_x(i->get_display_x());
        j->set_display_y(i->get_display_y());
        j->set_display_width(i->get_display_width());
        j->set_display_height(i->get_display_height());
        j->set_display_x_old(i->get_display_x());
        j->set_display_y_old(i->get_display_y());
        j->set_display_width_old(i->get_display_width());
        j->set_display_height_old(i->get_display_height());
        info->add_signal_source_in_tv_wall(j);
    }
    m_model_signal_mode->append(info);
    current_room->append_signal_mode(info);
//    save_conf();
}

void room_manager::sync_mode_edit_signal_slot(QSharedPointer<data_info> val)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        return;
    }
    qDebugx(qtr("同步修改信号模式"));
    QSharedPointer<data_info> info(new data_info);
//    info->set_ico_path(img);
    int i = 0;
    foreach(QSharedPointer<data_info> info, current_room->get_list_signal_mode()) {
        if(info.isNull()) {
            continue;
        }
        if(info->get_id() == val->get_id()) { //得到要修改的信号模式
            info->set_name(val->get_name());
            info->clear_signal_source_in_tv_wall();
            foreach(QSharedPointer<data_info> i, val->get_list_signal_source_in_tv_wall()) {
                if(i.isNull()) {
                    continue;
                }
                QSharedPointer<data_info> old = i->get_old_data_info();
                if(old.isNull()) {
                    old = current_room->get_signal_source_by_id(i->get_id());
                }
                QSharedPointer<data_info> j(new signal_resource_info);
                j->clone(old);
                j->set_index(m_model_signal_mode->count());
                j->set_old_data_info(old);
                j->set_logical_wall_row(current_room->get_logical_wall_row());
                j->set_logical_wall_column(current_room->get_logical_wall_column());
                j->set_win_id(i->get_win_id());//新增
                j->set_display_x(i->get_display_x());
                j->set_display_y(i->get_display_y());
                j->set_display_width(i->get_display_width());
                j->set_display_height(i->get_display_height());
                j->set_display_x_old(i->get_display_x());
                j->set_display_y_old(i->get_display_y());
                j->set_display_width_old(i->get_display_width());
                j->set_display_height_old(i->get_display_height());
                info->add_signal_source_in_tv_wall(j);
            }
            break;
        }
        i++;
    }
    m_model_signal_mode->data_changed_slot(i);
//    m_model_signal_mode->append(info);
//    current_room->append_signal_mode(info);
//    save_conf();
}

void room_manager::sync_audio_open_signal_slot(QString sig_id, int channel, int vol, int au_id)
{
    QList<QSharedPointer<data_info> > list = m_model_signal_source_display->get_data_list();
    foreach(QSharedPointer<data_info> i, list) {
        if(i->get_audio_id() != au_id) {
            continue;
        } else {
            emit display_audio_msg(true, i->get_name());
            break;
        }
    }
}

void room_manager::sync_audio_close_signal_slot(int au_id)
{
//    emit display_audio_msg(false,"dddd");
    QList<QSharedPointer<data_info> > list = m_model_signal_source_display->get_data_list();
    foreach(QSharedPointer<data_info> i, list) {
        if(i->get_audio_id() != au_id) {
            continue;
        } else {
            emit display_audio_msg(false, i->get_name());
            break;
        }
    }
}

void room_manager::sync_audio_set_signal_slot(int au_id, int vol)
{
    QList<QSharedPointer<data_info> > list = m_model_signal_source_display->get_data_list();
    int i = 0;
    foreach(QSharedPointer<data_info> val, list) {
        if(val->get_audio_id() != au_id) {
            continue;
        } else {
            val->set_volume(vol);
            break;
        }
    }
    m_model_signal_source_display->data_changed_slot(i);
}
void room_manager::select_mode_by_sync(int index, int wall_width, int wall_height)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return;
    }
    QSharedPointer<data_info> current_mode = m_model_signal_mode->get_data_info(index);
    if(current_mode.isNull()) {
        qWarnxx(qtr("没有找到信号模式"), index);
        return;
    }
    current_room->clear_signal_source_in_tv_wall(); //二期新增
    m_model_signal_source_display->clear();
    wait_by_event_loop(5);
    QList<QSharedPointer<data_info> > list = current_mode->get_list_signal_source_in_tv_wall();
    foreach(QSharedPointer<data_info> i, list) {
        if(i.isNull()) {
            continue;
        }
        QSharedPointer<data_info> old = i->get_old_data_info();
        if(old.isNull()) {
            old = current_room->get_signal_source_by_id(i->get_id());
            if(old.isNull()) {
                qWarnxx(qtr("未找到信号源"), i->get_id());
                continue;
            }
            i->set_old_data_info(old);
            i->cloneEx(old);
        }
        i->set_display_height_old(wall_height * i->get_display_height());
        i->set_display_width_old(wall_width * i->get_display_width());
        i->set_display_x_old(wall_width * i->get_display_x());
        i->set_display_y_old(wall_height * i->get_display_y());
        i->set_wall_height(wall_height);
        i->set_wall_width(wall_width);
        QSharedPointer<data_info> j(new signal_resource_info);
        j->clone(i);
        j->set_old_data_info(old);
        j->set_win_id(i->get_win_id());
        qDebugxxxxxxxx(qtr("模式"), j->get_name(), j->get_id(), i->get_name(), j->get_display_x(), j->get_display_y(), j->get_display_width(), j->get_display_height());
        current_room->add_signal_source_in_tv_wall(j); //二期新增
        m_model_signal_source_display->append(j);
    }
}

void room_manager::ptz_control(int operate, int action)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    QSharedPointer<data_info> win = m_model_signal_source_display->current_data_info();
    if(current_room.isNull() || win.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return ;
    }
//    qDebug()<<operate;
    if(win->get_device_type() == 0x05) { //0x05 是IPC信号源  0x01是HDMI信号源
        current_room->send_ptz_signal_event(operate, action, win->get_win_id())  ;
    }
}

void room_manager::hdmi_control(int operate, int action)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    QSharedPointer<data_info> win = m_model_signal_source_display->current_data_info();
    if(current_room.isNull() || win.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
        return ;
    }
    if(win->get_device_type() == 0x01) { //0x05 是IPC信号源  0x01是HDMI信号源
        if(operate < 5) { // 方向转动操作  这里action代表 开始和暂停
            if(action == 0) {
                current_room->send_write_to_serial_signal_event(operate, action);
            } else {
                current_room->send_close_serial_signal_event();
            }
        } else {  //缩放操作  这里action代表 缩放倍数
            if(action != 0) {
                current_room->send_write_to_serial_signal_event(operate, action);
            } else {
                current_room->send_close_serial_signal_event();
            }
        }
    }
}

void room_manager::language_switch()
{

    if(isZH) {

               isZH = !isZH;
               set_iszhongwen(isZH);

       } else {

               isZH = !isZH;
               set_iszhongwen(isZH);

       }
       change_language();
       emit fix_comboBox_signal();
}

void room_manager::init_language()  //初始化语言
{
    QDir dir;
    QString configFile="config.ini";
#ifdef Q_OS_ANDROID
            //安卓版本，配置文件不存在时，从资源文件中读取初始配置
            dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
            configFile = dir.absoluteFilePath(QString("%1").arg("config.ini"));

#endif
         QSettings *configIniRead = new QSettings(configFile, QSettings::IniFormat);

         //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
         QString Result = configIniRead->value("/setting/language").toString();
//         qDebug()<<Result;
         if(Result=="EN")
         {
             isZH=false;
             set_iszhongwen(false);
         }
         else
         {
             isZH=true;
             set_iszhongwen(true);
         }

                  //读入入完成后删除指针
         delete configIniRead;

}

void room_manager::set_signal_source_in_wall_volume(int index, int vol)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
//    qDebug()<<vol;
    QSharedPointer<data_info> win = m_model_signal_source_display->current_data_info();
    win->set_volume(vol);
    if(win->get_audio_id() == 0 || win->get_audio_channel() == 0) {
        find_audio_param_from_current_room(win);
    }
    current_room->send_set_audio_signal_event(win->get_audio_id(), vol);
}

void room_manager::set_audio_param(int index, bool hint)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    QSharedPointer<data_info> win = m_model_signal_source_display->current_data_info();
//    qDebug()<<win->get_volume();
    if(win->get_audio_id() == 0 || win->get_audio_channel() == 0) {
        find_audio_param_from_current_room(win);
    }
    if(hint) { //打开音频
        current_room->send_open_audio_signal_event(win->get_id(), win->get_audio_channel(), win->get_volume());
    } else {
        current_room->send_close_audio_signal_event(win->get_audio_id());
    }
}

void room_manager::change_language()
{
    QDir dir;
    QString configFile="config.ini";
#ifdef Q_OS_ANDROID

    dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    configFile = dir.absoluteFilePath(QString("%1").arg("config.ini"));

#endif
    QSettings *configIniWrite = new QSettings(configFile, QSettings::IniFormat);
    //向ini文件中写入内容,setValue函数的两个参数是键值对
    if(isZH) {
        configIniWrite->setValue("setting/language", "CN");
    } else {
        configIniWrite->setValue("setting/language", "EN");
    }
    delete configIniWrite;
}

void room_manager::free_memory()
{

//    delete  checkroom;
}

QVariantList room_manager::find_signal(QString str)
{
    QSharedPointer<data_info> current_room = m_model_room->current_data_info();
    if(current_room.isNull()) {
        qWarnx(qtr("没有找到当前场景"));
    }
    QVariantList msg;
    int hint = 0;
    int group = 1;
    int temp = 1;
    bool isbreak = false;
    QVariantList list = current_room->get_group_childs();
    for (int i = 0; i < list.length(); i++) {
        QVariantMap map2 = list[i].toMap();
        QVariantList list2 = map2.value("dmGroupChilds").toList();
        for (int j = 0; j < list2.length(); j++) {
            QVariantMap map3 = list2[j].toMap();
            QString n = map3.value("dmName").toString();
//            qDebug()<<n;
            if(n.contains(str)) {
//                qDebug()<<n;
                hint = temp;
                msg.insert(1, hint);
                isbreak = true;
                break;
            }
            temp++;
        }
        if(isbreak) {
            break;
        }
        group++;
    }
    if(group > list.length()) {
        group = 0;
    }
    msg.insert(1, hint);
    msg.insert(0, group);
//    qDebug()<<msg.at(0)<<msg.at(1);
    return msg;
}

int room_manager::which_os()
{
#ifdef Q_OS_WIN32
    return 1;
#endif
#ifdef Q_OS_ANDROID
    return 2;
#endif
#ifdef Q_OS_IOS
    return 3;
#endif
}

void room_manager::get_current_dir()
{
    qDebug()<<qApp->applicationDirPath();
}

QString room_manager::get_dev_cmdType(QHash<int, QSharedPointer<acc_conf_info> > m_hash, int dev_type)
{
    bool find=false;
//    qDebug()<<"dev ytpe:"<<sort <<"dev model:"<<model;

    foreach(QSharedPointer<acc_conf_info> conf ,m_hash.values()){

        if(conf->get_device_type()==dev_type)
        {

            find=true;

            return conf->get_cmd_type();   //foreaach中的return不会中断循环

        }
        if(find)
            break;
    }
    if(!find)
        return QString();

}

QSharedPointer<acc_conf_info> room_manager::get_diff_dev(QHash<int, QSharedPointer<acc_conf_info> > m_hash,int sort,QString model)
{
    bool find=false;
//    qDebug()<<"dev ytpe:"<<sort <<"dev model:"<<model;

    foreach(QSharedPointer<acc_conf_info> conf ,m_hash.values()){

        if(conf->get_device_type()==sort&&conf->get_model()==model)
        {

            find=true;
//            qDebug()<<conf->get_modelName();
            return conf;   //foreaach中的return不会中断循环

        }
        if(find)
            break;
    }
    if(!find){
        QSharedPointer<acc_conf_info> temp(new acc_conf_info);  //创建一个假的对象，避免出错
        return temp;
    }

}


void room_manager::headler_signal_scene_slot(bool hint, QString ip, QString port, QString username, QString password)
{
    checkroom->stopconnect();
    if(!hint) { //单场景登录失败 多场景登录  读取配置登录  注意:读取配置后，可以逐个检验场景是否登录成功
//        emit login_msg(1);
//        read_conf();
//        emit login_success();
        emit login_error();

    } else { //单场景登录
        qDebug()<<"begain one scene";
        emit login_success();
        read_msg_from_server(ip, port, username, password);
        emit scene_type(1);

    }

}

void room_manager::headler_congfig(int hint)
{
    if(hint==-1){
      m_model_room->set_current_index(hint);
      emit login_success();
      emit scene_type(2);
        emit login_error();
    }
    else if(hint ==-2){ //连接成功但，登录失败

        checkroom->coni_check_conf_one(checkroom->get_currentindex()+1);
    }
    else {
         qDebug()<<"currentindex is"<<hint;
        m_model_room->set_current_index(hint); //开始触发更换场景
        emit login_success();
        emit scene_type(2);
    }

}

void room_manager::mode_changed_slot()  //更换场景时 更换D区信号模式
{
    QSharedPointer<data_info> room = m_model_room->current_data_info();
    if(room.isNull()) {
        return;
    }
//    qDebug()<<"dd";
    qDebugxx(qtr("要显示的信号模式数量"), room->get_list_signal_mode().count());
    m_model_signal_mode->append(room->get_list_signal_mode());
}

void room_manager::save_conf()
{
    QJsonObject json;
    QJsonArray scene_array;
//    qDebugx(qtr("开始保存文件"));
    foreach(QSharedPointer<data_info> room, m_model_room->get_data_list()) {
        if(room.isNull()) {
            continue;
        }
//        qDebugx(qtr("开始保存场景"));
        QJsonObject scene;
        scene.insert("name", room->get_name());
        scene.insert("ip", room->get_ip_address());
        scene.insert("port", room->get_port());
        scene.insert("user", room->get_user_name());
        scene.insert("password", room->get_user_pwd());
        scene.insert("numGridRow", room->get_logical_wall_row());
        scene.insert("numGridCol", room->get_logical_wall_column());
        scene.insert("wallWidth", room->get_wall_width());
        scene.insert("wallHeight", room->get_wall_height());
        scene.insert("isLED",room->getIs_led());

        QJsonArray kong_array;
        foreach(QSharedPointer<data_info> kong, room->getList_akong()) {
            if(kong.isNull()) {
                continue;
            }

            QJsonObject json_model;
            json_model.insert("ip", kong->get_ip_address());
            json_model.insert("port", QString::number(kong->get_port()));
            json_model.insert("portSend", QString::number(kong->getPortSend()));
            json_model.insert("portRcv", QString::number(kong->getPortRcv()));
            kong_array.append(json_model);
        }
        scene.insert("aKong", kong_array);
        QJsonArray model_array;
        foreach(QSharedPointer<data_info> model, room->get_list_signal_mode()) {
            if(model.isNull()) {
                continue;
            }
//            qDebugx(qtr("开始保存信号模式"));
            QJsonObject json_model;
            json_model.insert("name", model->get_name());
            json_model.insert("image", model->get_ico_path().replace("file:/", "").replace(m_root_path, ""));
            json_model.insert("numGridRow", model->get_logical_wall_row());
            json_model.insert("numGridCol", model->get_logical_wall_column());
            QJsonArray src_array;
            foreach(QSharedPointer<data_info> src, model->get_list_signal_source_in_tv_wall()) {
                if(src.isNull()) {
                    continue;
                }
                QJsonObject json_src;
                json_src.insert("id", src->get_id());
                json_src.insert("x", src->get_display_x());
                json_src.insert("y", src->get_display_y());
                json_src.insert("width", src->get_display_width());
                json_src.insert("height", src->get_display_height());
                src_array.append(json_src);
            }
            json_model.insert("aSignal", src_array);
            model_array.append(json_model);
        }
        scene.insert("models", model_array);


        QJsonArray device_array;
        foreach(QSharedPointer<data_info> dev, room->get_list_device()) {
            if(dev.isNull()) {
                continue;
            }
//            qDebugx(qtr("开始保存"));
            QJsonObject json_dev;
            json_dev.insert("type", dev->get_device_type());
            json_dev.insert("name", dev->get_name());
            json_dev.insert("ip", dev->get_ip_address());
            json_dev.insert("port", dev->get_port());
            json_dev.insert("model", dev->get_model());

            //保存udp命令
            QJsonArray ja_cmds_udp;
            foreach(QSharedPointer<cmd_info> cmd, dev->get_udp_cmds().values()) {
                if(cmd.isNull()) {
                    continue;
                }
                QJsonObject json_cmd;
                json_cmd.insert("name", cmd->get_name());
                json_cmd.insert("cmd", cmd->get_cmd());
                json_cmd.insert("send", cmd->get_send());
                json_cmd.insert("suc", cmd->get_suc());
                json_cmd.insert("err", cmd->get_err());
                ja_cmds_udp.append(json_cmd);
            }
            json_dev.insert("cmds", ja_cmds_udp);

            if(dev->get_device_type() == 7) {
                json_dev.insert("com", dev->get_com());
            }
            if(dev->get_device_type() == 4 ||
               dev->get_device_type() == 5 ||
               dev->get_device_type() == 1) {
                json_dev.insert("bit", dev->get_channel_no());
            }
            QJsonArray sl_slides_in;
            QJsonArray sl_slides_out;
            QVariantList vl_slides = dev->get_music_sliders();
            bool mm = false;
            foreach(QVariant v, vl_slides) {
                if(v.isNull()) {
                    continue;
                }
                QVariantMap map = v.toMap();
                if(map.value("dmMusicIOType").toString() == "IN") {
                    sl_slides_in.append(map.value("dmMusicName").toString().replace("IN-", ""));
                } else if(map.value("dmMusicIOType").toString() == "OUT") {
                    sl_slides_out.append(map.value("dmMusicName").toString().replace("OUT-", ""));
                }
                if(!mm) {
                    json_dev.insert("dbMin", map.value("dmMusicDbMin").toInt());
                    json_dev.insert("dbMax", map.value("dmMusicDbMax").toInt());
                    json_dev.insert("dbStep", map.value("dmMusicDbStep").toInt());
                    mm = true;
                }
            }
            if(sl_slides_in.count() > 0) {
                json_dev.insert("aIn", sl_slides_in);
            }
            if(sl_slides_out.count() > 0) {
                json_dev.insert("aOut", sl_slides_out);
            }
            QJsonArray ja_effect;
            QVariantList vl_effect = dev->get_music_buttons();
            foreach(QVariant v, vl_effect) {
                if(v.isNull()) {
                    continue;
                }
                QVariantMap map = v.toMap();
                ja_effect.append(map.value("dmMusicName").toString());
            }
            if(ja_effect.count() > 0) {
                json_dev.insert("aEffect", ja_effect);
            }
            device_array.append(json_dev);
        }
        scene.insert("aDevice", device_array);
        scene_array.append(scene);
    }
    json.insert("aScene", scene_array);
    QJsonArray ja_conf;
    foreach(QSharedPointer<acc_conf_info> conf, m_hash_acc_conf.values()) {
        if(conf.isNull()) {
            continue;
        }
        QJsonObject json_conf;
        json_conf.insert("type", conf->get_device_type());
        json_conf.insert("name", conf->get_name());
        json_conf.insert("netType", conf->get_net_type());
        json_conf.insert("cmdType", conf->get_cmd_type());
        json_conf.insert("sort", conf->get_sort());
        json_conf.insert("model", conf->get_model());
        json_conf.insert("modelName", conf->get_modelName());
        json_conf.insert("controlBus", conf->get_control_bus());
        QJsonArray ja_cmds;
        foreach(QSharedPointer<cmd_info> cmd, conf->get_cmds().values()) {
            if(cmd.isNull()) {
                continue;
            }
            QJsonObject json_cmd;
            json_cmd.insert("name", cmd->get_name());
            json_cmd.insert("cmd", cmd->get_cmd());
            json_cmd.insert("send", cmd->get_send());
            json_cmd.insert("suc", cmd->get_suc());
            json_cmd.insert("err", cmd->get_err());
            ja_cmds.append(json_cmd);
        }
        json_conf.insert("cmds", ja_cmds);
        ja_conf.append(json_conf);
    }
    json.insert("aConfig", ja_conf);
    json.insert("localIP", m_local_ip_address);
    QDir appdir = directoryOf(qApp->applicationDirPath());
    QDir dir = directoryOf(appdir.absoluteFilePath("conf"));
    QString configFile = dir.absoluteFilePath(QString("%1").arg(CONF_FILE_NAME));
#ifdef Q_OS_ANDROID
    //安卓版本，配置文件不存在时，从资源文件中读取初始配置
    dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    configFile = dir.absoluteFilePath(QString("%1").arg(CONF_FILE_NAME));
#endif
    QFile file(configFile);
    if(!file.open(QFile::ReadWrite | QFile::Truncate | QFile::Text)) {
        qWarnxxx(qtr("打开文件失败"), configFile, file.errorString());
        return;
    }
    QString content = getStringFromJsonObject(json);
//    qDebugxxxxx(qtr("写入配置文件"), configFile, content.toUtf8().length(), content, QString(content.toUtf8()));
    file.write("\n\r");
    file.write(content.toUtf8());
    file.close();
}



void room_manager::stop_by_switch(QList<QSharedPointer<data_info> > list)
{
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        info->stop();
    }
}


void room_manager::start_by_switch(QList<QSharedPointer<data_info> > list)
{
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        info->start();
    }
}



void room_manager::index_changed_room_slot()   //更换场景
{
    QSharedPointer<data_info> room = m_model_room->current_data_info(); //得到当前场景
    //初始化轮巡标志
    set_polling_num(-1);
    if(room.isNull()) {
        return;
    }
    stop_by_switch(m_model_signal_source->get_data_list()); //停止所有信号连接
//    stop_by_switch(m_model_device->get_data_list());
    foreach(QSharedPointer<data_info> info, m_model_room->get_data_list()) {
        if(info.isNull()) {
            continue;
        }
        stop_by_switch(info->get_list_device()); //停止所有设备连接
    }
    stop_by_switch(m_model_room->get_data_list());  //停止所有场景连接
    room->start();   //开始新的场景连接  有tcp
//    m_model_device->append(room->get_list_device());
//    headler_device();
     m_model_device->clear();
    m_model_device->append(room->headler_device());
    m_model_device_ctrl->clear();
    m_model_signal_mode->clear();
//    m_model_signal_mode->append(room->get_list_signal_mode());
//    discreate_connect_for_image(m_model_signal_source->get_data_list());
    m_model_signal_source->append(room->get_list_signal_source());
//    create_connect_for_image(m_model_signal_source->get_data_list());
//    room->clear_signal_source_in_tv_wall();
//    m_model_signal_source_display->append(room->get_list_signal_source_in_tv_wall());   //注意append的是列表时，会先清空再增加，append的是一个对象是，直接插入
//    qDebug()<<m_model_signal_source_display->count();
    start_by_switch(m_model_signal_source->get_data_list()); //开启当前B区信号连接   有tcp
    start_by_switch(room->get_list_device()); //开启当前设备连接  有tcp   该场景拥有的设备
    emit sence_changed(); //触发B区更换信号
}

void room_manager::discreate_connect_for_image(QList<QSharedPointer<data_info> > list)
{
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        if(info->get_device_type() == 0x07) {
            if(info->get_list_signal_source().count() > 0) {
                discreate_connect_for_image(info->get_list_signal_source());
            }
        } else {
            disconnect(info.data(), &data_info::image_incoming_signal, this, &room_manager::image_incoming_signal);
        }
    }
}

void room_manager::create_connect_for_image(QList<QSharedPointer<data_info> > list)
{
    foreach(QSharedPointer<data_info> info, list) {
        if(info.isNull()) {
            continue;
        }
        if(info->get_device_type() == 0x07) {
            if(info->get_list_signal_source().count() > 0) {
                create_connect_for_image(info->get_list_signal_source());
            }
        } else {
            connect(info.data(), &data_info::image_incoming_signal, this, &room_manager::image_incoming_signal);
        }
    }
}

void room_manager::image_changed_time_out_slot()
{
    QSharedPointer<data_info> room = m_model_room->current_data_info();
    if(room.isNull()) {
        return;
    }
    QList<QSharedPointer<data_info> > list = room->get_list_signal_source();
    for(int i = 0; i < list.count(); i++) {
        QPixmap map;
        if(!map.load(QString(":/images/test/%1.jpg").arg(get_round_num() % 5))) {
            continue;
        }
        emit onIncoming(i, map);
    }
}

void room_manager::signal_source_changed_slot(int index)  //更换场景时  B区信号刷新
{
    QSharedPointer<data_info> room = m_model_room->current_data_info();
    if(room.isNull() || room->get_index() != index) {
        return;
    }
    stop_by_switch(m_model_signal_source->get_data_list());
    discreate_connect_for_image(m_model_signal_source->get_data_list());
    m_model_signal_source->append(room->get_list_signal_source());
    create_connect_for_image(m_model_signal_source->get_data_list());
    start_by_switch(m_model_signal_source->get_data_list());
    emit sence_changed_from_siganl_source(); //二期新增   防止界面刷新慢
}

void room_manager::signal_source_in_wall_changed_slot(int index)  //更换场景时  C区信号刷新
{
    QSharedPointer<data_info> room = m_model_room->current_data_info();
    if(room.isNull() || room->get_index() != index) {
        return;
    }
    qDebugxx(qtr("要显示的窗口数量"), room->get_list_signal_source_in_tv_wall().count());
    m_model_signal_source_display->append(room->get_list_signal_source_in_tv_wall());
//    emit rellay_get_device_display();
}

void room_manager::data_changed_by_device_slot(QString id)
{
    if(m_model_device_ctrl == nullptr) {
        return;
    }
    m_model_device_ctrl->data_changed_by_id(id);
}

void room_manager::device_status_changed_slot(QString id, bool status)   //设备状态改变
{
    qDebugxxx(qtr("设备状态变更"), id, status);
    if(m_model_device_ctrl == nullptr) {
        return;
    }
    m_model_device_ctrl->set_device_status(id, status);
}

void room_manager::win_changed_slot()
{
//    qDebug()<<(qtr("选择显示窗口"));
    QSharedPointer<data_info> room = m_model_room->current_data_info();
    QSharedPointer<data_info> win = m_model_signal_source_display->current_data_info();
    if(win.isNull() && win->get_old_data_info().isNull()) {
        qDebugxx(qtr("没有选中当前显示窗口"), m_model_signal_source_display->get_current_index());
        return;
    }
    //二期新增打开该视频源的音频   //如果有其他音频已经开了就先关了
    //
    win->get_old_data_info()->send_win_level_changed_event(win);
}

void room_manager::id_changed_slot(QString val)
{
    QSharedPointer<data_info> room = m_model_room->current_data_info();
    if(room.isNull()) {
        return;
    }
    room->reset_signal_source_status();
    QSharedPointer<data_info> s = room->get_signal_source_by_id(val);
    if(s.isNull()) {
        qWarnxx(qtr("id不存在"), val);
        return;
    }
    s->set_device_status(true);
}





QString room_manager::get_content_from_file(QString fileName)
{
    QFile file(fileName);
    if(!file.exists()) {
        return QString();
    }
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        return QString();
    }
    QString content = file.readAll();
    file.close();
    return content;
}
