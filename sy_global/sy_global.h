#ifndef SY_GLOBAL_H
#define SY_GLOBAL_H

#include <QString>


//错误码
const int code_qpid_sucess                  = 0;
const int code_qpid_no_exists_user          = 1;
const int code_qpid_no_exists_camera        = 2;
const int code_qpid_low_priority            = 3;
const int code_qpid_unknown_conf_storage    = 4;
const int code_qpid_unknown_conf_playback   = 5;
const int code_qpid_exists_storage          = 6;//存储已经存在
const int code_qpid_wrong_param             = 7;//参数错误
const int code_qpid_no_exists_storage       = 8;//录像不存在
const int code_qpid_unsupport_command       = 9;//不支持的命令
const int code_qpid_unknown_conf_real       = 10;
const int code_qpid_unknown_service_ptz     = 11;
const int code_qpid_exists_real             = 12;//实时播放已经存在
const int code_qpid_port_too_mall           = 13;//端口太少
const int code_qpid_time_out                = 14;//连接超时
const int code_qpid_failed_to_connect       = 15;//连接失败
const int code_qpid_no_exists_real             = 16;//实时播放已经不存在
const int code_qpid_no_service_stream             = 17;//流服务未启动
const int code_qpid_disconnect_stream             = 18;//流中断
const int code_qpid_no_service_hik             = 19;//海康服务未启动
//const int code_qpid_ptz_local_gbt             = 20;//本级国标云台控制
//const int code_qpid_ptz_child_gbt             = 21;//下级国标云台控制
const int code_qpid_local_gbt             = 20;//本级国标设备
const int code_qpid_child_gbt             = 21;//下级国标设备
const int code_qpid_no_exists_playback             = 22;//录像播放已经不存在
const int code_qpid_no_exists_service_storage       = 23;//存储服务不存在
const int code_qpid_no_exists_service_playback             = 24;//回放服务不存在
const int code_qpid_no_exists_service_real             = 25;//实时服务不存在
const int code_qpid_no_exists_service_law             = 26;//实时服务不存在
const int code_qpid_disconnect_service             = 27;//服务中断
const int code_qpid_wrong_system             = 28;//系统故障
const int code_qpid_reuse_real             = 29;//复用实时连接
const int code_qpid_exists_playback          = 30;//存储已经存在
const int code_qpid_failed_to_operate_db       = 31;//操作数据库失败
const int code_qpid_no_disks_for_camera             = 32;//没有为摄像机分配磁盘
const int code_qpid_storage_num_over             = 33;//存储路数超过限制
const int code_qpid_real_num_over             = 34;//实时连接路数超过限制
const int code_qpid_access_camera_num_over             = 35;//接入摄像机路数超过限制
const int code_qpid_full_disks_for_storage             = 36;//磁盘空间满
const int code_qpid_disks_lost             = 37;//磁盘丢失
const int code_qpid_no_disks_for_storage             = 38;//无可用磁盘，不能读写，或者磁盘丢失
const int code_qpid_device_offline_alarm             = 39;//设备离线报警
const int code_qpid_failed_to_login             = 40;//登录失败
const int code_qpid_no_disks_for_backup             = 41;//没有配置备份磁盘
const int code_qpid_full_disks_for_backup             = 42;//备份磁盘空间满
const int code_qpid_no_disks_for_storage_backup             = 43;//无可用备份磁盘，不能读写，或者磁盘丢失
const int code_qpid_reuse_client_port             = 44;//客户端端口重用
const int code_qpid_unsupport_ptz             = 45;//不支持的云台协议
const int code_qpid_unsupport_device             = 46;//不支持的设备


const int code_rtsp_ok                      = 200;
const int code_rtsp_wrong_request           = 400;
const int code_rtsp_unauthorized            = 401;
const int code_rtsp_auth_failed             = 403;
const int code_rtsp_no_resource             = 404;
const int code_rtsp_no_accept             = 406;

//状态值
const int status_server_ok                  = 0;
const int status_server_ready               = 1;
const int status_server_readied             = 2;


//设备类型
const int device_type_rtsp                   = 0;
const int device_type_dvr                   = 1;
const int device_type_gbt                   = 2;

const int command_code_user_login          = 1;
const int command_code_user_logout          = 2;
const int command_code_operate_db          = 3;
const int command_code_connected_to_server          = 4;

