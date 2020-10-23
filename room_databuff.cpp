#include "room_databuff.h"
#include <QDebug>
#pragma pack(push, 1)
typedef struct {
    int len;
    int id;
    int cmd;
} NUM, *PNUM;
#pragma pack(pop)
room_databuff::room_databuff(QObject *parent) : QObject(parent), buff(nullptr)
{
}

void room_databuff::spliceData(QByteArray res)  //防止字节流断流发来
{
//   emit canSend(res);
    buff.append(res);
    if(buff.length() >= sizeof (NUM)) {
        PNUM mode = (PNUM)buff.constData();
        if(buff.length() == (mode->len + 4)) { //刚好满足一个
            emit canSend(buff.mid(0, mode->len + 4));
            buff.clear();
        } else if(buff.length() > (mode->len + 4)) { //满足几个
            qDebug() << "Contains multiple byte";
            int len = mode->len + 4; //一条同步信号字节段的长度
            int all_len = buff.length(); //buff中所有字节的长度
            QByteArray send;
            while(all_len >= len) {
                send = buff.mid(0, len);
                emit canSend(send);
                buff = buff.mid(len, all_len - len);
                if(buff.length() < sizeof (NUM)) {
                    break;
                }
                PNUM mode = (PNUM)buff.constData();
                len = mode->len + 4;
                all_len = buff.length();
            }
        }
    }
}
