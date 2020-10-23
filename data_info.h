#ifndef DATA_INFO_H
#define DATA_INFO_H

#include <QObject>
#include <QPixmap>
#include <QList>
#include <QImage>
#include <QVariantMap>
#include <QVariantList>
#include "cmd_info.h"
#define IPC_NEW_VERSION

class data_info : public QObject
{
    Q_OBJECT
public:
    explicit data_info(QObject *parent = nullptr);
    ~data_info();

    Q_PROPERTY(int phy_wall_row READ get_phy_wall_row WRITE set_phy_wall_row NOTIFY phy_wall_row_changed)
    Q_PROPERTY(int phy_wall_column READ get_phy_wall_column WRITE set_phy_wall_column NOTIFY phy_wall_column_changed)
    Q_PROPERTY(bool device_air_slider READ get_device_air_slider WRITE set_device_air_slider NOTIFY device_air_slider_changed)
    Q_PROPERTY(bool device_air_switch READ get_device_air_switch WRITE set_device_air_switch NOTIFY device_air_switch_changed)


    QString get_name();
    void set_name(QString val);

    int get_index();
    void set_index(int val);

    QString get_modelName();
    void set_modelName(QString val);
    QString get_model();
    void set_model(QString val);

    QString get_ico_path();
    void set_ico_path(QString val);

    QPixmap get_image();
    void set_image(const QPixmap &val);

    void append_signal_source(QSharedPointer<data_info> &val);
    void append_signal_source(QList<QSharedPointer<data_info>> &val);

    void append_signal_mode(QSharedPointer<data_info> &val);
    void append_signal_mode(QList<QSharedPointer<data_info>> &val);

    void append_device(QSharedPointer<data_info> &val);
    void append_device(QList<QSharedPointer<data_info>> &val);

    void append_device_in_e(QSharedPointer<data_info> &val);
    void append_device_in_e(QList<QSharedPointer<data_info>> &val);
    QList<QSharedPointer<data_info>> get_list_device_in_e();
    QList<QSharedPointer<data_info>> get_list_device();
    QList<QSharedPointer<data_info>> get_list_signal_source();
    QList<QSharedPointer<data_info>> get_list_signal_mode();
    QList<QSharedPointer<data_info>> get_list_signal_source_in_tv_wall();

    int get_phy_wall_row();
    void set_phy_wall_row(int val);

    int get_phy_wall_column();
    void set_phy_wall_column(int val);

    int get_logical_wall_row();
    void set_logical_wall_row(int val);

    int get_logical_wall_column();
    void set_logical_wall_column(int val);


    bool get_device_air_slider();
    void set_device_air_slider(bool val);

    bool get_device_air_switch();
    void set_device_air_switch(bool val);

    QString get_device_ctrl_top();
    void set_device_ctrl_top(QString val);

    QString get_device_ctrl_hor_left();
    void set_device_ctrl_hor_left(QString val);

    QString get_device_ctrl_hor_center();
    void set_device_ctrl_hor_center(QString val);

    QString get_device_ctrl_hor_right();
    void set_device_ctrl_hor_right(QString val);

    QString get_ctrl_progress();
    void set_ctrl_progress(QString val);

    int get_device_type();
    void set_device_type(int val);

    QString get_device_type_str();

    double get_display_width();
    void set_display_width(double val);

    double get_display_height();
    void set_display_height(double val);

    double get_display_x();
    void set_display_x(double val);

    double get_display_y();
    void set_display_y(double val);

    int get_wall_width();
    void set_wall_width(int val);

    int get_wall_height();
    void set_wall_height(int val);

    void set_num_grid(int val);

    bool get_device_status();
    void set_device_status(bool val);

    QString get_ip_address();
    void set_ip_address(QString val);

    QString get_local_ip_address();
    void set_local_ip_address(QString val);

    QString get_user_name();
    void set_user_name(QString val);

    QString get_user_pwd();
    void set_user_pwd(QString val);

    QString get_id();
    void set_id(QString val);

    unsigned int get_win_id();
    void set_win_id(unsigned int val);

    int get_port();
    void set_port(int val);

    virtual void start();
    virtual void stop();

    QVariantList get_music_buttons();
    void set_music_buttons(const QVariantList &val);
    QVariantList get_music_sliders();
    void set_music_sliders(const QVariantList &val);

    //for room
    virtual void get_signal_source();
    virtual void get_scence();
    virtual QSharedPointer<data_info> get_signal_source_by_id(const QString &id);
    void add_signal_source(QSharedPointer<data_info> val);

