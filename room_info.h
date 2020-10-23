#ifndef ROOM_INFO_H
#define ROOM_INFO_H

#include <QObject>
#include <QTcpSocket>

#include "data_info.h"
#include "room_thread.h"
#include "room_signal_sync_thread.h"
#include "room_databuff.h"
#include "room_com.h"
#include <QThread>
class room_info : public data_info
{
    Q_OBJECT
public:
    explicit room_info(QObject *parent = nullptr);
    ~room_info();

    void start();
    void stop();


signals:


public slots:
    void sync_win_open_signal_slot(QSharedPointer<data_info> val);
    void sync_win_close_signal_slot(int win_id);
    void sync_win_close_all_signal_slot();
    void sync_win_move_signal_slot(QSharedPointer<data_info> val);
    void sync_win_level_signal_slot(int win_id,int hint);
    void sync_win_change_signal_slot(QSharedPointer<data_info> val);
    void send_polling_status_signal_slot(int index);

    void sync_mode_change_signal_slot(QString id);
    void sync_mode_remove_signal_slot(QString id);
    void sync_mode_add_signal_slot(QSharedPointer<data_info> val);
    void sync_mode_edit_signal_slot(QSharedPointer<data_info> val);

    void sync_audio_open_signal_slot(QString sig_id,int channel,int vol,int au_id);
    void sync_audio_close_signal_slot(int au_id);
    void sync_audio_set_signal_slot(int au_id,int vol);

    void disconnect_signal_slot();
    void reconnect_signal_slot();

    void send_get_resolution_slot(QString str);
private slots:

private:

    room_thread *m_room_thread;
    room_signal_sync_thread *m_room_signal_sync_thread;

    QThread *workerThread;
    room_databuff *buff;

    QThread *workerThread_com;
    room_com *buff_c;


};

#endif // ROOM_INFO_H
