#include "device_ctrl_tcp_type_5.h"
#include "sy_logger.h"
#include "sy_utils.h"

#include <QHostAddress>

QMutex   device_ctrl_tcp_type_5::m_mutex;
QHash<QString, QSharedPointer<device_ctrl_base>> device_ctrl_tcp_type_5::m_hash_ctrl;


device_ctrl_tcp_type_5::device_ctrl_tcp_type_5(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) :
    device_ctrl_tcp_base(data, conf, parent),
    m_dev_status(false),
    m_timer(nullptr),
    isFirstConnect(true),
    tcp_timer(nullptr)
{

}

device_ctrl_tcp_type_5::~device_ctrl_tcp_type_5()
{
    if(m_timer != nullptr){
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }

    if(tcp_timer != nullptr){
        tcp_timer->stop();
        delete tcp_timer;
        tcp_timer = nullptr;
    }
    if( m_tcp_socket!= nullptr){
        m_tcp_socket->close();
        delete m_tcp_socket;
        m_tcp_socket = nullptr;
    }
}

bool device_ctrl_tcp_type_5::send_cmd(const QString &cmd)
{
    if(m_data_info == nullptr || m_tcp_socket == nullptr){
        qWarnx(qtr("参数错误"));
        return false;
    }
    qWarnxxx(qtr("不支持的调用方式"), m_data_info->get_name(), cmd);
//    if(m_data_info->get_channel_no() >= 8 || m_data_info->get_channel_no() < 0){
//        qWarnxxx(qtr("设备位错误"), m_data_info->get_name(), m_data_info->get_channel_no());
//        return false;
//    }
//    QMutexLocker locker(&m_mutex_ctrl);
//    bool rv = false;
//    if(cmd == "btnCenterActive" || cmd == "btnBottomCenterActive"){
//        rv = send_content(QString("on%1:02").arg(m_data_info->get_channel_no()));
//    }
//    else if(cmd == "btnCenterIdle" || cmd == "btnBottomCenterIdle"){
//        rv = send_content(QString("off%1:02").arg(m_data_info->get_channel_no()));
//    }
//    else {
//        qWarnxxx(qtr("不支持的指令"), m_data_info->get_name(), cmd);
//    }
//    if(!rv){
//        stop();
//        start();
//    }
    return false;

//    if(m_data_info == nullptr){
//        return false;
//    }
//    if(m_tcp_socket != nullptr){
//        stop();
//    }
//    m_tcp_socket = new QTcpSocket();
//    m_tcp_socket->connectToHost(QHostAddress(m_data_info->get_ip_address()), m_data_info->get_port());

//    if(!m_tcp_socket->waitForConnected(3000)){
//        qWarnxxxx(qtr("连接失败"), m_data_info->get_name(), m_data_info->get_channel_no(), m_tcp_socket->errorString());
//        stop();
//        return false;
//    }
//    if(m_data_info->get_channel_no() >= 8 || m_data_info->get_channel_no() < 0){
//        qWarnxxx(qtr("设备位错误"), m_data_info->get_name(), m_data_info->get_channel_no());
//        stop();
//        return false;
//    }
//    bool rv = false;
//    if(cmd == "btnCenterActive" || cmd == "btnBottomCenterActive"){
//        rv = send_content(QString("on%1:02").arg(m_data_info->get_channel_no()));
//        if(m_tcp_socket->waitForReadyRead(500)){
//            m_tcp_socket->readAll();
//        }
//    }
//    else if(cmd == "btnCenterIdle" || cmd == "btnBottomCenterIdle"){
//        rv = send_content(QString("off%1:02").arg(m_data_info->get_channel_no()));
//    }
//    else {
//        qWarnxxx(qtr("不支持的指令"), m_data_info->get_name(), cmd);
//    }
//    if(m_tcp_socket != nullptr){
//        m_tcp_socket->flush();
//    }

//    stop();
    //    return rv;
}

void device_ctrl_tcp_type_5::start()
{
    QMutexLocker locker(&m_mutex_ctrl);
    if(m_tcp_socket != nullptr || m_data_info == nullptr || m_data_info->get_ip_address().isEmpty() || m_data_info->get_port() <= 0){
        return;
    }
    qDebugxxxx(qtr("启动设备连接"), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());

    m_tcp_socket = new QTcpSocket();

    connect(m_tcp_socket, &QTcpSocket::readyRead, this, &device_ctrl_tcp_base::recv_data);
    connect(m_tcp_socket, &QTcpSocket::connected, this, &device_ctrl_tcp_base::connected);
    connect(m_tcp_socket, &QTcpSocket::disconnected, this, &device_ctrl_tcp_type_5::disconnected_status);


    m_tcp_socket->connectToHost(QHostAddress(m_data_info->get_ip_address()), m_data_info->get_port());
//    m_tcp_socket->connectToHost("127.0.0.1", 8888);

//    if(m_timer == nullptr){
        //2分钟查询一次状态
//        m_timer = new QTimer;
 //       connect(m_timer, &QTimer::timeout, this, &device_ctrl_tcp_type_5::heatbeat_slot);
 //       m_timer->start(2 * 60 * 1000);
 //   }
    if(tcp_timer == nullptr) {
        tcp_timer = new QTimer();
        connect(tcp_timer, &QTimer::timeout, this, &device_ctrl_tcp_type_5::time_reconnect);
    } else {
        tcp_timer->stop();
    }
}

