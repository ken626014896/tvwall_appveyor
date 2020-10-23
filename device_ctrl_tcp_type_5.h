#ifndef DEVICE_CTRL_TCP_TYPE_5_H
#define DEVICE_CTRL_TCP_TYPE_5_H

#include "acc_conf_info.h"
#include "device_ctrl_base.h"
#include "data_info.h"
#include "device_ctrl_tcp_base.h"

#include <QObject>
#include <QTcpSocket>
#include <QHash>
#include <QSharedPointer>
#include <QMutexLocker>
#include <QMutex>
#include <QTimer>

class device_ctrl_tcp_type_5 : public device_ctrl_tcp_base
{
    Q_OBJECT
public:
    explicit device_ctrl_tcp_type_5(data_info *data, QSharedPointer<acc_conf_info> conf, QObject *parent = nullptr);
    ~device_ctrl_tcp_type_5();



    bool send_cmd(const QString &cmd);
    void start();
    void stop();

    static QSharedPointer<device_ctrl_base> get_device_ctrl(const QString key, data_info *data, QSharedPointer<acc_conf_info> conf);
    void set_acc_conf_info(QSharedPointer<acc_conf_info> conf);
signals:

public slots:
    void device_ctrl_slot(QString cmd, int bit);
    void device_ctrl_slot2(QString cmd, int bit,QSharedPointer<acc_conf_info> conf,QString tempStr);


protected slots:
    void recv_data();
    void connected();

private slots:
    void heatbeat_slot();
    void disconnected_status();
    void time_reconnect();
private:
    bool m_dev_status;
    QMutex m_mutex_ctrl;
    QTimer *m_timer;
    bool isFirstConnect;
    QTimer *tcp_timer;
    static QMutex   m_mutex;
    static QHash<QString, QSharedPointer<device_ctrl_base>> m_hash_ctrl;
    void reconnected();
};

#endif // DEVICE_CTRL_TCP_TYPE_5_H
