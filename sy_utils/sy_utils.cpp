#include "sy_utils.h"

#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QtDebug>
#include <QUuid>
#include <QStringList>
#include <QJsonArray>
#include <QTime>
#include <QtGlobal>
#include <QThread>
#include <QDateTime>
#include <QTimer>
#include <QEventLoop>
#include <QObject>
#ifdef _WIN32
#include <Windows.h>
#else
#include <stdlib.h>
#endif


void wait_by_event_loop(int msecs) {
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(msecs);
    loop.exec();
}


void add_data_to_byte_array(QByteArray &data, char *val, int len){
    for (int i = 0; i < len ; i++) {
        data.append(val[i]);
    }
}

bool getDiskFreeSpace(QString driver, qlonglong &totalsize, qlonglong &freesize, qlonglong &availablesize)
{
#ifdef _WIN32
    LPCWSTR lpcwstrDriver=(LPCWSTR)driver.utf16();
    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
    {
        qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed." << driver << ":" << GetLastError();
        return false;
    }
    totalsize = (qlonglong) liTotalBytes.QuadPart;///1024/1024/1024;
    freesize = (qlonglong) liTotalFreeBytes.QuadPart;///1024/1024/1024;
    availablesize = (qlonglong) liFreeBytesAvailable.QuadPart;///1024/1024/1024;
#else
    totalsize = 0;
    freesize = 0;
    availablesize = 0;
#endif
    return true;
}



QJsonObject getJsonObjectFromString(const QString jsonString){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string ";
        return QJsonObject();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}
// QJson >> QString
QString getStringFromJsonObject(const QJsonObject& jsonObject){
    return QString(QJsonDocument(jsonObject).toJson());
}


QDir directoryOf(const QString &subdir)
{
    QString dirPath = qApp->applicationDirPath();
    QDir dir(dirPath);

#if defined(Q_OS_WIN)
    if (dir.dirName().toLower() == "debug"
            || dir.dirName().toLower() == "release"
            || dir.dirName().toLower() == "bin")
        dir.cdUp();
#elif defined(Q_OS_MAC)
    if (dir.dirName() == "MacOS") {
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
    }
#endif
    dir.cd(subdir);
    return dir;
}


QString trimLeft(const QString &rLine)
{
    int i = 0;
    while (i < rLine.length() && rLine.at(i).isSpace())
        i++;
    return rLine.right(rLine.length() - i);
}

bool getInt64fromJsonString(const QString jsonString, QString name, qint64 &val)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
    if( jsonDocument.isNull() ){
        return false;
    }
    QJsonObject jsonObject = jsonDocument.object();
    if(!jsonObject.contains(name)){
        return false;
    }
    QJsonValue jsonVal = jsonObject.take(name);
    if(!jsonVal.isDouble()){
        return false;
    }
    val = jsonVal.toInt();
    return true;
}

bool getIntfromJsonString(const QString jsonString, QString name, int &val)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
    if( jsonDocument.isNull() ){
//        qDebug() << "wrong json " << name << " in " << jsonString.toUtf8().data();
        return false;
    }
    QJsonObject jsonObject = jsonDocument.object();
    if(!jsonObject.contains(name)){
//        qDebug() << "no " << name << " in " << jsonString.toUtf8().data();
        return false;
    }
    QJsonValue jsonVal = jsonObject.take(name);
    if(!jsonVal.isDouble()){
//        qDebug() << "no int " << name << " in " << jsonString.toUtf8().data();
        return false;
    }
    val = jsonVal.toInt();
    return true;
}

bool getStringfromJsonString(const QString jsonString, QString name, QString &val)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
    if( jsonDocument.isNull() ){
//        qDebug() << "wrong json " << name << " in " << jsonString.toUtf8().data();
        return false;
    }
    QJsonObject jsonObject = jsonDocument.object();
    if(!jsonObject.contains(name)){
//        qDebug() << "no " << name << " in " << jsonString.toUtf8().data();
        return false;
    }
    QJsonValue jsonVal = jsonObject.take(name);
    if(!jsonVal.isString()){
//        qDebug() << "no string " << name << " in " << jsonString.toUtf8().data();
        return false;
    }
    val = jsonVal.toString();
    return true;
}

bool getDoublefromJsonString(const QString jsonString, QString name, double &val)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
    if( jsonDocument.isNull() ){
        return false;
    }
    QJsonObject jsonObject = jsonDocument.object();

    if(!jsonObject.contains(name)){
        return false;
    }
    QJsonValue jsonVal = jsonObject.take(name);
    if(!jsonVal.isDouble()){
        return false;
    }
    val = jsonVal.toDouble();
    return true;
}


