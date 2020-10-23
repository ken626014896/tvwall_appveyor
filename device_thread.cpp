#include "device_thread.h"
#include "device_worker.h"
#include "sy_logger.h"


device_thread::device_thread(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) :
    QThread(parent),
    m_data_info(data),
    m_acc_conf_info(conf)
{

}

void device_thread::run()
{
    device_worker work(m_data_info, m_acc_conf_info);
    //主线程传递信号给线程    发送指令的信号
    connect(this, &device_thread::device_ctrl_signal, &work, &device_worker::device_ctrl_slot);
    connect(this, &device_thread::device_ctrl_param_1_signal, &work, &device_worker::device_ctrl_param_1_slot);
    connect(this, &device_thread::device_ctrl_param_2_signal, &work, &device_worker::device_ctrl_param_2_slot);
    connect(this, &device_thread::device_ctrl_param_3_signal, &work, &device_worker::device_ctrl_param_3_slot);


    //线程传递信号给主线程    发送完信号的回调操作信号
    connect(&work, &device_worker::temperature_changed_signal, this, &device_thread::temperature_changed_signal);
    connect(&work, &device_worker::device_status_changed_signal, this, &device_thread::device_status_changed_signal);
    connect(&work, &device_worker::mute_status_changed_signal, this, &device_thread::mute_status_changed_signal);
    connect(&work, &device_worker::volume_value_signal, this, &device_thread::volume_value_signal);

    work.start();
    this->exec();
    work.stop();
}
