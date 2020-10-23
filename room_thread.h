#ifndef ROOM_THREAD_H
#define ROOM_THREAD_H

#include <QObject>
#include <QThread>

#include "data_info.h"

class room_thread : public QThread
{
    Q_OBJECT
public:
    explicit room_thread(data_info *data, QObject *parent = nullptr);

signals:
    void image_incoming_signal(QString id, QImage img);
    void clear_window_signal();
    void add_signal_source_signal(QSharedPointer<data_info> val);
    void polling_status_signal(int index);
    void get_resolution(QString str); //获取大屏分辨率的信号
public slots:

protected:
    void run();
private:
    data_info *m_data_info;
};

#endif // ROOM_THREAD_H