bool getQListfromJsonString(const QString jsonString, QString name, QString childName, QList<QString> &val)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
    if( jsonDocument.isNull() ){
        return false;
    }
    QJsonObject jsonObject = jsonDocument.object();

    QJsonValue jsonArrVal = jsonObject.take(name);
    if(!jsonArrVal.isArray()){
        return false;
    }

    QJsonArray jsonArr = jsonArrVal.toArray();
    foreach(QJsonValue jo, jsonArr){
        if(!jo.isObject()){
            continue;
        }
        QJsonObject joVal = jo.toObject();
        QJsonValue jsonVal = joVal.take(childName);
        if(jsonVal.isString()){
            val.append(jsonVal.toString());
        }
    }
    return true;
}

QString getUuid()
{
    QUuid id = QUuid::createUuid();
    QString uuid = id.toString().replace("-", "");
    uuid = uuid.left(uuid.length() - 1);
    uuid = uuid.right(uuid.length() -1);
    return uuid;
}

bool existsDriver(QString filePath)
{
    QFileInfoList list = QDir::drives();
    QString driver = filePath.left(filePath.indexOf('/'));
    if(driver.isEmpty()){
        driver = filePath;
    }
    foreach(QFileInfo fi, list){
        if(fi.path().contains(driver)){
            return true;
        }
    }
    return false;
}
bool createDir(QString filePath)
{
    QDir dir(filePath);
    if(dir.exists())
    {
        return true;
    }

    QString path = dir.absolutePath();
    QString upPath = path.left(path.lastIndexOf('/'));
    if(createDir(upPath))
    {
        return dir.mkdir(dir.absolutePath());
    }
    return false;
}


bool createFileDir(QString fileName)
{
    if(!fileName.contains('/') &&
            !fileName.contains('\\')){
        return true;
    }
    QString path = fileName.left(fileName.lastIndexOf('/'));
    QDir dir(path);
    if(dir.exists())
    {
        return true;
    }
    return dir.mkpath(path);
}


QByteArray safeRecvbyTcp(QTcpSocket *socket, int len)
{
    QByteArray data;
    if(socket == NULL || !socket->isValid())
    {
        return data;
    }
    int restLen = len;
    while(data.length() < len)
    {
        QByteArray tmp;
        tmp = socket->read(restLen);
        if(tmp.length() > 0){
            restLen -= tmp.length();
            data.append(tmp);
        }
        else if(!socket->waitForReadyRead(1000)){
            break;
        }
    }
    return data;
}


int safeRecvbyTcp(QTcpSocket *socket, char *buf, int len)
{
    if(socket == NULL || !socket->isValid() || buf == NULL)
    {
        return -1;
    }
    int restLen = len;
    while(restLen > 0)
    {
        int rv = socket->read(buf, restLen);
        if(rv < 0){
            return -2;
        }
        restLen -= rv;
    }
    return len - restLen;
}

bool is_valid_ip_address(QString ip)
{
    QStringList sl = ip.split(".");
    if(sl.length() != 4){
        return false;
    }
    bool first = false;
    foreach(QString s, sl){
        bool b = false;
        int a = s.toInt(&b);
        if(!b){
            return false;
        }
        if(!first){
            if(a == 0){
                return false;
            }
            first = true;
        }
        if(a < 0 || a > 255){
            return false;
        }
    }
    return true;
}


QString get_ip_hex(QString ip)
{
    QStringList sl = ip.split('.');
    if(sl.length() < 4){
        return QString();
    }
    QString s1 = sl.value(0);
    QString s2 = sl.value(1);
    QString s3 = sl.value(2);
    QString s4 = sl.value(3);
    return QString("%1%2%3%4").arg(QString("%1").arg(s1.toInt(), 2, 16, QChar('0'))).arg(QString("%1").arg(s2.toInt(), 2, 16, QChar('0'))).arg(QString("%1").arg(s3.toInt(), 2, 16, QChar('0'))).arg(QString("%1").arg(s4.toInt(), 2, 16, QChar('0')));
}



uint32_t get_round_num()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    return (uint32_t)qrand();
}


QString get_uuid_by_uri(QString uri)
{
    if(uri.isEmpty()){
        return QString();
    }
    uri = uri.replace("\\", "/");
    uri = uri.right(uri.length() - uri.indexOf("//") - 2);
    uri = uri.right(uri.length() - uri.indexOf("/") - 1);
    int idx = uri.indexOf("/");
    if(idx > 0){
        uri = uri.left(idx);
    }
    return uri.trimmed();
}