const int command_code_ptz_control          = 10001;
const int command_code_start_real           = 10002;
const int command_code_start_playback       = 10003;
const int command_code_start_storage        = 10004;
const int command_code_stop_storage         = 10005;
const int command_code_stream_time_out      = 10006;
const int command_code_stop_real            = 10007;

const int command_code_stop_playback       = 10009;
const int command_code_gateway_service_status             = 10011;
const int command_code_storage_service_status             = 10012;
const int command_code_record_status             = 10013;
const int command_code_record_control             = 10014;
const int command_code_car_snap             = 10015;
const int command_code_car_alarm             = 10016;
const int command_code_law_gps_upload             = 10017;
const int command_code_download             = 10018;
const int command_code_stream_register_gateway             = 10019;
const int command_code_notify_gis             = 10020;
const int command_code_query_gis             = 10021;
const int command_code_stop_real_by_camera            = 10022;
const int command_code_search_catalog_notify            = 10023;
const int command_code_law_service_status             = 10024;
const int command_code_query_disks_for_storage_service             = 10025;
const int command_code_package_lost_storage             = 10026;
const int command_code_package_lost_gateway             = 10027;
const int command_code_query_channel_for_car             = 10028;
const int command_code_start_real_mobile_upload           = 10029;
const int command_code_stop_real_mobile_upload           = 10030;
const int command_code_start_real_mobile           = 10031;
const int command_code_start_playback_mobile       = 10032;
const int command_code_search_stream_name       = 10033;
const int command_code_search_record_child_platform       = 10034;//下级平台录像查询
const int command_code_search_directory_child_platform       = 10035;//下级平台目录查询
const int command_code_search_ptz_pos       = 10036;
const int command_code_search_channel_name       = 10037;
const int command_code_search_channel_online       = 10038;
const int command_code_notify_channel_online       = 10039;
const int command_code_alarm_device_offline       = 10040;
const int command_code_notify_storage_disk_update       = 10041;
const int command_code_call_video_to_screen       = 10042;
const int command_code_stop_video_to_screen       = 10043;
const int command_code_search_record_file       = 10044;//20180403 录像查询
const int command_code_device_alarm       = 10045;
const int command_code_device_restore_by_manu       = 10046;//手动恢复N+1
const int command_code_device_switch_to_backup       = 10047;//主服务器主动要求切换到备份服务器，主要是针对磁盘全故障的情况
const int command_code_device_switch_to_stop       = 10048;//停止备份服务器的备份工作
const int command_code_device_restore_by_auto       = 10049;//存储服务检查磁盘恢复之后，自动请求恢复N+1
const int command_code_record_control_scale             = 10050;
const int command_code_record_control_relocate             = 10051;
const int command_code_record_control_pause             = 10052;
const int command_code_record_control_restart             = 10053;


//qpid与各内部服务进行通信的指令
const int command_code_heartbeat            = 20001;
const int command_code_playbacked           = 20002;
const int command_code_storaged             = 20003;
const int command_code_register_storage     = 20004;
const int command_code_register_playback    = 20005;
const int command_code_register_real        = 20006;
const int command_code_register_ptz        = 20008;
const int command_code_register_car        = 20009;
const int command_code_register_gis_hik        = 20010;
const int command_code_register_gbt_gateway        = 20011;
const int command_code_stop_real_from_rtsp            = 20012;
const int command_code_gbt_stream_status      = 20021;
const int command_code_heartbeat_storage            = 20022;
const int command_code_ptz_camera_update            = 20023;
const int command_code_heartbeat_real            = 20024;
const int command_code_heartbeat_ptz            = 20025;
const int command_code_start_real_stream           = 20026;
const int command_code_stop_real_stream           = 20027;
const int command_code_login_hik           = 20028;
const int command_code_print_service_status           = 20029;
const int command_code_stop_real_stream_timeout           = 20030;

const int command_code_notify_irrigation_weather_station_status_to_user           = 30001;
const int command_code_notify_irrigation_soil_moisture_status_to_user           = 30002;
const int command_code_irrigation_control_solenoid_valve           = 30003;//电磁阀控制
const int command_code_irrigation_get_solenoid_valve_status           = 30004;//获取电磁阀状态

