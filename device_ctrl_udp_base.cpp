#include "device_ctrl_udp_base.h"
#include "sy_logger.h"
#include <QNetworkInterface>
#include <QHostAddress>
device_ctrl_udp_base::device_ctrl_udp_base( QString localIp,int revPort,QString send_ip,int send_port,QObject *parent) : QObject(parent),
    m_udp_socket(nullptr),
    isFirstConnect(true),
    m_timer(nullptr),
    m_ip_address(send_ip),
    m_port(send_port),
    m_rev_port(revPort),
    m_local_ip_address(localIp),
    isInit(true),
    isStop(false),
    reconnectnum(0)
{

}

void device_ctrl_udp_base::start()
{


    if(m_udp_socket != nullptr ){
        return;
    }
    m_udp_socket = new QUdpSocket();

    connect(m_udp_socket, &QUdpSocket::readyRead, this, &device_ctrl_udp_base::recv_data);
//    connect(m_udp_socket, &QUdpSocket::disconnected, this, &device_ctrl_udp_base::disconnected_status);
//    connect(m_udp_socket,&QUdpSocket::stateChanged,this,&device_ctrl_udp_base::onSocketStateChange);

    quint16     port=m_rev_port;


       QString strIpAddress;
       QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
       // 获取第一个本主机的IPv4地址
       int nListSize = ipAddressesList.size();
       for (int i = 0; i < nListSize; ++i)
       {
              if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                  ipAddressesList.at(i).toIPv4Address()) {
                  strIpAddress = ipAddressesList.at(i).toString();
                  break;
              }
        }
        // 如果没有找到，则以本地IP地址为IP
        if (strIpAddress.isEmpty())
           strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();

   QHostAddress    targetAddr(strIpAddress);
    QHostAddress    targetAddr2(m_local_ip_address);
    if (m_udp_socket->bind(targetAddr2,port))//绑定端口成功
    {
        qDebugxxx(QHostAddress::LocalHost+":",port,qtr("绑定成功"));

    }
    else{
        qDebugxxx(QHostAddress::LocalHost+":",port,qtr("绑定失败"));
        return;
    }


    if(m_timer == nullptr) {
        m_timer = new QTimer();
        connect(m_timer, &QTimer::timeout, this, &device_ctrl_udp_base::time_reconnect);
    } else {
        m_timer->stop();
    }


    init_send_cmd("&CXZT#");
    //设备软件操作完关闭，当下次打开软件后，向中控发送按键状态查询指令：&CXZT#      中控一一回复之前操作状态指令。
}

void device_ctrl_udp_base::restart()
{
    if(m_udp_socket != nullptr ){
            return;
        }
        m_udp_socket = new QUdpSocket();

        connect(m_udp_socket, &QUdpSocket::readyRead, this, &device_ctrl_udp_base::recv_data);

        quint16     port=m_rev_port;


           QString strIpAddress;
           QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
           // 获取第一个本主机的IPv4地址
           int nListSize = ipAddressesList.size();
           for (int i = 0; i < nListSize; ++i)
           {
                  if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                      ipAddressesList.at(i).toIPv4Address()) {
                      strIpAddress = ipAddressesList.at(i).toString();
                      break;
                  }
            }
            // 如果没有找到，则以本地IP地址为IP
            if (strIpAddress.isEmpty())
               strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();

       QHostAddress    targetAddr(strIpAddress);
        QHostAddress    targetAddr2(m_local_ip_address);
        if (m_udp_socket->bind(targetAddr2,port))//绑定端口成功
        {
            qDebug()<<targetAddr<<targetAddr2<<m_rev_port<<qtr("断开绑定成功");
            qDebugxxx(QHostAddress::LocalHost+":",port,qtr("断开绑定成功"));

        }
        else{
            qDebug()<<targetAddr<<targetAddr2<<m_rev_port<<qtr("断开绑定失败");
            qDebugxxx(QHostAddress::LocalHost+":",port,qtr("断开绑定失败"));
            return;
        }

}

void device_ctrl_udp_base::stop()
{
//    qDebug()<<"udp stop";

    if(m_udp_socket != nullptr){
        m_udp_socket->abort();
        m_udp_socket->close();
        delete m_udp_socket;
        m_udp_socket = nullptr;
    }
    if(m_timer != nullptr) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
}
//void device_ctrl_udp_base::stop()
//{
////    qDebug()<<"udp stop";
////    m_udp_socket->blockSignals(true);
//    isStop=true;
//    if(m_udp_socket != nullptr){
//        m_udp_socket->close();
//        delete m_udp_socket;
//        m_udp_socket = nullptr;
//    }
//    if(m_timer != nullptr) {
//        m_timer->stop();
//        delete m_timer;
//        m_timer = nullptr;
//    }
//}
void device_ctrl_udp_base::reconnected()
{
    isFirstConnect = false;
    m_udp_socket->blockSignals(true);
    if(m_udp_socket != nullptr) {
        m_udp_socket->close();
        m_udp_socket->deleteLater();
        m_udp_socket = nullptr;
    }
    if(m_timer != nullptr) {
        m_timer->start(3000);
    }
}

