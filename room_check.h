#ifndef ROOM_CHECK_H
#define ROOM_CHECK_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "data_info.h"
class room_check : public QObject
{
    Q_OBJECT
public:
    explicit room_check(QObject *parent = nullptr);
    ~room_check();
    void start(QString,QString,QString,QString);
    void start_check_conf(QList<QSharedPointer<data_info>> val);//检查配置文件中的场景是否能够登录
    void start_check_conf_one(QSharedPointer<data_info> room);//检查配置文件中的场景是否能够登录

    void stop();
    void stopconnect();
    int get_currentindex();
    void coni_check_conf_one(int index); //继续下个场景的检验
signals:
    void room_check_msg(int num,QString  msg);
    void return_scene(QList<QSharedPointer<data_info>> val);
    void login_status(bool hint,QString ip, QString port, QString username, QString password);
    void login_status_conf(int hint);
public slots:
    void connected();
    void recv_data();
    void time_reconnect();
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void time_out();
    void time_out_conf();
private:
    QTcpSocket *m_tcp_socket;
    QTimer *m_timer;
    QByteArray get_response();
    QByteArray get_response2();

    void login();
    void get_scene();
    QString usr;
    QString pwd;
    QString IP;
    QString PORT;
    void reconnected();
    QList<QSharedPointer<data_info>> m_list_scene;
    void add_cmd_header(QByteArray &data, int cmd, int len);
    int m_packet_id;
    QTimer *tiemr;
    QList<QSharedPointer<data_info>> vall;
    int isConfig;
    int current_index;
    int len;
    int  first_can_login;

   QTimer *tiemr_conf;
};

#endif // ROOM_CHECK_H
