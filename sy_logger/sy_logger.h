#ifndef SY_LOGGER_H
#define SY_LOGGER_H

#include <QString>
#include <QReadWriteLock>
#include <QMutex>
#include <QScopedPointer>


#ifndef QSL
#define QSL QStringLiteral
#endif

#define qzhstr(x) QString(QStringLiteral(x))
#define qtr(x) QString(QStringLiteral(x))
#define qlog(x) QString("%1:%2:%3").arg(x).arg(__FUNCTION__).arg(__LINE__)
#define qDebugEx qDebug()<<__FILE__<<__LINE__//<<__func__

#define qlogEx(x) sy_logger::getInstance()->log_info(qlog(QString("%1").arg(x)))
#define qlogExx(x, y) sy_logger::getInstance()->log_info(qlog(QString("%1:%2").arg(x).arg(y)))
#define qlogExxx(x, y, z) sy_logger::getInstance()->log_info(qlog(QString("%1:%2:%3").arg(x).arg(y).arg(z)))
#define qlogExxxx(x, y, z, a) sy_logger::getInstance()->log_info(qlog(QString("%1:%2:%3:%4").arg(x).arg(y).arg(z).arg(a)))

#define qErrx(x) sy_logger::getInstance()->log_err(qlog(QString("%1").arg(x)))
#define qErrxx(x, y) sy_logger::getInstance()->log_err(qlog(QString("%1:%2").arg(x).arg(y)))
#define qErrxxx(x, y, z) sy_logger::getInstance()->log_err(qlog(QString("%1:%2:%3").arg(x).arg(y).arg(z)))

#define qWarnx(x) sy_logger::getInstance()->log_warn(qlog(QString("%1").arg(x)))
#define qWarnxx(x, y) sy_logger::getInstance()->log_warn(qlog(QString("%1:%2").arg(x).arg(y)))
#define qWarnxxx(x, y, z) sy_logger::getInstance()->log_warn(qlog(QString("%1:%2:%3").arg(x).arg(y).arg(z)))
#define qWarnxxxx(x, y, z, a) sy_logger::getInstance()->log_warn(qlog(QString("%1:%2:%3:%4").arg(x).arg(y).arg(z).arg(a)))
#define qWarnxxxxx(x, y, z, a, b) sy_logger::getInstance()->log_warn(qlog(QString("%1:%2:%3:%4:%5").arg(x).arg(y).arg(z).arg(a).arg(b)))
#define qWarnxxxxxx(x, y, z, a, b, c) sy_logger::getInstance()->log_warn(qlog(QString("%1:%2:%3:%4:%5:%6").arg(x).arg(y).arg(z).arg(a).arg(b).arg(c)))
#define qWarnxxxxxxx(x, y, z, a, b, c, d) sy_logger::getInstance()->log_warn(qlog(QString("%1:%2:%3:%4:%5:%6:%7").arg(x).arg(y).arg(z).arg(a).arg(b).arg(c).arg(d)))


#define qDebugx(x) sy_logger::getInstance()->log_debug(qlog(QString("%1").arg(x)))
#define qDebugxx(x, y) sy_logger::getInstance()->log_debug(qlog(QString("%1:%2").arg(x).arg(y)))
#define qDebugxxx(x, y, z) sy_logger::getInstance()->log_debug(qlog(QString("%1:%2:%3").arg(x).arg(y).arg(z)))
#define qDebugxxxx(x, y, z, a) sy_logger::getInstance()->log_debug(qlog(QString("%1:%2:%3:%4").arg(x).arg(y).arg(z).arg(a)))
#define qDebugxxxxx(x, y, z, a, b) sy_logger::getInstance()->log_debug(qlog(QString("%1:%2:%3:%4:%5").arg(x).arg(y).arg(z).arg(a).arg(b)))
#define qDebugxxxxxx(x, y, z, a, b, c) sy_logger::getInstance()->log_debug(qlog(QString("%1:%2:%3:%4:%5:%6").arg(x).arg(y).arg(z).arg(a).arg(b).arg(c)))
#define qDebugxxxxxxx(x, y, z, a, b, c, d) sy_logger::getInstance()->log_debug(qlog(QString("%1:%2:%3:%4:%5:%6:%7").arg(x).arg(y).arg(z).arg(a).arg(b).arg(c).arg(d)))
#define qDebugxxxxxxxx(x, y, z, a, b, c, d, e) sy_logger::getInstance()->log_debug(qlog(QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(x).arg(y).arg(z).arg(a).arg(b).arg(c).arg(d).arg(e)))


#define qLostxxxxxxxx(x, y, z, a, b, c, d, e) sy_logger::getInstance()->log_package_lost(qlog(QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(x).arg(y).arg(z).arg(a).arg(b).arg(c).arg(d).arg(e)))
#define qLostxxxxxxx(x, y, z, a, b, c, d) sy_logger::getInstance()->log_package_lost(qlog(QString("%1:%2:%3:%4:%5:%6:%7").arg(x).arg(y).arg(z).arg(a).arg(b).arg(c).arg(d)))
#define qLostxxxxxx(x, y, z, a, b, c) sy_logger::getInstance()->log_package_lost(qlog(QString("%1:%2:%3:%4:%5:%6").arg(x).arg(y).arg(z).arg(a).arg(b).arg(c)))
#define qLostxxxxx(x, y, z, a, b) sy_logger::getInstance()->log_package_lost(qlog(QString("%1:%2:%3:%4:%5").arg(x).arg(y).arg(z).arg(a).arg(b)))
#define qLostxxxx(x, y, z, a) sy_logger::getInstance()->log_package_lost(qlog(QString("%1:%2:%3:%4").arg(x).arg(y).arg(z).arg(a)))
#define qLostxxx(x, y, z) sy_logger::getInstance()->log_package_lost(qlog(QString("%1:%2:%3").arg(x).arg(y).arg(z)))



class sy_logger
{
private:
    sy_logger();

public:

    static bool is_log_debug;
    static bool is_log_info;
    static bool is_log_err;
    static bool is_log_warn;
    static bool is_log_package_lost;

    static sy_logger *getInstance()
    {
        static QMutex mutex;
        static QScopedPointer<sy_logger> inst;
        if(Q_UNLIKELY(!inst)){
            mutex.lock();
            if(!inst){
                inst.reset(new sy_logger());
            }
        }
        return inst.data();
    }

    void load_logger_conf();
    void log_info(const QString message);
    void log_debug(const QString message);
    void log_warn(const QString message);
    void log_err(const QString message);
    void log_package_lost(const QString message);


};

#endif // sy_LOGGER_H