const int command_code_notify_irrigation_bengfang_status_to_user           = 30006;
const int command_code_notify_irrigation_gaoweishuichi_status_to_user           = 30005;
const int command_code_notify_irrigation_relay_status_to_user           = 30007;
const int command_code_alarm_irrigation_offline_status_to_user           = 30008;

const int command_code_notify_irrigation_status           = 30001;

const int service_type_gateway_rtsp         = 1;//rtsp网关的服务类型编号
const int service_type_gateway_gbt_to_rtsp  = 2;
const int service_type_gateway_rtsp_to_gbt  = 3;
const int service_type_gbt_sip              = 4;


const int way_play_single                   = 0; //单进程回放
const int way_play_multi                    = 1; //多进程回放
const int way_storage_rtsp_single           = 0; //单进程存储
const int way_storage_rtsp_multi            = 1; //多进程存储
const int way_real_single                   = 0; //单进程实时
const int way_real_multi                    = 1; //多进程实时



const int gbt_request_type_coming              = 1;
const int gbt_request_type_outing              = 2;

const QString qpid_datetime_format             ="yyyy-MM-dd hh:mm:ss";
const QString qpid_datetime_format_file_name   ="yyyyMMddhhmmss";
const QString qpid_datetime_format_ex             ="yyyy-M-d h:m:s";
const QString qpid_datetime_format_file_name_cvss   ="yyyyMMddhh";
const QString qpid_datetime_format_file_name_hik   ="yyyyMMddthhmmssz";
const QString qpid_datetime_format_video_frame             ="yyyy-MM-dd hh:mm:ss zzz";


const QString restful_url_get_camera_by_user            = "getCamerabyUser";
const QString restful_url_query_record_by_gbt            = "LuXiangJiLuXiangChaXunGuoBiaoBianHao";
const QString restful_url_get_catalog_for_parent            = "HuoQuTongDaoMuLuShuShangChuanGeiShangJi";
const QString restful_url_get_high_point_all            = "HuoQuGaoDianLieBiaoDaoWangGuan";
const QString restful_url_get_camera_info_by_gb_code    = "HuoQuGuoBiaoSheXiangJiXinXiGuoBiaoBianHao";
const QString restful_url_get_camera_info_for_playback_by_gb_code    = "HuoQuGuoBiaoSheXiangJiLuXiangHuiFangGuoBiaoBianHao";
const QString restful_url_get_user_info_by_user_id   = "HuoQuYongHuXinXiYongHuBianHao";
const QString restful_url_get_user_info_by_user_name   = "HuoQuYongHuXinXiYongHuMingChen";
const QString restful_url_get_disks_info_by_gb_code    = "HuoQuSheXiangJiCiPanXinXiSheXiangJiBianHao";
const QString restful_url_get_platform_parent_info    = "HuoQuShangJiPingTaiXinXi";
const QString restful_url_get_platform_local_info    = "HuoQuBenJiPingTaiXinXi";
const QString restful_url_get_directory_channel_info    = "HuoQuTongDaoMuLuXinXi";
const QString restful_url_get_directory_device_info    = "HuoQuSheBeiMuLuXinXi";
const QString restful_url_record_file_create    = "XinJianLuXiangWenJianJiLu";
const QString restful_url_record_file_update    = "GengXinLuXiangWenJianJiLu";
const QString restful_url_record_plan    = "HuoQuCunChuJiHuaXinXiWangGuan";
const QString restful_url_record_plan_disks    = "HuoQuCunChuJiHuaCiPan";
const QString restful_url_record_plan_disks_by_camera_id    = "HuoQuCunChuJiHuaCiPanSheXiangJiBianHao";
const QString restful_url_get_camera_ptz    = "HuoQuFuWuGuanLiDeQiuJiXinXiFuWuDiZhiFuWuDuanKou";
const QString restful_url_get_delete_cycle    = "HuoQuXunHuanShanChuGuiZeFuWuIPFuWuDuanKou";
const QString restful_url_delete_record_file_name    = "ShanChuLuXiangWenJianJiLu";
const QString restful_url_get_user_by_user_name    = "HuoQuYongHuDengLuXinXiYongHuMing";
const QString restful_url_get_camera_info_by_camera_id    = "HuoQuSheXiangJiXinXiSheXiangJiBianHao";
const QString restful_url_get_camera_list           = "HuoQuSuoYouSheXiangJiLieBiaoKeHuDuan";
const QString restful_url_get_camera_info_for_storage_by_camera_id    = "HuoQuCunChuSheXiangJiXinXiSheXiangJiBianHao";
const QString restful_url_get_camera_info_for_storage_by_gbt_code    = "HuoQuCunChuSheXiangJiXinXiGuoBiaoBianHao";
const QString restful_url_get_storage_info_by_camera_id    = "HuoQuCunChuFuWuXinXiSheXiangJiBianHao";
const QString restful_url_get_car_system_list    = "HuoQuCheLiangJianCeXiTongLieBiao";
const QString restful_url_get_device_id_by_gb_code    = "HuoQuSheBeiZhuJianGuoBiaoBianHao";
const QString restful_url_get_platform_id_by_gb_code    = "HuoQuPingTaiZhuJianGuoBiaoBianHao";
const QString restful_url_get_platform_service_id_by_gb_code    = "HuoQuPingTaiFuWuZhuJianPingTaiZhuJian";
const QString restful_url_insert_device_simple    = "ChaRuSheBeiXinXiZhuJianGuoBiaoBianHao";
const QString restful_url_get_channel_id_by_gb_code    = "HuoQuTongDaoXinXiZhuJianGuoBiaoBianHao";
const QString restful_url_update_device_info    = "GengXinSheBeiXinXiMuLuGengXinSheBeiBianHao";
const QString restful_url_update_channel_info    = "GengXinTongDaoXinXiGuoBiaoWangGuan";
const QString restful_url_insert_channel_info    = "ChaRuTongDaoXinXiGuoBiaoWangGuan";
const QString restful_url_update_catalog_info    = "GengXinGuoBiaoShuMuLuGuoBiaoWangGuan";
const QString restful_url_insert_catalog_info    = "ChaRuGuoBiaoShuMuLuGuoBiaoWangGuan";
const QString restful_url_get_catalog_id_by_civil_code    = "HuoQuQuYuZhuJianQuYuBianHao";
const QString restful_url_get_catalog_root    = "HuoQuMuLuGenJieDian";
const QString restful_url_get_platform_info_by_gbt_code    = "HuoQuPingTaiXinXiPingTaiGuoBiaoBianHao";
const QString restful_url_get_device_list    = "HuoQuSheBeiLieBiao";
const QString restful_url_get_storage_disks    = "HuoQuCunChuFuWuCiPanXinXiFuWuIPDiZhiFuWuDuanKou";//20180216
const QString restful_url_get_storage_disk_update    = "GengXinCunChuFuWuCiPanXinXi";//20180216
const QString restful_url_get_storage_disk_insert    = "ChaRuCunChuFuWuCiPanXinXi";//20180216
const QString restful_url_get_service_id_by_ip_port    = "HuoQuFuWuZhuJianFuWuIPDiZhiFuWuDuanKou";//20180216
const QString restful_url_get_storage_disks_by_service_id    = "HuoQuCunChuFuWuCiPanXinXiFuWuID";//20180216

