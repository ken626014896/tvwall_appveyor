#ifndef ROOM_SIGNAL_SYNC_WORKER_H
#define ROOM_SIGNAL_SYNC_WORKER_H

#include <QObject>
#include "data_info.h"
#include <QTcpSocket>
class room_signal_sync_worker : public QObject
{
    Q_OBJECT
public:
    explicit room_signal_sync_worker(data_info *data,QObject *parent = nullptr);
    ~room_signal_sync_worker();

    void start();
    void stop();
    void win_open(QByteArray res);
    void win_close(QByteArray res);
    void win_close_all(QByteArray res);
    void win_move(QByteArray res);
    void win_level(QByteArray res);
    void win_change(QByteArray res);  //窗口信号源改变
    void mode_change(QByteArray res);
    void mode_add(QByteArray res);
    void mode_edit(QByteArray res);
    void mode_remove(QByteArray res);
    void audio_open(QByteArray res);
    void audio_close(QByteArray res);
    void audio_set(QByteArray res);

signals:
    void sync_win_open_signal(QSharedPointer<data_info> val);
    void sync_win_close_signal(int win_id);
    void sync_win_close_all_signal();
    void sync_win_move_signal(QSharedPointer<data_info> val);
    void sync_win_level_signal(int win_id,int hint);
    void sync_win_change_signal(QSharedPointer<data_info> val);

    void sync_open_video_signal();
    void sync_close_video_signal();
    void sync_setting_videio_signal();

    void sync_mode_change_signal(QString id);
    void sync_mode_add_signal(QSharedPointer<data_info> val);
    void sync_mode_remove_signal(QString id);
    void sync_mode_edit_signal(QSharedPointer<data_info> val);
    void sync_scene_set_signal();
    void sync_scene_signal(); //0删除 1增加 2修改

    void msgCome(QByteArray res);

    void sync_audio_open_signal(QString sig_id,int channel,int vol,int au_id);
    void sync_audio_close_signal(int au_id);
    void sync_audio_set_signal(int au_id,int vol);

    void disconnect_signal();  //断开重连信号
    void reconnect_signal();
private slots:
    void connected();
    void recv_data();
    void time_reconnect();
    void reconnected();
    void disconnected_status();
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
public slots:
    void canSend_slot(QByteArray res);
private:
    void get_window_info();
    QTcpSocket *m_tcp_socket;
    int m_packet_id;
    data_info *m_data_info;
    QString m_group_id;
    QTimer *m_timer;
    int m_signal_source_index;
    int m_accept_msg_hint;

    void add_cmd_header(QByteArray &data, int cmd, int len);
    QByteArray get_response();
    void login();
    void sync();
    QSharedPointer<data_info> get_signal_source(const QString &id);
    QString m_group_id_mode;

    bool isFirstConnect;
};

#endif // ROOM_SIGNAL_SYNC_WORKER_H
