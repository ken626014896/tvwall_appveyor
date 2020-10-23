#include "device_ctrl_tcp_base.h"
#include "sy_logger.h"

#include <QHostAddress>


device_ctrl_tcp_base::device_ctrl_tcp_base(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent) :
    device_ctrl_base(data, conf, parent),
    m_tcp_socket(nullptr),
    isFirstConnect(true),
    m_timer(nullptr)
{

}

void device_ctrl_tcp_base::start()
{

    if(m_tcp_socket != nullptr || m_data_info == nullptr){
        qDebugx(qtr("参数错误"));
        return;
    }
    if(m_data_info->get_ip_address().isEmpty() || m_data_info->get_port() <= 0){
        qDebugxxx(qtr("参数错误2"), m_data_info->get_ip_address(), m_data_info->get_port());
        return;
    }
    qDebugxxxx(qtr("启动设备连接"), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());

    m_tcp_socket = new QTcpSocket();

    connect(m_tcp_socket, &QTcpSocket::readyRead, this, &device_ctrl_tcp_base::recv_data);
    connect(m_tcp_socket, &QTcpSocket::connected, this, &device_ctrl_tcp_base::connected);
    connect(m_tcp_socket, &QTcpSocket::disconnected, this, &device_ctrl_tcp_base::disconnected_status);

//    qDebug()<<m_data_info->get_ip_address();
    m_tcp_socket->connectToHost(QHostAddress(m_data_info->get_ip_address()), m_data_info->get_port());
//    m_tcp_socket->connectToHost("127.0.0.1", 8888);
    if(m_timer == nullptr) {
        m_timer = new QTimer();
        connect(m_timer, &QTimer::timeout, this, &device_ctrl_tcp_base::time_reconnect);
    } else {
        m_timer->stop();
    }
    device_ctrl_base::start();
}

void device_ctrl_tcp_base::stop()
{
    m_tcp_socket->blockSignals(true);

    if(m_tcp_socket != nullptr){
        m_tcp_socket->close();
        delete m_tcp_socket;
        m_tcp_socket = nullptr;
    }
    if(m_timer != nullptr) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    device_ctrl_base::stop();
}

void device_ctrl_tcp_base::re_connect()
{
    if(m_tcp_socket == nullptr || m_acc_conf_info.isNull()){
        return ;
    }

    stop();
    start();
}

void device_ctrl_tcp_base::reconnected()
{
    isFirstConnect = false;
    m_tcp_socket->blockSignals(true);
    if(m_tcp_socket != nullptr) {
        m_tcp_socket->close();
        m_tcp_socket->deleteLater();
        m_tcp_socket = nullptr;
    }
    if(m_timer != nullptr) {
        m_timer->start(3000);
    }
}

bool device_ctrl_tcp_base::send_cmd(const QString &cmd)
{
//    re_connect();

    if(m_tcp_socket == nullptr || m_acc_conf_info.isNull() || !m_tcp_socket->isValid()){
        return false;
    }
    //清空读缓冲
    if(m_tcp_socket->bytesAvailable()){
        m_tcp_socket->readAll();
    }
    QSharedPointer<cmd_info> info = m_acc_conf_info->get_cmd_info(cmd);
    if(info.isNull()){
        qWarnxxxxx(qtr("不支持的指令"), cmd, m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());
        return false;
    }
    qDebug()<<info->get_send();
    QByteArray data = QByteArray::fromHex(info->get_send().toLatin1());
    if(m_acc_conf_info->get_cmd_type() == "ascii"){
        data = info->get_send().toLatin1();
    }
//    qDebug()<<data.toHex();
    qDebugxxxxx(qtr("发送指令"), info->get_send(), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port());
    int rv = m_tcp_socket->write(data);
    if(rv != data.length()){
        qWarnxxxxxxx(qtr("指令发送失败"), QString("%1:%2").arg(m_data_info->get_ip_address()).arg(m_data_info->get_port()), rv, cmd, info->get_send(), data.length(), m_tcp_socket->errorString());
        return false;
    }
    return true;
}

bool device_ctrl_tcp_base::send_content(const QByteArray &content)
{
//    re_connect();

    if(m_tcp_socket == nullptr || !m_tcp_socket->isValid()){
        qWarnx(qtr("未进行有效连接"));
        return false;
    }
    //清空读缓冲
    if(m_tcp_socket->bytesAvailable() > 0){
        m_tcp_socket->readAll();
    }

    if(m_tcp_socket->write(content) != content.length()){
        qWarnxxx(qtr("指令发送失败"), QString(content.toHex()), content.length());
        return false;
    }
    return true;
}


bool device_ctrl_tcp_base::send_content(const QString &content)
{
//    re_connect();

    if(m_tcp_socket == nullptr || m_acc_conf_info.isNull() || !m_tcp_socket->isValid()){
        qDebugxxx(qtr("参数错误"), m_acc_conf_info.isNull(), m_tcp_socket->isValid());
        return false;
    }
    //清空读缓冲
    if(m_tcp_socket->bytesAvailable()){
        m_tcp_socket->readAll();
    }
    qDebugxxx(qtr("发送指令"), QString("%1:%2").arg(m_data_info->get_ip_address()).arg(m_data_info->get_port()), content);


    QByteArray data = content.toLatin1();
//    qDebug()<<content;
//    qDebug()<<data.toHex();
    if(m_tcp_socket->write(data) != data.length()){
        qWarnxxxxx(qtr("指令发送失败"), QString("%1:%2").arg(m_data_info->get_ip_address()).arg(m_data_info->get_port()), content, data.length(), m_tcp_socket->errorString());
        return false;
    }
    return true;
}

bool device_ctrl_tcp_base::send_content_by_hex(const QString &content)
{
//    qDebugxx(qtr("发送指令"), content);
    if(content.isEmpty()){
        return false;
    }
    return send_content(QByteArray::fromHex(content.toLatin1()));
}

void device_ctrl_tcp_base::recv_data()
{
    if(m_tcp_socket == nullptr){
        return;
    }
    qDebug()<<QString(m_tcp_socket->readAll().toHex());
    qDebugxxxxx(qtr("收到响应"), m_data_info->get_name(), m_data_info->get_ip_address(), m_data_info->get_port(), QString(m_tcp_socket->readAll().toHex()));
}

void device_ctrl_tcp_base::connected()
{

}

void device_ctrl_tcp_base::disconnected_status()
{
    qDebug() << "device_tcp_disconnect";
    reconnected();
}

void device_ctrl_tcp_base::time_reconnect()
{
    start();
}
