#ifndef ROOM_COM_H
#define ROOM_COM_H

#include <QObject>
#include <QSerialPort>
class room_com : public QObject
{
    Q_OBJECT
public:
    explicit room_com(QObject *parent = nullptr);

public slots:
    void  operation_direction(int o,int a);
    void  operation_zoom(int o,int num);
    void  closeSerialPort();
signals:
    void canSend(QByteArray);

private:
    QSerialPort *serial;
};

#endif // ROOM_COM_H
