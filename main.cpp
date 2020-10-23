#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSharedPointer>
#include <QTextCodec>

#include "data_model.h"
#include "room_manager.h"
#include "video_item.h"
#include "sy_logger.h"
#include "sy_minidump.h"
#include "data_info.h"
#include "secret_key.h"
#include "acc_conf_info.h"

int main(int argc, char *argv[])
{
#ifdef WIN32
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//注册异常捕获函数
#endif

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("GScontrol");
    QCoreApplication::setOrganizationName("CNC");
    QCoreApplication::setOrganizationDomain("cnc.org");

//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    qmlRegisterType<video_item>("videoitem", 1, 0, "VideoItem");
    qRegisterMetaType<QSharedPointer<data_info>>("QSharedPointer<data_info>");
    qRegisterMetaType<QSharedPointer<acc_conf_info>>("QSharedPointer<acc_conf_info>");


//        int param1=-38;
//        QString a=QString("A5 C3 3C 5A FF 36 04 04 AA AA %1 EE").arg(htons((unsigned int)(param1 *10)), 4, 16, QLatin1Char('0')).replace(" ", "").toUpper();;
//        QString aa=QString("A5 C3 3C 5A FF 36 04 04 AA AA c800 EE");
//        QByteArray b=QByteArray::fromHex(a.toLatin1());
//        qDebug()<<b.toHex();

//        QByteArray vol = b.mid(10, 2);
//        qDebug()<<vol.toHex();
//        qDebug()<<vol.at(0)-vol.at(1);
//        if(vol.at(1)!=0){
//            int n=vol.at(0);
//            int p=vol.at(1);
//            int m=vol.at(1)<<8 ;
//            qDebug()<<n<<p<<m;
//            int iVol = (vol.at(1) << 8 | vol.at(0))/10 ;
//            qDebug()<<iVol;

//        }

//        else{
//            int n=vol.at(0);
//            QString  x=vol.toHex();
//            bool ok;
//            QString hex=x;
//            hex=hex.right(2)+hex.left(2);
//            int diec = hex.toInt(&ok, 16);

//            qDebug()<<diec/10;
//             qDebug()<<x.mid(0,2);

//        }


    QGuiApplication app(argc, argv);
    qlogEx(qtr("启动"));
    QQmlApplicationEngine engine;

    data_model room;
    data_model signal_source;
    data_model signal_mode;
    data_model device;
    data_model signal_source_display;
    data_model signal_mode_image;
    data_model device_ctrl;

    room_manager manager(&room, &signal_source, &signal_mode, &device, &signal_source_display, &signal_mode_image, &device_ctrl);
#ifdef Q_OS_ANDROID
    manager.check_permission("android.permission.WRITE_EXTERNAL_STORAGE");  //写包括读
#endif
    manager.read_ip();

    //启动密钥服务类
    secret_key key;

    engine.rootContext()->setContextProperty("$key", &key);

    engine.rootContext()->setContextProperty("$manager", &manager);
    engine.rootContext()->setContextProperty("$room", &room);
    engine.rootContext()->setContextProperty("$signal_source", &signal_source);
    engine.rootContext()->setContextProperty("$signal_mode_image", &signal_mode_image);
    engine.rootContext()->setContextProperty("$signal_mode", &signal_mode);
    engine.rootContext()->setContextProperty("$device", &device);
    engine.rootContext()->setContextProperty("$device_ctrl", &device_ctrl);
    engine.rootContext()->setContextProperty("$signal_source_display", &signal_source_display);




    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    manager.init_language();
    //启动密钥服务类
    key.start();  //免费版不用授权


    if (engine.rootObjects().isEmpty())
        return -1;
    //初始化语言


    return app.exec();
}