const QString restful_url_get_weather_station_info    = "HuoQuQiXiangZhanXinXiQiXiangZhanDiZhi";//20180320
const QString restful_url_get_irrigation_device_type_info    = "HuoQuSheBeiLeiXingXinXiYingJianSheBeiBianHao";//20180320
const QString restful_url_get_irrigation_channel_info    = "HuoQuTongDaoIDGuanGaiSheBeiBianHao";//20180320
const QString restful_url_update_weather_station_info_current    = "GengXinQiXiangZhanDangQianXinXiSheBeiID";//20180320
const QString restful_url_insert_weather_station_info_history    = "XieRuQiXiangZhanLiShiXinXi";//20180320
const QString restful_url_update_soil_moisture_info_current    = "GengXinTuRangShangQingDangQianXinXiSheBeiID";//20180320
const QString restful_url_insert_soil_moisture_info_history    = "XieRuTuRangShangQingLiShiXinXiSheBeiID";//20180320
const QString restful_url_get_high_pool_list    = "HuoQuGaoWeiShuiChiLieBiao";//20180320
const QString restful_url_get_high_pool_info_current    = "HuoQuGaoWeiShuiChiDeDangQianZhiGaoWeiShuiChiID";//20180320
const QString restful_url_update_high_pool_info_current    = "GengXinGaoWeiShuiChiDangQianZhuangTaiSheBeiID";//20180320
const QString restful_url_insert_high_pool_info_history    = "XieRuGaoWeiShuiChiLiShiXinXiSheBeiID";//20180320
const QString restful_url_update_pump_station_info_current    = "GengXinBengFangDangQianZhuangTaiSheBeiID";//20180320
const QString restful_url_insert_pump_station_info_history    = "XieRuBengFangLiShiZhuangTaiSheBeiID";//20180320
const QString restful_url_get_pump_station_info    = "HuoQuShuiFeiJiBengZhanDianCiFa";//20180321
const QString restful_url_update_irrigation_info_current    = "GengXinShuiFeiJiDangQianZhuangTaiSheBeiID";//20180320
const QString restful_url_insert_irrigation_info_history    = "XieRuShuiFeiJiLiShiXinXiSheBeiID";//20180320
const QString restful_url_get_monitor_by_channel_id    = "HuoQuJieMaQiXinXiJieMaQiTongDaoBianHao";//20180320
const QString restful_url_update_relay_status_current    = "GengXinDianCiFaZhuangTaiTongDaoIDZhuangTai";//20180320
const QString restful_url_get_channel_service_info    = "HuoQuTongDaoFuWuTongDaoID";//20180320
const QString restful_url_alarm_irrigation_offline    = "ShuiLiSheBeiLiXianBaoJingSheBeiBianHao";//20180320
const QString restful_url_insert_pump_init    = "XieRuChouShuiBengKaiQiShiDeChuShiLeiJiLiuLiangZhi";//20180320
const QString restful_url_insert_pump_water_flow    = "XieRuBengZhanDangCiKaiGuanShuiDeLiuLiang";//20180320
const QString restful_url_insert_shuifeiji_water_flow    = "XieRuShuiFeiJiDangCiKaiGuanShuiDeLiuLiang";//20180320
const QString restful_url_get_pump_init    = "ChaXunLiuLiangJiDeChuShiZhiSheBeiBianHao";//20180320
const QString restful_url_get_disks_name_by_service    = "HuoQuBenFuWuDeCiPanFuWuipFuWuDuanKou";
const QString restful_url_get_service_of_storage_by_main    = "HuoQuCunChuZhuFuWu";
const QString restful_url_get_service_of_storage_by_backup    = "HuoQuCunChuBeiFenFuWu";


