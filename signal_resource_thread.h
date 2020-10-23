#ifndef SIGNAL_RESOURCE_THREAD_H
#define SIGNAL_RESOURCE_THREAD_H

#include "data_info.h"


#include <QObject>
#include <QThread>

class signal_resource_thread : public QThread
{
    Q_OBJECT
public:
    explicit signal_resource_thread(data_info *data, QObject *parent = nullptr);

signals:

public slots:

protected:
    void run();

private:
    data_info *m_data_info;
};

#endif // SIGNAL_RESOURCE_THREAD_H
