#ifndef CMD_INFO_H
#define CMD_INFO_H

#include <QString>


class cmd_info
{
public:
    cmd_info();

    QString get_cmd();
    void set_cmd(QString val);

    QString get_send();
    void set_send(QString val);

    QString get_suc();
    void set_suc(QString val);

    QString get_err();
    void set_err(QString val);

    QString get_name();
    void set_name(QString val);

private:
    QString m_cmd;
    QString m_send;
    QString m_suc;
    QString m_err;
    QString m_name;
};

#endif // CMD_INFO_H
