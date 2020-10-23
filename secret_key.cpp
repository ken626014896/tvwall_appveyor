#include "secret_key.h"
#include "qmutex.h"
#include "qfile.h"
#include "qtimer.h"
#include "qdatetime.h"
#include "qapplication.h"
#include <QDebug>

#include "sy_utils.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QProcess>
#include <QSettings>
#include <QDir>
#include <QStandardPaths>
secret_key::secret_key(QObject *parent) : QObject(parent)
{
    keyData = "";
    keyUseDate = false;
    keyDate = "2017-01-01";
    keyUseRun = false;
    keyRun = 1;
    keyUseCount = false;
    keyCount = 10;
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkTime()));
    startTime = QDateTime::currentDateTime();
#ifdef Q_OS_WIN32
    idkey = QString("%1|%2").arg(getCpuId()).arg(getDiskNum());

#endif

#ifdef Q_OS_ANDROID
    idkey=getKey();
#endif


    idkey=getFormat(getXorEncryptDecrypt(idkey, 110).toLatin1().toBase64().toHex());
}

secret_key::~secret_key()
{
    delete  timer;
}

void secret_key::set_num(QString val)
{

        QStringList templist = val.toUpper().split("-");
        if(templist.length()!=8){


           emit error_msg(QStringLiteral("请输入正确的序列号"));
            return;
        }
//        for (int i=0;i<templist.length();i++) {
//            if(templist.at(i).length()!=4){
//                emit error_msg(QStringLiteral("请输入正确的序列号"));
//                return;
//            }

//        }


        QString temp=templist.join("").toLower();


        QByteArray keyByte=QByteArray::fromHex(temp.toLatin1());
        QString key = QByteArray::fromBase64(keyByte);
        key=getXorEncryptDecrypt(key,110);
        qDebug()<<key;
        QStringList data = key.split("|");
        if (data.count() != 4) {
            emit error_msg(QStringLiteral("序列号已损坏,请联系供应商更新注册码！"));
             return;
        }

        keyUseDate = (data.at(0) == "1" ? true : false);
        qDebug()<<keyUseDate;
        keyDate = data.at(1);
        QString nowDate = QDate::currentDate().toString("yyyy-MM-dd");
        if(keyUseDate)
        if (nowDate > keyDate) {
            emit error_msg(QStringLiteral("序列号已到期,请联系供应商更新注册码！"));

            return;
        }
        //如果序列号有效，开始生成密匙文件
        QString fileName = QApplication::applicationDirPath() + "/key.db";
         QDir dir;
#ifdef Q_OS_ANDROID

        dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
        fileName = dir.absoluteFilePath(QString("%1").arg("key.db"));

#endif
       QFile file(fileName);
       file.open(QFile::WriteOnly | QIODevice::Text);
       file.write(getXorEncryptDecrypt(key, 110).toLatin1().toBase64());
       file.close();
       emit success_msg();

}

void secret_key::try_30_days()
{

    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t(); //单位为秒
    //设置试用过的标记
    QDir dir;
    QString configFile="config.ini";
#ifdef Q_OS_ANDROID
            //安卓版本，配置文件不存在时，从资源文件中读取初始配置
            dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
            configFile = dir.absoluteFilePath(QString("%1").arg("config.ini"));

#endif
    QSettings *configIniRead = new QSettings(configFile, QSettings::IniFormat);
    QString nowDate = QDate::currentDate().toString("yyyy-MM-dd");
    QString idkey = QString("%1|%2").arg(true).arg(nowDate);
    idkey=getXorEncryptDecrypt(idkey, 110).toLatin1().toBase64().toHex();
    configIniRead->setValue("other/temp",idkey);
    //将当前时间转为时间戳
    timeT=timeT+2592000;
    QDateTime time_1 = QDateTime::fromTime_t(timeT); //将时间戳转为时间
    QString timeee = time_1.date().toString("yyyy-MM-dd");
    QString newkey = QString("%1|%2|%3|%4").arg(true).arg(timeee).arg(getCpuId()).arg(getDiskNum());
    QString fileName = QApplication::applicationDirPath() + "/key.db";
#ifdef Q_OS_ANDROID

            dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
            fileName = dir.absoluteFilePath(QString("%1").arg("key.db"));

#endif
    QFile file(fileName);
    file.open(QFile::WriteOnly | QIODevice::Text);
    file.write(getXorEncryptDecrypt(newkey, 110).toLatin1().toBase64());
    file.close();
    delete configIniRead;
    emit try_success_msg();



}

void secret_key::activation()
{
    idkey = QString("%1|%2").arg(getCpuId()).arg(getDiskNum());
    idkey=getFormat(getXorEncryptDecrypt(idkey, 110).toLatin1().toBase64().toHex());
    emit activation_msg(idkey);
}