    void clear_signal_source_in_tv_wall();
    void add_signal_source_in_tv_wall(QSharedPointer<data_info> val);
    void add_signal_source_in_tv_wall(QList<QSharedPointer<data_info>> val);

    int get_channel_no();
    void set_channel_no(int val);

    int get_volume();
    void set_volume(int vol);

    int get_audio_id();
    void set_audio_id(int id);

    QString get_cmd_type();
    void set_cmd_type(QString val);
    int get_audio_channel();
    void set_audio_channel(int id);
    void send_signal_source_changed_event();
    void send_signal_source_in_wall_changed_event();
    void send_image_incoming_event(QImage img);

    void clone(QSharedPointer<data_info> &val);
    void set_old_data_info(QSharedPointer<data_info> val);
    QSharedPointer<data_info> get_old_data_info();
    void remove_mode(QSharedPointer<data_info> val);
    void remove_signal_source(QSharedPointer<data_info> val);

    void send_add_signal_source_to_wall_signal_event(QSharedPointer<data_info> val);
    void send_update_signal_source_to_wall_signal_event(QSharedPointer<data_info> val);
    void send_update_signal_source_to_wall_signal_by_old_event(QSharedPointer<data_info> val);
    void send_remove_signal_source_to_wall_signal_by_old_event(QSharedPointer<data_info> val);
    void send_remove_signal_source_to_wall_signal_event(QSharedPointer<data_info> val);
    void send_clear_window_signal_event();
    void send_device_ctrl_signal_event(const QString &cmd);
    void send_device_ctrl_signal_event(const QString &cmd, int channel);
    void send_remove_signal_source_signal_event(QSharedPointer<data_info> val);
    void send_add_signal_source_signal_event(QSharedPointer<data_info> val);

    void cloneEx(QSharedPointer<data_info> &val);

    int get_channel_flag(int index);
    void send_device_ctrl_signal_event(const QString &cmd, int channel, int param1);
    void send_device_ctrl_signal_event(int operate ,int action,int multiple,int win_id);

    int get_channel_type(int index);
    int get_sort();
    void set_sort(int val);
    double get_display_width_old();
    void set_display_width_old(double val);
    double get_display_height_old();
    void set_display_height_old(double val);
    double get_display_x_old();
    void set_display_x_old(double val);
    double get_display_y_old();
    void set_display_y_old(double val);

    void set_echo_ip(QList<QString> val);
    QList<QString> get_echo_ip();
    QString get_echo_ip_by_index(int index);

    unsigned char *get_signal_source_cmd();
    void set_signal_source_cmd(const unsigned char *val);
    void sort_signal_source();
    QSharedPointer<data_info> get_signal_source_by_id(const QString &id, const QList<QSharedPointer<data_info> > &list);
    QVariantList get_group_childs();
    void set_display_row_num(int val);
    int get_display_row_num();
    void set_display_row(int val);
    int get_display_row();
    void set_display_column_num(int val);
    int get_display_column_num();
    void set_display_column(int val);
    int get_display_column();
    QString get_group_name();
    void set_group_name(QString val);
    void print_signal_source();

    void reset_signal_source_status();

    void send_win_level_changed_event(QSharedPointer<data_info> val);
    void send_start_image_request_event();

    QImage get_first_image();
    void set_first_image(const QImage &val);
    //二期新增 轮巡的
    void add_polling(QSharedPointer<data_info> val);
    QVariantList get_list_polling();
     void clear_polling();
    void send_stop_polling_event(int index);
    void send_open_polling_event(int index);
    void send_stop_and_open_polling_event(int o_index, int s_index);
    QSharedPointer<data_info> get_polling_by_id(int index);

    //二期 改变信号源的
    void send_change_signal_source_to_wall_signal_event(QSharedPointer<data_info> val);
    void send_change_signal_source_to_wall_signal_by_old_event(QSharedPointer<data_info> val);
   //
    bool get_isMe();
    void set_isMe(bool val);

    //二期新增 发送字幕信号
    void send_caption_signal_event(int hint);
    int getPolling_hint() const;
    void setPolling_hint(int polling_hint);

    //删除room对象C区的某个信号
    void remove_signal_source_in_tv_wall(int index);
    //刷新窗口
    void send_fresh_win_signal_event();