void device_ctrl_udp_base::send_cmd(const QString &cmd,data_info *data)
{
    qDebug()<<cmd;
    qDebugxx(qtr("执行指令"),cmd);
    QString     targetIP=data->get_ip_address(); //目标IP
    QHostAddress    targetAddr(targetIP);

    quint16     targetPort=data->get_port();//目标port

    QString  msg=cmd;//发送的消息内容
    QByteArray send_data = QByteArray::fromHex(msg.toLatin1());

    if(data->get_cmd_type() == "ascii"){ //屏幕设备
        send_data = msg.toLatin1();
    }
//    QByteArray  str=msg.toUtf8();
    int temppp=m_udp_socket->writeDatagram(send_data,targetAddr,targetPort); //发出数据报
    if(temppp==-1){
            //发送错误，重新连接
            reconnectnum++;
            m_udp_socket->blockSignals(true);
            if(m_udp_socket != nullptr) {
                m_udp_socket->abort();
                m_udp_socket->deleteLater();
                m_udp_socket = nullptr;
            }
            restart();
            if(reconnectnum<=3)
              device_ctrl_slot(cmd,data);
            return;
        }
    reconnectnum=0;
}

void device_ctrl_udp_base::init_send_cmd(QString cmd)
{
    QString     targetIP=m_ip_address; //目标IP
    QHostAddress    targetAddr(targetIP);

    quint16     targetPort=m_port;//目标port


  //  QByteArray send_data = QByteArray::fromHex(cmd.toLatin1());
    QByteArray send_data=cmd.toUtf8();
//    qDebug()<<cmd<<send_data<<send_data.toHex();
    m_udp_socket->writeDatagram(send_data,targetAddr,targetPort); //发出数据报
}

void device_ctrl_udp_base::recv_data()
{
    while(m_udp_socket->hasPendingDatagrams())
    {
//不能用下面的方法读取发送端的IP地址和端口
//        QString peerAddr=udpSocket->peerAddress().toString();
//        quint16 peerPort=udpSocket->peerPort();
//        QString peer="[From "+peerAddr+":"+QString::number(peerPort)+"] ";

        QByteArray   datagram;
        datagram.resize(m_udp_socket->pendingDatagramSize());

        QHostAddress    peerAddr;
        quint16 peerPort;
        m_udp_socket->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);
        QString str=datagram.toHex();

        QString peer="[From "+peerAddr.toString()+":"+QString::number(peerPort)+"] "+str;
//        qDebug()<<peer;
        if(isInit){
            emit init_device_number_return_information(str);
            isInit=!isInit;
            return;
        }
        else

           emit device_number_return_information(str);


    }
}

void device_ctrl_udp_base::connected()
{

}

void device_ctrl_udp_base::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
    {
        qDebug()<<(QStringLiteral("udp scoket状态：UnconnectedState"));
//           emit login_status(false,IP, PORT, usr, pwd);
          if(!isStop)
          {

              qDebug()<<"xi ping disconnect";
          }
          isStop=false;
    }
    break;
    case QAbstractSocket::HostLookupState:
        qDebug()<<(QStringLiteral("udp scoket状态：HostLookupState"));
        break;
    case QAbstractSocket::ConnectingState:
        qDebug()<<(QStringLiteral("udp scoket状态：ConnectingState"));
        break;

    case QAbstractSocket::ConnectedState:
        qDebug()<<(QStringLiteral("udp scoket状态：ConnectedState"));
        break;

    case QAbstractSocket::BoundState:
        qDebug()<<(QStringLiteral("udp scoket状态：BoundState"));
        break;

    case QAbstractSocket::ClosingState:
        qDebug()<<(QStringLiteral("udp scoket状态：ClosingState"));
        break;

    case QAbstractSocket::ListeningState:
        qDebug()<<(QStringLiteral("udp scoket状态：ListeningState"));
    }
}

void device_ctrl_udp_base::device_ctrl_slot(QString cmd,data_info *data)
{


    qDebugxxx(qtr("执行指令"),cmd, data->get_channel_no());
    qDebug()<<__FUNCTION__<<cmd;

    QHostAddress    targetAddr(data->get_ip_address()); //目标IP

    quint16     targetPort=data->get_port();//目标port

    QString  msg=cmd;//发送的消息内容
    QByteArray send_data = QByteArray::fromHex(msg.toLatin1());

    if(data->get_cmd_type() == "ascii"){ //屏幕设备
        send_data = msg.toLatin1();
    }
//    QByteArray  str=msg.toUtf8();
//    qDebug()<<targetAddr<<targetPort<<"send";
    int temppp=m_udp_socket->writeDatagram(send_data,targetAddr,targetPort); //发出数据报
    if(temppp==-1){
            //发送错误，重新连接
            reconnectnum++;
            m_udp_socket->blockSignals(true);
            if(m_udp_socket != nullptr) {
                m_udp_socket->abort();
                m_udp_socket->deleteLater();
                m_udp_socket = nullptr;
            }
            restart();
            if(reconnectnum<=3)
              device_ctrl_slot(cmd,data);
            return;
        }
    reconnectnum=0;

}

void device_ctrl_udp_base::disconnected_status()
{
    qDebug() << "device_udp_disconnect";
    reconnected();
}

void device_ctrl_udp_base::time_reconnect()
{
         start();
}

//void device_ctrl_udp_base::reconnected()
//{
//    isFirstConnect = false;
//    m_udp_socket->blockSignals(true);
//    if(m_udp_socket != nullptr) {
//        m_udp_socket->abort();
//        m_udp_socket->deleteLater();
//        m_udp_socket = nullptr;
//    }
//    if(m_timer != nullptr) {
//        m_timer->start(3000);
//    }
//}
