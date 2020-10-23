#ifndef SY_UTILS_H
#define SY_UTILS_H
#include <QJsonObject>
#include <QString>
#include <QDir>
#include <QTcpSocket>
#include <QByteArray>
#include <QList>
#include <QJsonArray>


void wait_by_event_loop(int msecs);

bool getDiskFreeSpace(QString driver, qlonglong &totalsize, qlonglong &freesize, qlonglong &availablesize);


// QString >> QJson
QJsonObject getJsonObjectFromString(const QString jsonString);
QJsonArray getJsonArrayFromString(const QString jsonString);

bool getBoolfromJsonString(const QString jsonString, QString name, bool &val);
bool getIntfromJsonString(const QString jsonString, QString name, int &val);
bool getInt64fromJsonString(const QString jsonString, QString name, qint64 &val);
bool getJsonfromJsonString(const QString &jsonString, const QString &name, QJsonObject &val);

bool getStringfromJsonString(const QString jsonString, QString name, QString &val);

unsigned char getBytefromQByteArray(QByteArray data, int index);
unsigned short getUShortfromQByteArray(QByteArray data, int index);
unsigned short getNUShortfromQByteArray(QByteArray data, int index);
void writeNUShortfromQByteArray(QByteArray data, unsigned short val);

// QJson >> QString
QString getStringFromJsonObject(const QJsonObject& jsonObject);
QString getStringFromJsonArray(const QJsonArray& jsonArray);
bool getQListfromJsonString(const QString jsonString, QString name, QString childName, QList<QString> &val);
bool getDoublefromJsonString(const QString jsonString, QString name, double &val);
bool getStringFromJsonObject(const QJsonObject& jsonObject, QString name, QString &val);
bool getIntFromJsonObject(const QJsonObject& jsonObject, QString name, int &val);
bool getBoolFromJsonObject(const QJsonObject &jsonObject, QString name, bool &val);
bool getdoubleFromJsonObject(const QJsonObject &jsonObject, QString name, double &val);

QDir directoryOf(const QString &subdir);

QString getUuid();

QString trimLeft(const QString &rLine);

bool createFileDir(QString fileName);

QByteArray safeRecvbyTcp(QTcpSocket *socket, int len);
int safeRecvbyTcp(QTcpSocket *socket, char *buf, int len);

bool is_valid_ip_address(QString ip);

QString get_ip_string(uint32_t ip);
QString get_ip_hex(QString ip);
uint32_t get_ip_uint32(QString ip);

uint32_t get_round_num();

QString get_uuid_by_uri(QString uri);
unsigned short irrigation_weather_station_crc16(unsigned char *p, unsigned short data_len);
bool existsDriver(QString filePath);

void set_debug_privilege();
void add_data_to_byte_array(QByteArray &data, char *val, int len);
void yuv420_to_rgb(unsigned char* pRGB, unsigned char** pYUV, int* pitch, int width, int height);

#endif // sy_UTILS_H
