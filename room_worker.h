#ifndef ROOM_WORKER_H
#define ROOM_WORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

#include "data_info.h"
#include "room_check.h"

class room_worker : public QObject
{
    Q_OBJECT
public:
    explicit room_worker(data_info *data, QObject *parent = nullptr);
    ~room_worker();

    void start();
    void stop();
signals:
    void image_incoming_signal(QString id, QImage img);
    void polling_status_signal(int index);
    void get_resolution(QString str); //获取大屏分辨率的信号
public slots:
    void clear_window_slot();
    void add_signal_source_slot(QSharedPointer<data_info> val);

private slots:
    void connected();
    void recv_data();
    void disconnected_status();
    void add_signal_source_to_wall_slot(QSharedPointer<data_info> val);
    void update_signal_source_to_wall_slot(QSharedPointer<data_info> val);
    void remove_signal_source_to_wall_slot(QSharedPointer<data_info> val);
    void remove_signal_source_slot(QSharedPointer<data_info> val);
    void win_level_chanaged_slot(QSharedPointer<data_info> val);
    void time_reconnect();

    //二期 信号源替换
    void change_signal_source_to_wall_slot(QSharedPointer<data_info> val);
    //二期 安卓或ios操作字幕
    void change_caption_slot(int hint);

    //触发信号同步--信号模式
    void mode_chanaged_slot(QString index);
    void mode_chanaged_slot_int(int index);
    void mode_add_slot(QSharedPointer<data_info> info);
    void mode_edit_slot(QSharedPointer<data_info> info);
    void mode_remove_slot(QString id);

    //云台控制
    void ptz_slot(int o,int a,int win_id);

    //音频控制
    void open_audio_slot(QString sig_id,int Channel,int vol);
    void close_audio_slot(int audio_id);
    void set_audio_slot(int audio_id,int vol);

    //
    void get_wall_signal_slot();

    void is_display_wall_signal();//判断大屏中是否出现了视频源


private:
    QTcpSocket *m_tcp_socket;
    int m_packet_id;
    data_info *m_data_info;
    QString m_group_id;
    QTimer *m_timer;
    int m_signal_source_index;
    int m_accept_msg_hint;

    void get_signal_source();
    void add_cmd_header(QByteArray &data, int cmd, int len);
    void add_cmd_header2(QByteArray &data, int cmd, int len);
    void logout();
    QByteArray get_response();
    QByteArray get_response2();
    void login();
    void get_sys_info();
    void reconnected();
    void get_resource_info(int type);
    QSharedPointer<data_info> get_signal_source_object(const QString &id);
    void get_window_info();
    QSharedPointer<data_info> get_signal_source(const QString &id);
    void create_connect_for_signal_source(QSharedPointer<data_info> &info);
    void get_node_info();
    void get_resource_group_info(const QString &id, data_info *info, const bool root = false);
    void get_resource_group_info(const QString &id);
    QSharedPointer<data_info> get_signal_source_object(const QString &id, data_info *data);
    void create_connect_for_signal_source(QList<QSharedPointer<data_info> > list);
    //二期新增获取轮询列表
    void get_polling();
    void get_mode();
    int hexToTen(QString str);

    void open_polling_slot(int index);
    void stop_polling_slot(int index);
    void open_and_stop_polling_slot(int o_index,int s_index);
    //获取场景

    //同步
    void sync();
    //刷新窗口
    void fresh_win_slot();

    QString m_group_id_mode;
    bool isFirstConnect;
};

#endif // ROOM_WORKER_H
