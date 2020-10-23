#ifndef ROOM_SIGNAL_SYNC_THREAD_H
#define ROOM_SIGNAL_SYNC_THREAD_H

#include <QObject>
#include "data_info.h"
#include <QThread>
class room_signal_sync_thread : public QThread
{
    Q_OBJECT
public:
    explicit room_signal_sync_thread(data_info *data, QObject *parent = nullptr);

signals:
    void sync_win_open_signal(QSharedPointer<data_info> val);
    void sync_win_close_signal(int win_id);
    void sync_win_close_all_signal();
    void sync_win_move_signal(QSharedPointer<data_info> val);
    void sync_win_level_signal(int win_id, int hint);
    void sync_win_change_signal(QSharedPointer<data_info> val);

    void sync_mode_change_signal(QString id);
    void sync_mode_remove_signal(QString id);
    void sync_mode_add_signal(QSharedPointer<data_info> val);
    void sync_mode_edit_signal(QSharedPointer<data_info> val);

    void sync_audio_open_signal(QString sig_id, int channel, int vol, int au_id);
    void sync_audio_close_signal(int au_id);
    void sync_audio_set_signal(int au_id, int vol);

    void canSend(QByteArray msg);
    void msgCome(QByteArray msg);

    void disconnect_signal();  //断开重连信号
    void reconnect_signal();
public slots:

protected:
    void run();
private:
    data_info *m_data_info;
};

#endif // ROOM_SIGNAL_SYNC_THREAD_H