void device_ctrl_tcp_type_5::stop()
{
    if(m_timer != nullptr){
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }

    if(tcp_timer != nullptr){
        tcp_timer->stop();
        delete tcp_timer;
        tcp_timer = nullptr;
    }
    m_tcp_socket->blockSignals(true);

    if( m_tcp_socket!= nullptr){
        m_tcp_socket->close();
        delete m_tcp_socket;
        m_tcp_socket = nullptr;
    }
}

QSharedPointer<device_ctrl_base> device_ctrl_tcp_type_5::get_device_ctrl(const QString key, data_info *data, QSharedPointer<acc_conf_info> conf)
{
    QMutexLocker locker(&m_mutex);
    QSharedPointer<device_ctrl_base> ctrl = m_hash_ctrl.value(key);
    if(!ctrl.isNull()){
        return ctrl;
    }
    ctrl = QSharedPointer<device_ctrl_base>(new device_ctrl_tcp_type_5(data, conf));
    m_hash_ctrl.insert(key, ctrl);
    return ctrl;
}

void device_ctrl_tcp_type_5::set_acc_conf_info(QSharedPointer<acc_conf_info> conf)
{
    m_acc_conf_info=conf;
}

void device_ctrl_tcp_type_5::heatbeat_slot()
{
    if(m_tcp_socket == nullptr || m_data_info == nullptr){
        return;
    }
    if(!send_content(QString("read"))){  //读取设备状态
        qWarnxx(qtr("发送读取指令失败"), m_data_info->get_name());
        return;
    }
    if(!m_tcp_socket->waitForReadyRead(3000)){
        qWarnxx(qtr("读取状态，未收到响应"), m_data_info->get_name());
        return;
    }
    QString data = QString(m_tcp_socket->readAll()).trimmed();
//    emit device_status_changed_for_relay_signal(1, true);
    qDebugxx(qtr("读取继电器状态"), data);
    for(int i=1;i<=data.length()-5;i++){
        if(data.right(i).left(1) == "1"){
            emit device_status_changed_for_relay_signal(i, true);
        }
        else{
            emit device_status_changed_for_relay_signal(i, false);
        }
    }
}

void device_ctrl_tcp_type_5::disconnected_status()
{
    qDebug() << "type5_device_tcp_disconnect";
    reconnected();
}

void device_ctrl_tcp_type_5::time_reconnect()
{
    start();
}

void device_ctrl_tcp_type_5::reconnected()
{
    isFirstConnect = false;
    m_tcp_socket->blockSignals(true);
    if(m_tcp_socket != nullptr) {
        m_tcp_socket->close();
        m_tcp_socket->deleteLater();
        m_tcp_socket = nullptr;
    }
    if(tcp_timer != nullptr) {
        tcp_timer->start(3000);
    }
}

void device_ctrl_tcp_type_5::device_ctrl_slot(QString cmd, int bit)   //发送的指令
{
    QMutexLocker locker(&m_mutex_ctrl);
    if(m_data_info == nullptr || bit < 0  || m_acc_conf_info.isNull()){
        qWarnxxx(qtr("参数错误"), bit, m_acc_conf_info.isNull());
        return;
    }

    qDebugxxxx(qtr("执行指令"), m_data_info->get_name(), cmd, bit);
    if(cmd == "read"){

        if(!send_content(QString("read"))){
            qWarnxx(qtr("发送读取指令失败"), m_data_info->get_name());
            re_connect();
            return;
        }
        if(!m_tcp_socket->waitForReadyRead(1000)){
            qWarnxx(qtr("读取状态，未收到响应"), m_data_info->get_name());
            re_connect();
            return;
        }
        QString data = QString(m_tcp_socket->readAll());
        bool status = false;
        if(data.trimmed().mid(5+bit, 1) == "1"){
            status = true;
        }

//        m_data_info->set_device_status(status);
        qDebugxx(qtr("接收到状态返回"), data);
        emit device_status_changed_for_relay_signal(bit, status);
    }
    else{
        QSharedPointer<cmd_info> info = m_acc_conf_info->get_cmd_info(cmd);
        if(info.isNull()){
            qWarnxxxxx(qtr("不支持的指令"), cmd, m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());
            return;
        }
        QString msg = info->get_send();
//         qDebug()<<msg;
        QByteArray data = QByteArray::fromHex(info->get_send().toLatin1());

        if(m_acc_conf_info->get_cmd_type() == "ascii"){ //屏幕设备

            msg = info->get_send().replace("*", QString("%1").arg(bit));

            data = msg.toLatin1();
        }
//        qDebug()<<m_data_info->get_device_type();
        qDebug()<<data.toHex();
        qDebugxxxxx(qtr("发送指令"), msg, m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());
        int rv = m_tcp_socket->write(data);
        if(rv != data.length()){
            qWarnxxxxxxx(qtr("指令发送失败"), QString("%1:%2").arg(m_data_info->get_ip_address()).arg(m_data_info->get_port()), rv, cmd, info->get_send(), data.length(), m_tcp_socket->errorString());
            re_connect();
            return ;
        }
        if(cmd=="btnCenterIdle")
            emit device_status_changed_for_relay_signal(bit, false);
        else if(cmd=="btnCenterActive"){
            emit device_status_changed_for_relay_signal(bit, true);
        }

    }
//    else if(cmd == "btnCenterActive" || cmd == "btnBottomCenterActive"){
//        if(!send_content(QString("on%1:02").arg(bit))){
//            qWarnxx(qtr("发送开指令失败"), m_data_info->get_name());
//            re_connect();
//            return;
//        }
//    }
//    else if(cmd == "btnCenterIdle" || cmd == "btnBottomCenterIdle"){
//        if(!send_content(QString("off%1:02").arg(bit))){
//            qWarnxx(qtr("发送关指令失败"), m_data_info->get_name());
//            re_connect();
//            return;
//        }
//    }
//    else {
//        qWarnxxx(qtr("不支持的指令"), m_data_info->get_name(), cmd);
    //    }
}