    //轮巡完后加载数据
    void send_polling_changed_event();
    //获取完信号模式后发送
    void send_mode_changed_event();
    bool getIsModeChanged() ;
    void setIsModeChanged(int  hint);
    //触发信号同步---信号模式
    void send_changed_mode_signal_event(QString index);
    void send_changed_mode_signal_event(int index);

    void send_add_mode_signal_event(QSharedPointer<data_info> val);
    void send_remove_mode_signal_event(QString id);
    void send_edit_mode_signal_event(QSharedPointer<data_info> val);

    //
    void send_ptz_signal_event(int o,int a,int win_id);
    //
    //音频
    void send_open_audio_signal_event(QString sig_id,int Channel,int vol);
    void send_close_audio_signal_event(int audio_id);
    void send_set_audio_signal_event(int audio_id,int vol);
    void add_audio_list(QSharedPointer<data_info> val);
    QList<QSharedPointer<data_info> > get_list_audio();
    QList<QSharedPointer<data_info> > headler_device();

    //
    void send_get_wall_signal_event();

    void send_write_to_serial_signal_event(int o,int a);
    void send_close_serial_signal_event();
    int get_com() const;
    void set_com(int com);


    QHash<QString, QSharedPointer<cmd_info>> get_udp_cmds();
    void add_udp_cmds(QSharedPointer<cmd_info> val);
    QSharedPointer<cmd_info> get_udp_cmds_info(QString val);
    QString get_cmd_from_msg(QString val);
    bool is_in_cmd(QString val);

    int getPortSend() const;
    void setPortSend(int portSend);

    int getPortRcv() const;
    void setPortRcv(int portRcv);

    QList<QSharedPointer<data_info> > getList_akong() const;
    void setList_akong(const QList<QSharedPointer<data_info> > &list_akong);
    
    bool getIs_led() const;
    void setIs_led(bool is_led);

signals:
    void image_changed_signal(int index);
    void phy_wall_row_changed();
    void phy_wall_column_changed();
    void device_air_slider_changed();
    void device_air_switch_changed();
    void logical_wall_row_changed();
    void logical_wall_column_changed();
    void signal_source_changed(int index);
    void signal_source_in_wall_changed(int index);
    void image_incoming_signal(QString id, QImage img);
    void add_signal_source_to_wall_signal(QSharedPointer<data_info> val);
    void update_signal_source_to_wall_signal(QSharedPointer<data_info> val);
    void remove_signal_source_to_wall_signal(QSharedPointer<data_info> val);
    void clear_window_signal();
    void device_ctrl_signal(QString cmd);
    void remove_signal_source_signal(QSharedPointer<data_info> val);
    void add_signal_source_signal(QSharedPointer<data_info> val);
    void temperature_changed_signal(QString id);
    void device_ctrl_param_1_signal(QString cmd, int channel);
    void device_ctrl_param_2_signal(QString cmd, int channel, int param1);
    void device_ctrl_param_3_signal(int operate ,int action,int multiple,int win_id);
    void device_status_changed_signal(QString id, bool status);
    void mute_status_changed_signal(QString id, int index, bool status);
    void volume_value_signal(QString id, int index, int value, double y);
    void win_level_changed(QSharedPointer<data_info> val);
    void start_image_request_signal();
    void status_chanaged(bool val);
    //二期新增 轮询信号
    void stop_polling(int index);
    void open_polling(int index);
    void stop_and_open_polling(int o_index, int s_index);
    void polling_changed(int index); //用于防止数据刷新不出来的信号
    //二期新增 接受要同步操作的信号
    void sync_win_open_signal(QSharedPointer<data_info> val);
    void sync_win_close_signal(int win_id);
    void sync_win_close_all_signal();
    void sync_win_move_signal(QSharedPointer<data_info> val);
    void sync_win_level_signal(int win_id,int hint);
    void sync_win_change_signal(QSharedPointer<data_info> val);

    void sync_mode_change_signal(QString id);
    void sync_mode_remove_signal(QString id);
    void sync_mode_add_signal(QSharedPointer<data_info> val);
    void sync_mode_edit_signal(QSharedPointer<data_info> val);

    void sync_audio_open_signal(QString sig_id,int channel,int vol,int au_id);
    void sync_audio_close_signal(int au_id);
    void sync_audio_set_signal(int au_id,int vol);

    //二期新增 替换C区信号源的信号
    void change_signal_source_to_wall_signal(QSharedPointer<data_info> val);
    //二期新增 字幕信号
    void caption_signal(int hint);
    //
    void fresh_win_signal();
    //轮巡状态信号
    void polling_status_signal(int index);
    //信号模式
    void mode_changed();