const int send_buffer_frames    = 30;

const int max_ptz_speed = 255.0;

//超时时间定义，单位：秒
const int time_out_service      = 60; //服务超时
const int time_out_ptz_control  = 60;
const int time_out_connect_wait      = 60;
const int time_out_gis_query      = 30; //服务超时
const int time_out_record_file      = 30;
const int time_out_record_reconnect      = 30;//存储重连时间
const int time_out_record_plan_interval      = 120;//存储计划的执行间隔
const int time_out_data_update      = 60;//更新数据的时间间隔
const int time_out_heatbeat      = 10;
const int time_out_connect      = time_out_heatbeat * 3;
const int time_out_delete_cycle      = 1800;//循环删除的时间间隔

//接入服务类型
const QString service_type_shebeifuwu = "SheBeiFuWu";
const QString service_type_avtfuwu = "avtFuWu";
const QString service_type_anweisifuwu = "AnWeiSiSheBeiFuWu";
const QString service_type_cunchufuwu = "CunChuFuWu";
const QString service_type_hai_kang    = "HaiKang";//20180320
const QString service_type_hai_kang_rtsp    = "HaiKangRtsp";
const QString service_type_gbt    = "gbt";//20180320

const QString device_type_rtsp_ipc = "rtspipc";
const QString device_type_gbt_virtual = "gbt28181";
const QString device_type_hai_kang = "haikang";
const QString device_type_da_hua = "dahua";
const QString device_type_avtrace = "avtrace";
const QString device_type_yjd = "jyd";
const QString device_type_nkf = "nkf";
const QString device_type_uec_ex = "uec_ex";
const QString device_type_imos = "imos";

#define DTS_PER_SECOND 90000