QString secret_key::getCpuId()
{
#ifdef Q_OS_ANDROID
    return getKeyOne(true);
#endif
    return getWMIC("wmic cpu get processorid");
}

QString secret_key::getDiskNum()
{
#ifdef Q_OS_ANDROID
    return getKeyOne(false);
#endif
    return getWMIC("wmic diskdrive where index=0 get serialnumber");
}

QString secret_key::getWMIC(const QString &cmd)
{
    //获取cpu名称：wmic cpu get Name
    //获取cpu核心数：wmic cpu get NumberOfCores
    //获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
    //查询cpu序列号：wmic cpu get processorid
    //查询主板序列号：wmic baseboard get serialnumber
    //查询BIOS序列号：wmic bios get serialnumber
    //查看硬盘：wmic diskdrive get serialnumber
    QProcess p;
    p.start(cmd);
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    QStringList list = cmd.split(" ");
    result = result.remove(list.last(), Qt::CaseInsensitive);
    result = result.replace("\r", "");
    result = result.replace("\n", "");
    result = result.simplified();
//    qDebug()<<result;
    return result;
}

QString secret_key::getFormat(QString str)
{
    QString temp=str.toUpper();

    for(int i=str.length()/8;i<str.length();i=i+str.length()/8+1){

        temp.insert(i,'-');
    }

    return temp;
}

QString secret_key::getKey()
{

    QDir dir;
    dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    QString configFile = dir.absoluteFilePath(QString("%1").arg("config.ini"));
    QSettings *configIniRead = new QSettings(configFile, QSettings::IniFormat);
    QString keyResult = configIniRead->value("/other/uuid").toString();
    if(keyResult==nullptr){
        QString key=getUuid();
        key = QString("%1|%2").arg(key.mid(0,key.length()/2)).arg(key.mid(key.length()/2,key.length()));
        configIniRead->setValue("/other/uuid", key);
        return key;
    }
    delete  configIniRead;
    return keyResult;



}

QString secret_key::getKeyOne(bool isLeft)
{
    QDir dir;
    dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    QString configFile = dir.absoluteFilePath(QString("%1").arg("config.ini"));
    QSettings *configIniRead = new QSettings(configFile, QSettings::IniFormat);
    QString keyResult = configIniRead->value("/other/uuid").toString();
    if(keyResult==nullptr){

        return QString();
    }
    QStringList  s=keyResult.split("|");
   delete  configIniRead;
   if(isLeft)
       return s.at(0);

   else {
       return s.at(1);
   }

}

void secret_key::checkTime()
{
    // 找出当前时间与首次启动时间比较
    QDateTime now = QDateTime::currentDateTime();
    if (startTime.secsTo(now) >= (keyRun * 60)) {
//        QMessageBox::critical(0, "错误", QStringLiteral("试运行时间已到,请联系供应商更新注册码!"));
//        exit(0);
        emit send_msg(false,QStringLiteral("软件已到时,请联系供应商更新注册码!"));
    }
}



QString secret_key::getXorEncryptDecrypt(const QString& data, char key)
{
    // 采用异或加密,也可以自行更改算法
    QByteArray buffer = data.toLatin1();
    int size = buffer.size();
    for (int i = 0; i < size; i++) {
        buffer[i] = buffer.at(i) ^ key;
    }
    return QLatin1String(buffer);
}

void secret_key::getNetTime()
{
    QUrl newUrl ("http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp");//请求地址
    QNetworkRequest request;
    request.setUrl(newUrl);
    reply = networkManager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(get_reply()));
}

