#ifndef ROOM_DATABUFF_H
#define ROOM_DATABUFF_H

#include <QObject>

class room_databuff : public QObject
{
    Q_OBJECT
public:
    explicit room_databuff(QObject *parent = nullptr);


private:
    QByteArray buff;

public slots:
    void  spliceData(QByteArray);
signals:
    void canSend(QByteArray);
};

#endif // ROOM_DATABUFF_H
