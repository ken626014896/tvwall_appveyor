#include "room_signal_sync_thread.h"
#include "room_signal_sync_worker.h"
room_signal_sync_thread::room_signal_sync_thread(data_info *data, QObject *parent) : QThread(parent),
    m_data_info(data)
{
}

void room_signal_sync_thread::run()
{
    room_signal_sync_worker work(m_data_info);
    connect(&work, &room_signal_sync_worker::sync_win_open_signal, this, &room_signal_sync_thread::sync_win_open_signal);
    connect(&work, &room_signal_sync_worker::sync_win_close_signal, this, &room_signal_sync_thread::sync_win_close_signal);
    connect(&work, &room_signal_sync_worker::sync_win_close_all_signal, this, &room_signal_sync_thread::sync_win_close_all_signal);
    connect(&work, &room_signal_sync_worker::sync_win_move_signal, this, &room_signal_sync_thread::sync_win_move_signal);
    connect(&work, &room_signal_sync_worker::sync_win_level_signal, this, &room_signal_sync_thread::sync_win_level_signal);
    connect(&work, &room_signal_sync_worker::sync_win_change_signal, this, &room_signal_sync_thread::sync_win_change_signal);
    connect(&work, &room_signal_sync_worker::sync_mode_change_signal, this, &room_signal_sync_thread::sync_mode_change_signal);
    connect(&work, &room_signal_sync_worker::sync_mode_add_signal, this, &room_signal_sync_thread::sync_mode_add_signal);
    connect(&work, &room_signal_sync_worker::sync_mode_remove_signal, this, &room_signal_sync_thread::sync_mode_remove_signal);
    connect(&work, &room_signal_sync_worker::sync_mode_edit_signal, this, &room_signal_sync_thread::sync_mode_edit_signal);
    connect(&work, &room_signal_sync_worker::sync_audio_open_signal, this, &room_signal_sync_thread::sync_audio_open_signal);
    connect(&work, &room_signal_sync_worker::sync_audio_close_signal, this, &room_signal_sync_thread::sync_audio_close_signal);
    connect(&work, &room_signal_sync_worker::sync_audio_set_signal, this, &room_signal_sync_thread::sync_audio_set_signal);
    connect(this, &room_signal_sync_thread::canSend, &work, &room_signal_sync_worker::canSend_slot);
    connect(&work, &room_signal_sync_worker::msgCome, this, &room_signal_sync_thread::msgCome);

    connect(&work, &room_signal_sync_worker::reconnect_signal, this, &room_signal_sync_thread::reconnect_signal);
    connect(&work, &room_signal_sync_worker::disconnect_signal, this, &room_signal_sync_thread::disconnect_signal);
    work.start();
    this->exec();
    work.stop();
}