bool getBoolfromJsonString(const QString jsonString, QString name, bool &val)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
    if( jsonDocument.isNull() ){
        return false;
    }
    QJsonObject jsonObject = jsonDocument.object();
    if(!jsonObject.contains(name)){
        return false;
    }
    QJsonValue jsonVal = jsonObject.take(name);
    if(!jsonVal.isBool()){
        return false;
    }
    val = jsonVal.toBool();
    return true;
}

QJsonArray getJsonArrayFromString(const QString jsonString)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
    if( jsonDocument.isNull() ){
        qDebug()<< "===> please check the string "<< jsonString.toUtf8();
    }
    return jsonDocument.array();
}

bool getStringFromJsonObject(const QJsonObject &jsonObject, QString name, QString &val)
{
    QJsonValue jv = jsonObject.value(name);
    if(jv.isNull() || !jv.isString()){
        return false;
    }
    val = jv.toString();
    return true;
}
bool getBoolFromJsonObject(const QJsonObject &jsonObject, QString name, bool &val)
{
    QJsonValue jv = jsonObject.value(name);
    if(jv.isNull() || !jv.isBool()){
        return false;
    }
    val = jv.toBool();
    return true;
}

bool getIntFromJsonObject(const QJsonObject &jsonObject, QString name, int &val)
{
    QJsonValue jv = jsonObject.value(name);
    if(jv.isNull() || !jv.isDouble()){
        return false;
    }
    val = jv.toInt();
    return true;
}

bool getdoubleFromJsonObject(const QJsonObject &jsonObject, QString name, double &val)
{
    QJsonValue jv = jsonObject.value(name);
    if(jv.isNull() || !jv.isDouble()){
        return false;
    }
    val = jv.toDouble();
    return true;
}

bool getJsonfromJsonString(const QString &jsonString, const QString &name, QJsonObject &val)
{
    QJsonObject json = getJsonObjectFromString(jsonString);
    QJsonValue jv = json.value(name);
    if(jv.isNull() || !jv.isObject()){
        return false;
    }
    val = jv.toObject();
    return true;
}

QString getStringFromJsonArray(const QJsonArray &jsonArray)
{
    return QString(QJsonDocument(jsonArray).toJson());
}

uint32_t get_ip_uint32(QString ip)
{
    QStringList sl = ip.split('.');
    if(sl.length() != 4){
        return 0;
    }
    return sl.value(3).toInt() << 24 |
                                  sl.value(2).toInt() << 16 |
                                  sl.value(1).toInt() << 8 |
                                  sl.value(0).toInt();
}


unsigned short irrigation_weather_station_crc16(unsigned char *p, unsigned short data_len)
{
    unsigned char CRC16Lo, CRC16Hi,CL,CH,SaveHi,SaveLo;
    int i,Flag;
    CRC16Lo = 0xFF;
    CRC16Hi = 0xFF;
    CL = 0x01;
    CH = 0xA0;
    for(i=0;i<data_len;i++)
    {
        CRC16Lo ^=*(p+i); //每一个数据与 CRC 寄存器进行异或
        for(Flag=0;Flag<8;Flag++)
        {
            SaveHi = CRC16Hi;
            SaveLo = CRC16Lo;
            CRC16Hi >>= 1 ;
            CRC16Lo >>= 1 ; //高位右移一位，低位右移一位
            if ((SaveHi & 0x01) == 0x01) //如果高位字节最后一位为 1
                CRC16Lo |=0x80 ; //则低位字节右移后前面补 1 否则自动补 0
            if ((SaveLo & 0x01) == 0x01) //如果 LSB 为 1，则与多项式码进行异或
            {
                CRC16Hi ^= CH;
                CRC16Lo ^= CL;
            }
        }
    }

    return (CRC16Hi<<8)|CRC16Lo;
}

unsigned short getUShortfromQByteArray(QByteArray data, int index)
{
    if(index >= data.length()){
        return 0;
    }
    unsigned short tmp = ((int)data.at(index) & 0xff);
    tmp = (tmp << 8) | ((int)data.at(index + 1) & 0xff);
    return tmp;
}

unsigned char getBytefromQByteArray(QByteArray data, int index)
{
    if(index >= data.length()){
        return 0;
    }
    unsigned short tmp = ((int)data.at(index) & 0xff);
    return tmp;
}


unsigned short getNUShortfromQByteArray(QByteArray data, int index)
{
    unsigned short tmp = ((int)data.at(index + 1)) & 0xff;
    tmp = (tmp << 8) | ((int)data.at(index) & 0xff);
    return tmp;
}

void writeNUShortfromQByteArray(QByteArray data, unsigned short val)
{
    data.append((unsigned char)(val & 0xff));
    data.append((unsigned char)((val >> 8) & 0xff));
}