const QString qpid_field_name_device_ip        									= "ipaddress";
const QString qpid_field_name_sn               									= "sn";
const QString qpid_field_name_region           									= "region";
const QString qpid_field_name_version          									= "version";
const QString qpid_field_name_channel_num      									= "channelnum";
const QString qpid_field_name_channel_list      									= "channels";
const QString qpid_field_name_disk             									= "disks";
const QString qpid_field_name_disk_name        									= "name";
const QString qpid_field_name_disk_total_size        									= "totalsize";
const QString qpid_field_name_disk_free_size        									= "freesize";
const QString qpid_field_name_disk_available_size        									= "availablesize";
const QString qpid_field_name_disk_id        									= "diskid";
const QString qpid_field_name_record_files        								= "files";
const QString qpid_field_name_device_port      									= "port";
const QString qpid_field_name_parent_id      									= "parentid";
const QString qpid_field_name_id      									= "id";
const QString qpid_field_name_stream_name      									= "mainstreamname";
const QString qpid_field_name_stream_name_aux      									= "aux_stream_name";
const QString qpid_field_name_stream_ip      									= "streamip";
const QString qpid_field_name_encoding_name      									= "encodingname";
const QString qpid_field_name_payload_type      									= "payloadtype";
const QString qpid_field_name_encoding_name_audio      									= "encodingnameaudio";
const QString qpid_field_name_payload_type_audio      									= "payloadtypeaudio";
const QString qpid_field_name_storage_ip      									= "storageip";
const QString qpid_field_name_user_name        									= "username";
const QString qpid_field_name_user_passwd      									= "userpasswd";
const QString qpid_field_name_user_name_device                                  = "deviceusername";
const QString qpid_field_name_multicast_ip                                  = "multicastip";
const QString qpid_field_name_user_passwd_device                                = "deviceuserpasswd";
const QString qpid_field_name_rtp_port_video   									= "rtpportvideo";
const QString qpid_field_name_rtp_port_audio   									= "rtpportaudio";
const QString qpid_field_name_rtp_port_server   								= "rtpportserver";
const QString qpid_field_name_rtp_port_client   								= "rtpportclient";
const QString qpid_field_name_cmd_type          								= "cmdtype";
const QString qpid_field_name_message_type      								= "messagetype";
const QString qpid_field_name_camera_id         								= "cameraid";
const QString qpid_field_name_camera_name         								= "cameraname";
const QString qpid_field_name_package_lost_current         								= "lostcurrent";
const QString qpid_field_name_package_lost_total         								= "losttotal";
const QString qpid_field_name_data_update_date         								= "updatedate";
const QString qpid_field_name_storage_type          								= "storagetype";

const QString qpid_field_name_user_id           								= "userid";
const QString qpid_field_name_uuid              								= "uuid";
const QString qpid_field_name_priority                                          = "priority";
const QString qpid_field_name_service_id        								= "serviceid";
const QString qpid_field_name_service_type        								= "servicetype";
const QString qpid_field_name_service_type_of_storage        								= "servicetypeofstorage";
const QString qpid_field_name_service_id_local        								= "serviceidlocal";
const QString qpid_field_name_uri               								= "uri";
const QString qpid_field_name_rtsp_port         								= "rtspport";//设备rtsp端口
const QString qpid_field_name_rtsp_port_num         								= "rtspportcount";//设备rtsp端口
const QString qpid_field_name_rtsp_port_stream         								= "rtspportstream";//流服务的rtsp端口
const QString qpid_field_name_record_file       								= "recordfile";
const QString qpid_field_name_record_file_status                                = "isnew";
const QString qpid_field_name_playback_start_time                               = "starttime";
const QString qpid_field_name_playback_end_time 								= "endtime";
const QString qpid_field_name_playback_record_source 								= "recordsource";
const QString qpid_field_name_scale                                             = "scale";
const QString qpid_field_name_service_ip                                        = "serviceip";
const QString qpid_field_name_service_port                                      = "serviceport";
const QString qpid_field_name_service_ip_real                                        = "realserviceip";
const QString qpid_field_name_service_port_real                                      = "realserviceport";
const QString qpid_field_name_service_id_real        								= "realserviceid";
const QString qpid_field_name_service_ip_storage                                        = "storageserviceip";
const QString qpid_field_name_service_port_storage                                      = "storageserviceport";
const QString qpid_field_name_service_id_storage        								= "storageserviceid";
const QString qpid_field_name_service_type_storage        								= "storageservicetype";
const QString qpid_field_name_service_ip_local                                        = "serviceiplocal";//用于心跳上传时本机的服务ip地址
const QString qpid_field_name_service_port_local                                      = "serviceportlocal";//用于心跳上传时本机的服务端口
const QString qpid_field_name_service_name                                      = "servicename";
const QString qpid_field_name_process_id                                        = "processid";
const QString qpid_field_name_file_length                                       = "filelength";
const QString qpid_field_name_status                                            = "status";
const QString qpid_field_name_transmit_way                                            = "transmitway";
const QString qpid_field_name_status_type                                            = "statustype";
const QString qpid_field_name_start_way_of_storage                                            = "startwayofstorage";
const QString qpid_field_name_create_time                                            = "createtime";
const QString qpid_field_name_dest_ip                                            = "destip";
const QString qpid_field_name_dest_port                                            = "destport";
const QString qpid_field_name_online                                           = "online";
const QString qpid_field_name_delete_cycle_type                                           = "cycletype";
const QString qpid_field_name_delete_cycle_value                                           = "cyclevalue";
const QString qpid_field_name_remarks                                            = "remarks";
const QString qpid_field_name_access_way                                            = "accessway";

