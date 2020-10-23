#include "sy_logger.h"

#include <QDir>
#include <QCoreApplication>
#include <QTextStream>
#include <QTextCodec>
#include <QtDebug>
#include <QStandardPaths>

#include "log4qt/logger.h"
#include "log4qt/basicconfigurator.h"
#include "log4qt/fileappender.h"
#include "log4qt/propertyconfigurator.h"
#include "log4qt/ttcclayout.h"
#include "log4qt/rollingfileappender.h"
#include "sy_utils.h"
#include "sy_global.h"

bool sy_logger::is_log_debug = true;
bool sy_logger::is_log_info = true;
bool sy_logger::is_log_err = true;
bool sy_logger::is_log_warn = true;
bool sy_logger::is_log_package_lost = false;

sy_logger::sy_logger()
{
    load_logger_conf();
}




void sy_logger::load_logger_conf()
{
    QDir appdir = directoryOf(qApp->applicationDirPath());
    QDir dir = directoryOf(appdir.absoluteFilePath("conf"));
    QString configFile = dir.absoluteFilePath(QString("%1.%2").arg("log4j.properties").arg(SY_APP_TYPE));

    qDebugEx << configFile;

    if(QFile::exists(configFile)){
        QFile file(configFile);
        QString property = "";
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            QString line;
            int line_number = 0;
            do {
                line = trimLeft(stream.readLine());
                line_number++;
                if (line.contains("log4j.appender.LogFile.File="))
                {
                    int idx = line.lastIndexOf('/');
                    QString log_file_name = line.right(line.length() - idx - 1);
                    line = "log4j.appender.LogFile.File="+appdir.absoluteFilePath(QString("logs/%1").arg(log_file_name));
                }
                property = property + line + "\r\n";
            }
            while (!line.isNull());
            file.flush();
            file.close();
        }
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << property;
            file.flush();
            file.close();
        }
        Log4Qt::PropertyConfigurator::configure(configFile);
        Log4Qt::Logger *logger = Log4Qt::Logger::rootLogger();
        QList<Log4Qt::Appender *> apps = logger->appenders();
        foreach(Log4Qt::Appender *app, apps)
        {
            Log4Qt::FileAppender *wa = (Log4Qt::FileAppender *)app;
            if(wa != NULL)
            {
                wa->setEncoding(QTextCodec::codecForLocale());
            }
        }
    }
    else{
        Log4Qt::Logger *logger = Log4Qt::Logger::rootLogger();
        Log4Qt::TTCCLayout *layout = new Log4Qt::TTCCLayout();
        layout->setDateFormat("yyyy-MM-dd HH:mm:ss");
        layout->activateOptions();

        // 创建ConsoleAppender
        Log4Qt::RollingFileAppender *appender = new Log4Qt::RollingFileAppender;
        // 设置输出目的地为应用程序所在目录下的*.log
#ifdef Q_OS_ANDROID
        appender->setFile(QString("%2/%1.log").arg(SY_APP_TYPE).arg(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)));
#else
        appender->setFile(QString("logs/%1.log").arg(SY_APP_TYPE));
#endif
        // 设置日志为追加方式写入输出文件
        appender->setAppendFile(true);
        // 设置备份文件的最大数量为10个
        appender->setMaxBackupIndex(10);
        // 设置输出文件的最大值为20MB
        appender->setMaxFileSize("20MB");
        appender->setLayout(layout);
        // 设置编码
//        appender->setEncoding(QTextCodec::codecForName("UTF-8"));
        appender->setEncoding(QTextCodec::codecForLocale());

        appender->setImmediateFlush(true);
        // 设置阈值级别为INFO
        appender->setThreshold(Log4Qt::Level::DEBUG_INT);
        // 激活选项
        appender->activateOptions();
        logger->addAppender(appender);
        // 设置级别为 DEBUG
        logger->setLevel(Log4Qt::Level::DEBUG_INT);
    }

}

void sy_logger::log_info(const QString message)
{
    if(!is_log_info){
        return;
    }
    Log4Qt::Logger * log = Log4Qt::Logger::rootLogger();
    QString msg = message;
    log->info(msg.replace("\n", " ").replace("\r", " "));
}

void sy_logger::log_debug(const QString message)
{
    if(!is_log_debug){
        return;
    }
    Log4Qt::Logger * log = Log4Qt::Logger::rootLogger();
    QString msg = message;
    log->debug(msg.replace("\n", " ").replace("\r", " "));
}

void sy_logger::log_warn(const QString message)
{
    if(!is_log_warn){
        return;
    }
    Log4Qt::Logger * log = Log4Qt::Logger::rootLogger();
    QString msg = message;
    log->warn(msg.replace("\n", " ").replace("\r", " "));
}

void sy_logger::log_err(const QString message)
{
    if(!is_log_err){
        return;
    }
    Log4Qt::Logger * log = Log4Qt::Logger::rootLogger();
    QString msg = message;
    log->error(msg.replace("\n", " ").replace("\r", " "));
}

void sy_logger::log_package_lost(const QString message)
{
    if(!is_log_package_lost){
        return;
    }
    Log4Qt::Logger * log = Log4Qt::Logger::rootLogger();
    QString msg = message;
    log->debug(msg.replace("\n", " ").replace("\r", " "));
}