QString get_ip_string(uint32_t ip)
{
    return QString("%1.%2.%3.%4").arg((ip) & 0xff).arg((ip >> 8) & 0xff).arg((ip >> 16) & 0xff).arg((ip >> 24) & 0xff);
}

#if defined(Q_OS_WIN)
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
    TOKEN_PRIVILEGES tp; // 该结构包含一个数组，数据组的每个项指明了权限的类型和要进行的操作
    LUID luid;

    // 查找
    if (!LookupPrivilegeValue(
        NULL,            // 系统的名字,null,在本地系统上查找权限 lookup privilege on local system
        lpszPrivilege,   // 要找的权限名 privilege to lookup
        &luid))        // 通过指针返回权限的LUID receives LUID of privilege
    {
        qDebug() << ("LookupPrivilegeValue error: %u\n") << ":" << GetLastError();
        return FALSE;
    }

    tp.PrivilegeCount = 1;    // 要修改的特权数目
    tp.Privileges[0].Luid = luid;    // 代表不同特权类型
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // 调整访问令牌的权限    Enable the privilege or disable all privileges.
    if (!AdjustTokenPrivileges(
        hToken,// OpenProcessToken第三个指针参数传出的访问令牌的句柄
        FALSE, // 是否禁用所有所有的特权
        &tp,    // 指明要修改的权限
        sizeof(TOKEN_PRIVILEGES),    // PreviousState的长度
        NULL,   // 存放修改前的访问权限的信息，可空
        NULL))    // 实际PreviousState结构返回的大小
    {
        qDebug() << ("AdjustTokenPrivileges error: %u\n") << ":" << GetLastError();
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        qDebug() << ("令牌在NewState参数中没有这个或这些权限") << ":" << ERROR_NOT_ALL_ASSIGNED;
        return FALSE;
    }

    return TRUE;
}

#endif
void set_debug_privilege(){
#if defined(Q_OS_WIN)
    HANDLE hToken;
    BOOL bRet = OpenProcessToken(
        GetCurrentProcess(),    // 进程句柄(当前进程)
        TOKEN_ALL_ACCESS,    // 全权访问令牌
        &hToken    // 返回的参数 进程令牌句柄 （就是AdjustTokenPrivileges的第一个参数）
        ); // 获取进程的令牌句柄
    if (bRet != TRUE){
        qDebug() << "获取令牌句柄失败!";
    }
    BOOL set = SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);
    if (!set || GetLastError() != ERROR_SUCCESS){
        qDebug() << "提升权限失败 error：" << GetLastError();
    }
#endif
}


void yuv420_to_rgb(unsigned char *pRGB, unsigned char **pYUV, int* pitch, int width, int height)
{
    int w, h;
    unsigned char y0, u0, v0, y1;
    int r, g, b;
    unsigned char *pY, *pU, *pV;
    unsigned char *pYStart, *pUStart, *pVStart;
    int ypitch, uvpitch;
    unsigned short *pDest = (unsigned short *)pRGB;

    pYStart = pYUV[0];
    pUStart = pYUV[1];
    pVStart = pYUV[2];

    ypitch  = pitch[0];
    uvpitch = pitch[1];

    for(h=0; h<height; h++)
    {
        pY = pYStart + ypitch  * h;
        pU = pUStart + uvpitch * (h>>1);
        pV = pVStart + uvpitch * (h>>1);

        for(w=0; w<width; w+=2)
        {
            y0 = *pY++;
            y1 = *pY++;
            u0 = *pU++;
            v0 = *pV++;

            r = (298*(y0-16) + 409*(v0 - 128) + 128) >> 8;
            g = (298*(y0-16) - 100*(u0 - 128) - 208*(v0 - 128) + 128) >> 8;
            b = (298*(y0-16) + 516*(u0 - 128) + 128) >> 8;
            if (r>255) r=255; if (r<0) r=0;
            if (g>255) g=255; if (g<0) g=0;
            if (b>255) b=255; if (b<0) b=0;
            *pDest++ = (b >> 3) | ((g & 0xfc) << 3) | ((r & 0xf8) << 8);

            r = (298*(y1-16) + 409*(v0 - 128) + 128) >> 8;
            g = (298*(y1-16) - 100*(u0 - 128) - 208*(v0 - 128) + 128) >> 8;
            b = (298*(y1-16) + 516*(u0 - 128) + 128) >> 8;
            if (r>255) r=255; if (r<0) r=0;
            if (g>255) g=255; if (g<0) g=0;
            if (b>255) b=255; if (b<0) b=0;
            *pDest++ = (b >> 3) | ((g & 0xfc) << 3) | ((r & 0xf8) << 8);
        }
    }
}