    //触发信号模式同步改变的信号
    void changed_mode_signal(QString index);
    void changed_mode_signal_int(int index);

    void add_mode_signal(QSharedPointer<data_info> val);
    void remove_mode_signal(QString id);
    void edit_mode_signal(QSharedPointer<data_info> val);

    //云台控制
    void ptz_signal(int o,int a,int win_id);

    //音频
    void open_audio_signal(QString sig_id,int Channel,int vol);
    void close_audio_signal(int audio_id);
    void set_audio_signal(int audio_id,int vol);

    //枚举信号墙的信号
    void get_wall_signal();


    //写数据入串口
    void write_to_serial_signal(int o,int a);
    void close_serial_signal();

    void disconnect_signal();  //断开重连信号
    void reconnect_signal();

    void get_resolution(QString str); //获取大屏分辨率的信号
public slots:


protected:
    QString m_name;
    QString m_ico_path;
    QPixmap m_image;
    int m_index;//设备索引
    int m_device_type;//设备类型
    QImage m_first_image;
    //二期新增的两个属性  model 和 modelName  相同设备不同厂家不同指令
    QString m_modelName;
    QString m_model;
    QString m_device_ctrl_top;//设备控制区域，上面的图片路径
    QString m_device_ctrl_hor_left;
    QString m_device_ctrl_hor_center;
    QString m_device_ctrl_hor_right;
    QString m_cmd_type;

    unsigned int m_win_id;
    QString m_id;
    QString m_ctrl_progress;
    QString m_local_ip_address;
    QString m_ip_address;
    QString m_user_name;
    QString m_user_pwd;
    QString m_group_name;
    unsigned char m_signal_source_cmd[256];
    int m_port;
    int m_portSend;
    int m_portRcv;
    int m_channel_no;
    int m_sort;
    int m_volume;
    int m_audio_id;
    int m_audio_channel;
    int m_com;

    QVariantList m_music_buttons;
    QVariantList m_music_slides;
    QVariantList m_group_childs;


    bool m_device_air_slider;
    bool m_device_air_switch;
    bool m_device_status;

    double m_display_width;
    double m_display_height;
    double m_display_x;
    double m_display_y;

    double m_display_width_old;
    double m_display_height_old;
    double m_display_x_old;
    double m_display_y_old;

    int m_display_row;//吸附,包括双击,水平在第几个小宫格开始
    int m_display_column;//吸附,包括双击,垂直在第几个小宫格开始
    int m_display_row_num;//吸附,包括双击,水平占用几个小宫格
    int m_display_column_num;//吸附,包括双击,垂直占用几个小宫格

    int m_wall_width;
    int m_wall_height;
    bool m_isMe; //二期新增
    bool m_isModeChanged; //信号模式切换时，由于过快会导致本机接收到同步信号
    int  m_isModeChanged_hint;
    int m_polling_hint;
    QSharedPointer<data_info> m_old_data_info;//原始信号源信息
    QList<QSharedPointer<data_info>> m_list_signal_source;
    QList<QSharedPointer<data_info>> m_list_signal_mode;
    QList<QSharedPointer<data_info>> m_list_device;
    QList<QSharedPointer<data_info>> m_list_device_in_e; //二期新增 e区的设备
    QList<QSharedPointer<data_info>> m_list_signal_source_in_tv_wall;
    QList<QSharedPointer<data_info>> m_list_polling;  //二期新增 轮巡列表
    QList<QSharedPointer<data_info>> m_list_audio;  //二期新增 音频列表

    QList<QSharedPointer<data_info>> m_list_akong;  //三期新增 udp 连接列表

//    QList<QSharedPointer<data_info>> m_list_signal_source_group;//信号源分组

    int m_phy_wall_row;
    int m_phy_wall_column;
    int m_logical_wall_row;
    int m_logical_wall_column;

    QList<QString> m_list_echo_ip;
    QVariantList device_type_list; //设备型号列表

    QString get_ico_path(const QString &path);
    bool exists_signal_source(const QString &id);
    void sort_signal_source(QList<QSharedPointer<data_info> > &list);

    QHash<QString, QSharedPointer<cmd_info>> m_udp_cmds; //存设备的udp指令
    QSharedPointer<cmd_info> temp;
    bool m_is_led;  // 区别是不是led 默认false
};

#endif // DATA_INFO_H