void device_ctrl_tcp_type_5::device_ctrl_slot2(QString cmd, int bit, QSharedPointer<acc_conf_info> conf, QString tempStr)
{
    //    qDebug()<<111<<cmd;
        QMutexLocker locker(&m_mutex_ctrl);

        if(m_data_info == nullptr || bit < 0  || conf.isNull()){
            qWarnxxx(qtr("参数错误"), bit, conf.isNull());
            return;
        }
        qDebugxxxx(qtr("执行指令"), m_data_info->get_name(), cmd, bit);
        if(cmd == "read"){

            if(!send_content(QString("read"))){
                qWarnxx(qtr("发送读取指令失败"), m_data_info->get_name());
                re_connect();
                return;
            }
            if(!m_tcp_socket->waitForReadyRead(1000)){
                qWarnxx(qtr("读取状态，未收到响应"), m_data_info->get_name());
                re_connect();
                return;
            }
            QString data = QString(m_tcp_socket->readAll());
            bool status = false;
            if(data.trimmed().mid(5+bit, 1) == "1"){
                status = true;
            }

    //        m_data_info->set_device_status(status);
            qDebugxx(qtr("接收到状态返回"), data);


    //        emit device_status_changed_for_relay_signal(bit, status);
            emit device_status_changed_for_relay_signal2(tempStr, status);
        }
        else{
            QSharedPointer<cmd_info> info = conf->get_cmd_info(cmd);
            if(info.isNull()){
                qWarnxxxxx(qtr("不支持的指令"), cmd, m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());
                return;
            }
            QString msg = info->get_send();
            QByteArray data = QByteArray::fromHex(info->get_send().toLatin1());

            if(conf->get_cmd_type() == "ascii"){ //屏幕设备

                msg = info->get_send().replace("*", QString("%1").arg(bit));

                data = msg.toLatin1();
            }
    //        qDebug()<<m_data_info->get_device_type();
            qDebug()<<qtr("发送指令")<<data.toHex();
            qDebugxxxxx(qtr("发送指令"), msg, m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());
            int rv = m_tcp_socket->write(data);
            if(rv != data.length()){
                qWarnxxxxxxx(qtr("指令发送失败"), QString("%1:%2").arg(m_data_info->get_ip_address()).arg(m_data_info->get_port()), rv, cmd, info->get_send(), data.length(), m_tcp_socket->errorString());
                re_connect();
                return ;
            }
            if(cmd=="btnCenterIdle")
    //            emit device_status_changed_for_relay_signal(bit, false);
                emit device_status_changed_for_relay_signal2(tempStr, false);
            else if(cmd=="btnCenterActive"){
    //            emit device_status_changed_for_relay_signal(bit, true);
                emit device_status_changed_for_relay_signal2(tempStr, true);

            }

        }
}

void device_ctrl_tcp_type_5::recv_data()
{
//    QByteArray res=m_tcp_socket->readAll();
//    qDebug()<<"tcp7  cmd"<<res.toHex();
//    emit device_status_changed_for_relay_signal(1, true);

}

void device_ctrl_tcp_type_5::connected()
{
//    QMutexLocker locker(&m_mutex_ctrl);
    if(m_tcp_socket == nullptr){
        return;
    }
    if(m_tcp_socket->waitForReadyRead(3000)){
        qDebugxxxxx(qtr("收到继电器响应"), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port(), QString(m_tcp_socket->readAll()));
    }
    else{
        qDebugxxxxx(qtr("收到继电器响应，未收型号"), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port(), QString(m_tcp_socket->readAll()));
    }
//   qDebug()<<"2";
    wait_by_event_loop(100);
//    emit device_connected_signal();
// heatbeat_slot();
}