void secret_key::start()
{
    // 判断密钥文件是否存在,不存在则从资源文件复制出来,同时需要设置文件写权限
//    qDebug() << qApp->applicationDirPath();
    QDir dir;
    QString configFile="config.ini";
#ifdef Q_OS_ANDROID

            dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
            configFile = dir.absoluteFilePath(QString("%1").arg("config.ini"));

#endif
    QSettings *configIniRead = new QSettings(configFile, QSettings::IniFormat);

#ifdef Q_OS_ANDROID

    if(!QFile::exists(configFile)) {
        QString nowDate = QDate::currentDate().toString("yyyy-MM-dd");
        QString idkey = QString("%1|%2").arg(false).arg(nowDate);
        idkey=getXorEncryptDecrypt(idkey, 110).toLatin1().toBase64().toHex();
        configIniRead->setValue("/other/temp", idkey);

    }

#endif

    QString ipResult = configIniRead->value("other/temp").toString();

    if(ipResult == nullptr) {

//        emit send_msg(false,QStringLiteral("config.ini文件错误,请联系供应商!"));
        emit send_msg(false,"1");

        configIniRead->setValue("/other/temp", idkey);
//        emit hide_try_btn(2);
        return;
    }

    delete configIniRead;
    QByteArray tryByte=QByteArray::fromHex(ipResult.toLatin1());
    QString tryData = QByteArray::fromBase64(tryByte);
    tryData=getXorEncryptDecrypt(tryData,110);
    QStringList tryDataList = tryData.split("|");
    bool isTry=(tryDataList.at(0) == "1" ? true : false);//false 代表没有试用过  true代表使用过




    QString keyName = qApp->applicationDirPath() + "/key.db";
#ifdef Q_OS_ANDROID

            dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
            keyName = dir.absoluteFilePath(QString("%1").arg("key.db"));

#endif
    QFile   keyFile(keyName);
    if (!keyFile.exists() || (keyFile.size() == 0)) {
        if(!isTry)  //代表第一次使用软件，提供试用机会
           emit send_msg(true,idkey);

        else {     //key.db不存在，但istry为true，即使用过，代表用户恶意删除key文件。不提供试用功能

           emit send_msg(true,idkey);
           emit hide_try_btn(1);
        }

    } else {
        // 读取密钥文件
        keyFile.open(QFile::ReadOnly);
        //    keyData = keyFile.readLine();
        keyData = QByteArray::fromBase64(keyFile.readLine()); // 使用base64解密
        keyFile.close();
        // 将从注册码文件中的密文解密,与当前时间比较是否到期
        keyData = getXorEncryptDecrypt(keyData, 110);
        QStringList data = keyData.split("|");
        if (data.count() != 4) {
            //        QMessageBox::critical(0, "错误",  QStringLiteral("注册码文件已损坏,程序将自动关闭!"));
//            emit send_msg(false,QStringLiteral("密匙文件错误已损坏,请联系供应商获取新的密钥!"));
            emit send_msg(false,"2");
            //        exit(0);
        }
        keyUseDate = (data.at(0) == "1" ? true : false);
        keyDate = data.at(1);
//        qDebug()<<data.at(2);
//        qDebug()<<data.at(3);
        if(data.at(2)!=getCpuId()){
//            qDebug()<<data.at(2);
//            emit send_msg(false,QStringLiteral("密匙文件错误,请联系供应商获取新的密钥!"));
            emit send_msg(false,"3");
            return;
        }
        if(data.at(3)!=getDiskNum()){
//            qDebug()<<data.at(3);
//            emit send_msg(false,QStringLiteral("密匙文件错误,请联系供应商获取新的密钥!"));
            emit send_msg(false,"3");
            return;
        }
        // 如果启用了时间限制
        if (keyUseDate) {
//            QString nowDate = QDate::currentDate().toString("yyyy-MM-dd");
//            if (nowDate > keyDate) {
//                //            QMessageBox::critical(0, "错误",
//                // QStringLiteral("软件已到期,请联系供应商更新注册码!"));
//                //            exit(0);
//                emit send_msg(QStringLiteral("软件已到期,请联系供应商更新注册码!"));
//            }
            getNetTime();//如果可以获取网络时间，就使用网络时间对比
        }

    }

}

void secret_key::stop()
{
    timer->stop();
}


void secret_key::get_reply()  //获取网络时间    //软件过期，要重新回到序列号页面！！！！
{

    QByteArray bytes = reply->readAll();
    if(bytes.toHex() == "") {
        qDebug() << "local time";
        QString nowDate = QDate::currentDate().toString("yyyy-MM-dd");
        if (nowDate > keyDate) {
            idkey = QString("%1|%2").arg(getCpuId()).arg(getDiskNum());
            idkey=getFormat(getXorEncryptDecrypt(idkey, 110).toLatin1().toBase64().toHex());
            emit send_msg_again(idkey);
            return;
        }

    }
    qDebug() << "net time";
    QJsonParseError jsonError;
    QJsonDocument jsonDoucment = QJsonDocument::fromJson(bytes, &jsonError);
    if(!(jsonDoucment.isNull() || jsonDoucment.isEmpty())) {
        QVariantMap dataMap = jsonDoucment.toVariant().toMap();
        QVariantMap b = dataMap["data"].toMap();
        QString str = b["t"].toString();
        int  timestamp = str.mid(0, 10).toInt() + 1;
//        qDebug() << timestamp;
        QDateTime time_1 = QDateTime::fromTime_t(timestamp); //将时间戳转为时间
        QString timeee = time_1.date().toString("yyyy-MM-dd");
        qDebug()<<timeee<<keyDate;
        if (timeee > keyDate) {

            idkey = QString("%1|%2").arg(getCpuId()).arg(getDiskNum());
            idkey=getFormat(getXorEncryptDecrypt(idkey, 110).toLatin1().toBase64().toHex());
            emit send_msg_again(idkey);
        }
    }
}
