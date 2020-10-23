#ifndef SECRET_KEY_H
#define SECRET_KEY_H

#include <QDateTime>
#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
class QTimer;
class secret_key:public QObject
{
    Q_OBJECT

public:
    static secret_key *Instance();
    explicit secret_key(QObject *parent = 0);
    ~secret_key();
    Q_INVOKABLE void set_num(QString val);
    Q_INVOKABLE void try_30_days();
    Q_INVOKABLE void activation(); //试用后，想激活
    QString getCpuId();
    QString getDiskNum();
    QString getWMIC(const QString &cmd);
    QString getFormat(QString str);

    QString getKey();//安卓和ios使用uuid作为标记
    QString getKeyOne(bool isLeft);   //获取uuid不同部分
private:
    static secret_key *self;

    QString keyData;            //注册码密文
    bool keyUseDate;            //是否启用运行日期时间限制
    QString keyDate;            //到期时间字符串
    bool keyUseRun;             //是否启用可运行时间限制
    int keyRun;                 //可运行时间
    bool keyUseCount;           //是否启用设备数量限制
    int keyCount;               //设备限制数量

    QTimer *timer;              //定时器判断是否运行超时
    QDateTime startTime;        //程序启动时间

    QString idkey;
    QNetworkAccessManager networkManager;//网络管理
    QNetworkReply *reply;

signals:
    void send_msg(bool first,QString msg);
    void send_msg_again(QString key);
    void error_msg(QString msg);
    void success_msg();
    void try_success_msg();
    void activation_msg(QString msg);
    void hide_try_btn(int hint);

private slots:
    void checkTime();
    QString getXorEncryptDecrypt(const QString &data, char key);
    void getNetTime();

public slots:
    void start();
    void stop();
    void get_reply();
};

#endif // SECRET_KEY_H
