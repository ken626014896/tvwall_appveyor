#ifndef SIGNAL_RESOURCE_INFO_H
#define SIGNAL_RESOURCE_INFO_H

#include <QObject>

#include "data_info.h"
#include "signal_resource_thread.h"

class signal_resource_info : public data_info
{
    Q_OBJECT
public:
    explicit signal_resource_info(QObject *parent = nullptr);
    ~signal_resource_info();

    void start();
    void stop();


signals:

public slots:

private:
    signal_resource_thread *m_signal_resource_thread;
};

#endif // SIGNAL_RESOURCE_INFO_H
