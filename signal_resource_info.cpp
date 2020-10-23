 #include "signal_resource_info.h"
#include "sy_logger.h"


signal_resource_info::signal_resource_info(QObject *parent) :
    data_info(parent),
    m_signal_resource_thread(nullptr)
{

}

signal_resource_info::~signal_resource_info()
{
    stop();
}

void signal_resource_info::start()
{
    if(m_signal_resource_thread != nullptr){
        return;
    }
    if(get_device_type() == 0x07){
//        qDebugxxx(qtr("启动分组获取"), get_name(), m_list_signal_source.count());
        foreach (QSharedPointer<data_info> info, m_list_signal_source) {
            if(info.isNull()){
                continue;
            }
//            qDebugxxx(qtr("启动分组设备"), info->get_name(), info->get_device_type());
            info->start();
        }
    }
    else{
//        qDebugxx(qtr("启动图片获取"), get_name());
        m_signal_resource_thread = new signal_resource_thread((data_info *)this);
        m_signal_resource_thread->start();
    }

}

void signal_resource_info::stop()
{
    if(m_signal_resource_thread != nullptr){
        m_signal_resource_thread->quit();
        m_signal_resource_thread->wait();
        delete m_signal_resource_thread;
        m_signal_resource_thread = nullptr;
    }
    if(get_device_type() == 0x07){
        foreach (QSharedPointer<data_info> info, m_list_signal_source) {
            if(!info.isNull()){
                continue;
            }
            info->stop();
        }
    }
    data_info::stop();
    qDebugxxxx(qtr("停止信号源连接"), get_name(), get_ip_address(), get_port());
}