const QString qpid_field_name_time              								= "time";
const QString qpid_field_name_single_process                                    = "singleprocess";
const QString qpid_field_name_ssrc              								= "ssrc";
const QString qpid_field_name_invite_type              								= "invitetype";
const QString qpid_field_name_ptz_cmd                                           = "cmd";
const QString qpid_field_name_ptz_speed1                                        = "speed1";
const QString qpid_field_name_ptz_speed2                                        = "speed2";
const QString qpid_field_name_ptz_param1                                        = "param1";
const QString qpid_field_name_ptz_param2                                        = "param2";
const QString qpid_field_name_ptz_status                                        = "status";
const QString qpid_field_name_result                                            = "result";
const QString qpid_field_name_result_desc                                       = "resultdesc";
const QString qpid_field_name_gis              									= "gis";
const QString qpid_field_name_gis_dwRelativeTime                                = "dwrelativetime";
const QString qpid_field_name_gis_dwabstime                                     = "dwabstime";
const QString qpid_field_name_gis_fazimuth                                      = "fazimuth";
const QString qpid_field_name_gis_bylatitudetype                                = "bylatitudetype";

const QString qpid_field_name_gis_bylongitudetype                               = "bylongitudetype";
const QString qpid_field_name_gis_fhorizontalvalue                              = "fhorizontalvalue";
const QString qpid_field_name_gis_fverticalvalue                                = "fverticalvalue";
const QString qpid_field_name_gis_fvisibleradius                                = "fvisibleradius";
const QString qpid_field_name_gis_fmaxviewradius                                = "fmaxviewradius";
const QString qpid_field_name_gis_devinfo                                       = "devinfo";

const QString qpid_field_name_gis_devip                                         = "devip";
const QString qpid_field_name_gis_wport                                         = "wport";
const QString qpid_field_name_gis_bychannel                                     = "bychannel";
const QString qpid_field_name_gis_byivmschannel                                 = "byivmschannel";
const QString qpid_field_name_gis_latitude                                      = "latitude";
const QString qpid_field_name_gis_fsec                                          = "fsec";
const QString qpid_field_name_gis_bydegree                                      = "bydegree";
const QString qpid_field_name_gis_byminute                                      = "byminute";
const QString qpid_field_name_gis_longitude                                     = "longitude";
const QString qpid_field_name_gis_sensorparam                                   = "sensorparam";
const QString qpid_field_name_gis_bysensortype                                  = "bysensortype";
const QString qpid_field_name_gis_fhorwidth                                     = "fhorwidth";
const QString qpid_field_name_gis_fverwidth                                     = "fverwidth";
const QString qpid_field_name_gis_ptzpos                                        = "ptzpos";
const QString qpid_field_name_gis_fpanpos                                       = "fpanpos";
const QString qpid_field_name_gis_ftiltpos                                      = "ftiltpos";
const QString qpid_field_name_gis_fzoompos                                      = "fzoompos";
const QString qpid_field_name_setup_alarm_handle                                = "handle";
const QString qpid_field_name_gbt_request_type                                  = "requesttype";
const QString qpid_field_name_gbt_run_nums                                      = "runnums";
const QString qpid_field_name_gbt_id_device                                     = "devicegbcode";
const QString qpid_field_name_id_device                                         = "deviceid";
const QString qpid_field_name_gbt_id_channel                                    = "channelgbcode";
const QString qpid_field_name_gbt_platform_type                                 = "platformtype";
const QString qpid_field_name_gbt_payload_type                                  = "payloadtype";
const QString qpid_field_name_gbt_local                                         = "localgbt";
const QString qpid_field_name_gbt_platform_code                                 = "platformcode";
const QString qpid_field_name_gbt_platform_domain                               = "platformdomain";
const QString qpid_field_name_gbt_platform_user_passwd                          = "platformuserpasswd";
const QString qpid_field_name_gbt_platform_user_name                            = "platformusername";
const QString qpid_field_name_gbt_platform_ip_address                            = "platformipaddress";
const QString qpid_field_name_gbt_media_send_type                               = "mediasendtype";
const QString qpid_field_name_gbt_platform_port                            = "platformport";
const QString qpid_field_name_operate_code                                   = "code";
const QString qpid_field_name_operate_params                                   = "params";
const QString qpid_field_name_operate_datas                                   = "datas";
const QString qpid_field_name_civil_code                                   = "CivilCode";
const QString qpid_field_name_device_type                                         = "devicetype";


