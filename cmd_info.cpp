#include "cmd_info.h"

cmd_info::cmd_info():
    m_cmd(QString()),
    m_send(QString()),
    m_suc(QString()),
    m_err(QString()),
    m_name(QString())
{

}

QString cmd_info::get_cmd()
{
    return m_cmd;
}

void cmd_info::set_cmd(QString val)
{
    m_cmd = val;
}

QString cmd_info::get_send()
{
    return m_send;
}

void cmd_info::set_send(QString val)
{
    m_send = val;
}

QString cmd_info::get_suc()
{
    return m_suc;
}

void cmd_info::set_suc(QString val)
{
    m_suc = val;
}

QString cmd_info::get_err()
{
    return m_err;
}

void cmd_info::set_err(QString val)
{
    m_err = val;
}

QString cmd_info::get_name()
{
   return m_name;
}

void cmd_info::set_name(QString val)
{
    m_name=val;
}
