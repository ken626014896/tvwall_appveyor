#include "signal_resource_thread.h"
#include "signal_resource_worker.h"



signal_resource_thread::signal_resource_thread(data_info *data, QObject *parent) : QThread(parent),
    m_data_info(data)
{

}

void signal_resource_thread::run()
{
    signal_resource_worker work(m_data_info);
    work.start();
    this->exec();
    work.stop();
}