typedef struct
{
    uint32_t ip;
    int32_t channel;
    int64_t timeYear:16;
    int64_t timeMonth:8;
    int64_t timeDay:8;
    int64_t timeHour:8;
    int64_t timeMinute:8;
    int64_t timeSecond:8;	//64bit
    int64_t playLoadType:8;
    int16_t x;
    int16_t y;
    uint32_t gop:16;
    uint32_t rate:16;
    uint32_t unused;
} sy_tag_file_header;


typedef struct
{
    uint64_t offset;
    uint64_t timestamp:56;
    uint64_t frametype:8;
} sy_tag_video_index;

typedef struct {
    QString host;
    QString address;
    QString username;
    QString password;
    bool isAuth;
    QString queue;//回复用的queue，当这个不为空时，为异步回复的临时队列
} BusInfo;

#define MSG_HEAD 0xFFFFFFFF
typedef struct{
    unsigned int flag;
    int len;
}sy_tag_rpc_header;


#define MOBILE_DATA_HEAD 0x24
#pragma pack(push, 1)
typedef struct{
    unsigned char flag;
    unsigned char channel_num;
    unsigned short len;
}sy_tag_mobile_header;


typedef struct{
    unsigned char addr;
    unsigned char flag;
    unsigned short cmd;
    unsigned short len;
}sy_tag_irrigation_weather_station_header;

typedef struct{
    unsigned char addr;
    unsigned char flag;
    unsigned short len;
}sy_tag_irrigation_weather_station_header_return;

typedef struct{
    unsigned char addr;
    unsigned char flag;
}sy_tag_irrigation_weather_station_header_flag;


typedef struct{
    unsigned char id[6];
    unsigned char cmd;
}sy_tag_irrigation_header_flag;


enum sy_enum_irrigation_protocol_type
{
    PROTOCOL_TYPE_XPH,
    PROTOCOL_TYPE_PLATFORM
};

typedef struct
{
    unsigned int	steam_id;
    ///////////////////////////////////
    unsigned short	pes_len;

    unsigned char	original_copy:1;
    unsigned char	copyright:1;
    unsigned char	data_align:1;
    unsigned char	pes_priority:1;
    unsigned char	pes_scram:2;
    unsigned char	flag:2;

    unsigned char	pes_extension_flag:1;
    unsigned char	pes_crc_flag:1;
    unsigned char	additional_copy_info_flag:1;
    unsigned char	dsm_trick_mode_flag:1;
    unsigned char	es_rate_flag:1;
    unsigned char	escr_flag:1;
    unsigned char	pts_dts_flags:2;
    ///////////////////////////////////
    unsigned char	pes_head_data_len;
}PES_HEAD_PLAYBACK,*PPES_HEAD_PLAYBACK;


typedef struct
{	// PTS
    unsigned char	marker0:1;
    unsigned char	pts4:3;
    unsigned char	pts_flag:4;

    unsigned char	pts3;
    unsigned char	marker1:1;
    unsigned char	pts2:7;

    unsigned char	pts1;
    unsigned char	marker2:1;
    unsigned char	pts0:7;
}PTS_PARAM_PLAYBACK,*PPTS_PARAM_PLAYBACK;


#pragma pack(pop)

//void sy_gb_rtp_msg(int msg_level, QString msg);

#endif // sy_GLOBAL_H
