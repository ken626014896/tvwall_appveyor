#include "room_signal_sync_worker.h"
#include "sy_utils.h"
#include "sy_logger.h"
#include <QHostAddress>
#include <QtDebug>
#include <QTimer>
#include "signal_resource_info.h"
#ifdef WIN32
    #include <QTimer>
    #include <qt_windows.h>
    #include "room_signal_sync_thread.h"
    #include "signal_resource_info.h"
#endif // ifdef WIN32


#define RES_UNKNOWN             -1
#define RES_NULL                        0
#define RES_WALL                        1
#define RES_SIGNAL                      2
#define RES_SCENE                       3
#define RES_SEQUENCE            4
#define RES_MATRIX              5
#define RES_USER                        6
#define RES_GROUP                       7
#define RES_OPERATION           8
#define RES_TASK                        9
#define RES_FTEXT                       10

#define MAX_CMD_LEN 256

#pragma pack(push, 1) // 字节对齐，不然会出错
typedef struct {
    int len;
    int id;
    int cmd;
} CMD_HEADER, *PCMD_HEADER;

typedef struct {
    int id;
    int status;
    int connect_id;
} LOGIN_RESPONSE, *PLOGIN_RESPONSE;

typedef struct {
    int id;
    int status;
} SYNC_RESPONSE, *PSYNC_RESPONSE;

typedef struct { // 窗口打开
    CMD_HEADER head;
    int        wall_id;
    char       id[16];
    int        x;
    int        y;
    int        width;
    int        height;
    int        win_uk; // 暂时为0
    unsigned int        win_id;
} WIN_OPEN, *PWIN_OPEN;

typedef struct { // 窗口关闭
    CMD_HEADER head;
    unsigned int        win_id;
} WIN_CLOSE, *PWIN_CLOSE;

typedef struct { // 窗口全部关闭
    CMD_HEADER head;
    int        wall_id;
} WIN_CLOSE_ALL, *PWIN_CLOSE_ALL;

typedef struct { // 窗口更新
    CMD_HEADER head;
    unsigned int        win_id;
    int        x;
    int        y;
    int        width;
    int        height;
} WIN_UPDATE, *PWIN_UPDATE;

typedef struct { // 窗口顶置
    CMD_HEADER head;
    unsigned int        win_id;
    int        level;
} WIN_LEVEL_CHANGED, *PWIN_LEVEL_CHANGED;

typedef struct { // 信号源改变
    CMD_HEADER head;
    unsigned int        win_id;
    char       id[16];
} SIG_CHANGED, *PSIG_CHANGED;

typedef struct {
    CMD_HEADER head;

    char id[16];
} MODE_CHANGED, *PMODE_CHANGED;
typedef struct {
    CMD_HEADER head;
    int        type;
    char       groud_id[16];
    char       res_id[16];
} MODE_REMOVE, *PMODE_REMOVE;


typedef struct { // 增加和修改公用
    CMD_HEADER head;
    int        type;
    char       groud_id[16];
    int        cbSize;
    char       id[16]; // guid
    int        len;    // 单个场景名称的长度
} MODE_ADD_EDIT, *PMODE_ADD_EDIT;

typedef struct {
    int num;
} NUM, *PNUM;

typedef struct tagWIN {
    char signal_id[16];

    // WINPOSITION
    int          left;
    int          top;
    unsigned int width;
    unsigned int height;
    int          style;
    unsigned int win_id;
    int          signal_type;
} WIN, *PWIN;

typedef struct tagAUDIO {
    char         signal_id[16];
    int          channel;
    int          opt; // volume;
    unsigned int play_id;
} AUDIO, *PAUDIO;
typedef struct {      // 音频
    CMD_HEADER head;
    char       signal_id[16];
    int        channel;
    char       vol;
    char       temp1;
    char       temp2;
    char       temp3;
    int        audio_id;
} AUDIO_OPEN, *PAUDIO_OPEN;

typedef struct {
    CMD_HEADER head;
    int        audio_id;
} AUDIO_CLOSE, *PAUDIO_CLOSE;

typedef struct {
    CMD_HEADER head;
    int        audio_id;
    char       vol;
    char       temp1;
    char       temp2;
    char       temp3;
} AUDIO_SET, *PAUDIO_SET;

// 音频结构


typedef struct {
    char         id[16];
    int          x;
    int          y;
    int          width;
    int          height;
    int          win_unknow;
    unsigned int win_id;
    int          signal_type;
} WALL_WIN_INFO, *PWALL_WIN_INFO;

typedef struct {
    int id;
    int status;
    int count;
} SIGNAL_HEADER, *PSIGNAL_HEADER;
#pragma pack(pop)

QList<int> cmd_exp = { 0x12, 0x13,  0x15,
                       0x21,  0x22,  0x23, 0x24, 0x25, 0x26,
                       0x2d,  0x2e,  0x2f, 0x30
                     };
room_signal_sync_worker::room_signal_sync_worker(data_info *data,
        QObject   *parent) : QObject(parent),
    m_tcp_socket(nullptr),
    m_packet_id(1),
    m_data_info(data),
    m_group_id("02000000000000000000000000000000"),
    m_timer(nullptr),
    m_signal_source_index(0),
    m_accept_msg_hint(0),
    m_group_id_mode("03000000000000000000000000000000"),
    isFirstConnect(true)
{
}

room_signal_sync_worker::~room_signal_sync_worker()
{
    stop();
}

void room_signal_sync_worker::start()
{
    if (m_tcp_socket != nullptr) {
        return;
    }
    qDebugxxxx(qtr("连接场景"),
               m_data_info->get_name(),
               m_data_info->get_ip_address(),
               m_data_info->get_port());
    m_tcp_socket = new QTcpSocket();
    connect(m_tcp_socket,
            &QTcpSocket::readyRead,
            this,
            &room_signal_sync_worker::recv_data);
    connect(m_tcp_socket,
            &QTcpSocket::connected,
            this,
            &room_signal_sync_worker::connected);
    connect(m_tcp_socket,
            &QTcpSocket::disconnected,
            this,
            &room_signal_sync_worker::disconnected_status);
//    connect(m_tcp_socket,&QTcpSocket::stateChanged,this,&room_signal_sync_worker::onSocketStateChange);
    m_tcp_socket->connectToHost(QHostAddress(m_data_info->get_ip_address()),
                                m_data_info->get_port());
    if (m_timer == nullptr) {
        m_timer = new QTimer();
        connect(m_timer,
                &QTimer::timeout,
                this,
                &room_signal_sync_worker::time_reconnect);
    } else {
        m_timer->stop();
    }
    if(isFirstConnect) {
        m_packet_id = 1;
    }
}

void room_signal_sync_worker::stop()
{
    if (m_tcp_socket != nullptr) {
        m_tcp_socket->close();
        delete m_tcp_socket;
        m_tcp_socket = nullptr;
    }
    if (m_timer != nullptr) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    qDebugxxxx(qtr("停止信号同步"),
               m_data_info->get_name(),
               m_data_info->get_ip_address(),
               m_data_info->get_port());
}

void room_signal_sync_worker::connected()
{
    //    m_accept_msg_hint=1;
    qDebugxx(qtr("信号同步线程tcp连接成功"), m_data_info->get_name());
    login();
    QByteArray res = get_response();
    if (res.count() <= 0) {
        qWarnx(qtr("登录失败，重新登录"));
        //            reconnected();
        return;
    }
    PLOGIN_RESPONSE rv = (PLOGIN_RESPONSE)res.constData();
    if (rv->status != 0) {
        qWarnxx(qtr("同步登录失败"), rv->status);
        return;
    } else {
        qWarnxx(qtr("同步登录成功"), rv->status);
    }
    // 开启信号同步
    sync();
    res = get_response();
    PSYNC_RESPONSE syncrs = (PSYNC_RESPONSE)res.constData();
    if (syncrs->status != 0) {
        qWarnxx(qtr("同步失败"), syncrs->status);
        return;
    } else {
        qWarnxx(qtr("同步成功"), syncrs->status);
        m_accept_msg_hint = 1;
    }
    if(!isFirstConnect) {
        emit reconnect_signal();
    }
    m_tcp_socket->readAll();
}

void room_signal_sync_worker::recv_data()
{
    if (m_tcp_socket == nullptr) {
        return;
    }
//    QByteArray res = m_tcp_socket->readAll();
//    qDebug()<<"sync cmd2"<<res.toHex();
    if (m_accept_msg_hint == 1) {
        if (!m_data_info->get_isMe()) {
            QByteArray res = m_tcp_socket->readAll();
//        QByteArray res=m_tcp_socket->readLine();
//            qDebug()<<"sync cmd2"<<res.toHex();
            emit msgCome(res);
        } else {
            QByteArray res = m_tcp_socket->readAll();
        }
    }
    m_data_info->set_isMe(false);
}

void room_signal_sync_worker::time_reconnect()
{
    start(); //ios锁屏后，tcp莫名断开了，要重新连接
}

void room_signal_sync_worker::reconnected()
{
    m_accept_msg_hint = 0;
    isFirstConnect = false;
    m_tcp_socket->blockSignals(true);
    if(m_tcp_socket != nullptr) {
        m_tcp_socket->close();
        m_tcp_socket->deleteLater();
        m_tcp_socket = nullptr;
    }
    if(m_timer != nullptr) {
        emit disconnect_signal();
        m_timer->start(3000);
    }
}

void room_signal_sync_worker::disconnected_status()
{
    qDebug() << "discenncted";
    reconnected();
}

void room_signal_sync_worker::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
    {
        qDebug()<<(QStringLiteral("scoket状态：UnconnectedState"));
//           emit login_status(false,IP, PORT, usr, pwd);
//           stop();
    }
    break;
    case QAbstractSocket::HostLookupState:
        qDebug()<<(QStringLiteral("scoket状态：HostLookupState"));
        break;
    case QAbstractSocket::ConnectingState:
        qDebug()<<(QStringLiteral("scoket状态：ConnectingState"));
        break;

    case QAbstractSocket::ConnectedState:
        qDebug()<<(QStringLiteral("scoket状态：ConnectedState"));
        break;

    case QAbstractSocket::BoundState:
        qDebug()<<(QStringLiteral("scoket状态：BoundState"));
        break;

    case QAbstractSocket::ClosingState:
        qDebug()<<(QStringLiteral("scoket状态：ClosingState"));
        break;

    case QAbstractSocket::ListeningState:
        qDebug()<<(QStringLiteral("scoket状态：ListeningState"));
    }
}

void room_signal_sync_worker::canSend_slot(QByteArray res)
{
    qDebug() << "sync msg" << res.toHex();
    PCMD_HEADER rv = (PCMD_HEADER)res.constData();
    if (cmd_exp.indexOf(rv->cmd) >= 0) { // 区别返回信息
        switch (rv->cmd) {
            case 0x21:
                win_open(res);
                break;
            case 0x22:
                win_close(res);
                break;
            case 0x23:
                win_close_all(res);
                break;
            case 0x24:
                win_move(res);
                break;
            case 0x25:
                win_change(res);
                break;
            case 0x26:
                win_level(res);
                break;
            case 0x30:
                mode_change(res);
                break;
            case 0x12:
                mode_add(res);
                break;
            case 0x13:
                mode_remove(res);
                break;
            case 0x15:
                mode_edit(res);
                break;
            default:
                qDebug() << "nor";
                break;
        }
    }
}

void room_signal_sync_worker::get_window_info()
{
    if (m_tcp_socket == nullptr) {
        return;
    }
    //    QByteArray data;
    //    add_cmd_header(data, 0x20, 4 + 4 + 4);
    //    int id = 0;
    //    add_data_to_byte_array(data, (char *)&id, sizeof(int));
    //    m_tcp_socket->write(data);
}

void room_signal_sync_worker::add_cmd_header(QByteArray& data, int cmd, int len)
{
    CMD_HEADER cmd_header;
    cmd_header.len = len;
    cmd_header.id = m_packet_id++;
    cmd_header.cmd = cmd;
    add_data_to_byte_array(data, (char *)&cmd_header, sizeof(CMD_HEADER));
}

QByteArray room_signal_sync_worker::get_response()
{
    if (!m_tcp_socket->waitForReadyRead()) {
        qWarnx(qtr("未接收到响应"));
        return QByteArray();
    }
    int len = 0;
    if (m_tcp_socket->read((char *)&len, sizeof(int)) != sizeof(int)) {
        qWarnx(qtr("未能收取有效的响应长度"));
        m_tcp_socket->readAll();
        return QByteArray();
    }
    qDebugxx(qtr("接收响应数据的长度"), len);
    if (len <= 0) {
        return QByteArray();
    }
    if (len > 2 * 1024 * 1024) {
        qWarnx(qtr("收到的数据长度过长"));
        m_tcp_socket->readAll();
        return QByteArray();
    }
    return safeRecvbyTcp(m_tcp_socket, len);
}

void room_signal_sync_worker::login()
{
    if (m_tcp_socket == nullptr) {
        return;
    }
    QByteArray data;
    add_cmd_header(data,
                   0x00,
                   4 + 4 + 4 + m_data_info->get_user_name().length() + 4 +
                   m_data_info->get_user_pwd().length());
    int len = m_data_info->get_user_name().length();
    add_data_to_byte_array(data, (char *)&len, sizeof(int));
    data.append(m_data_info->get_user_name().toLocal8Bit().constData());
    len = m_data_info->get_user_pwd().length();
    add_data_to_byte_array(data, (char *)&len, sizeof(int));
    data.append(m_data_info->get_user_pwd().toLocal8Bit().constData());
    qDebugxx(qtr("登录指令"), QString(data.toHex()));
    m_tcp_socket->write(data);
}

void room_signal_sync_worker::sync()
{
    if (m_tcp_socket == nullptr) {
        return;
    }
    QByteArray data;
    add_cmd_header(data, 0x03, 4 + 4 + 4);
    int cmd = 1;
    add_data_to_byte_array(data, (char *)&cmd, sizeof(int));
    //    qDebug()<<"heollo";
    //    qDebug()<<data.toHex();
    qDebugxx(qtr("开启同步指令"), QString(data.toHex()));
    m_tcp_socket->write(data);
}

QSharedPointer<data_info>room_signal_sync_worker::get_signal_source(
    const QString& id)
{
    if (m_data_info == nullptr) {
        qWarnx(qtr("参数错误"));
        return QSharedPointer<data_info>();
    }
    QSharedPointer<data_info> info = m_data_info->get_signal_source_by_id(id);
    if (!info.isNull()) {
        return info;
    }
    return QSharedPointer<data_info>();
}

// 处理打开窗口
void room_signal_sync_worker::win_open(QByteArray res)
{
    PWIN_OPEN win = (PWIN_OPEN)res.constData();
    QString   id = QString(QByteArray(win->id, sizeof(win->id)).toHex());
    QSharedPointer<data_info> old = get_signal_source(id);
    if (old.isNull()) {
        qWarnxx(qtr("未正确创建对象"),
                QString(QByteArray(win->id, sizeof(win->id)).toHex()));
    }
    QSharedPointer<data_info> info = QSharedPointer<data_info>(
                                         new signal_resource_info);
    info->set_old_data_info(old);
    info->clone(old);
    //        info->set_display_x_old(win->x);  //old代表监控屏的属性  ,这里要替换成软件中的大小，不然出错
    //        info->set_display_y_old(win->y);
    //        info->set_display_width_old(win->width);
    //        info->set_display_height_old(win->height);
    info->set_display_x_old(win->x * 1.0 / m_data_info->get_wall_width()); // old代表监控屏的属性
    //  ,这里要替换成软件中的大小，不然出错
    info->set_display_y_old(win->y * 1.0 / m_data_info->get_wall_height());
    info->set_display_width_old(win->width * 1.0 / m_data_info->get_wall_width());
    info->set_display_height_old(
        win->height * 1.0 / m_data_info->get_wall_height());
    if ((win->x < 0) || (win->x > m_data_info->get_wall_width())) {
        win->x = 0;
    }
    if ((win->y < 0) || (win->y > m_data_info->get_wall_height())) {
        win->y = 0;
    }
    info->set_display_x(win->x * 1.0 / m_data_info->get_wall_width());
    info->set_display_y(win->y * 1.0 / m_data_info->get_wall_height());
    if (win->width == 0) {
        info->set_display_width(0.5);
    } else {
        info->set_display_width(win->width * 1.0 / m_data_info->get_wall_width());
    }
    if (win->height == 0) {
        info->set_display_height(0.5);
    } else {
        info->set_display_height(
            win->height * 1.0 / m_data_info->get_wall_height());
    }
    info->set_win_id(win->win_id);
    qDebugxxxxxxxx(qtr("获取到窗口信号源"),
                   win->win_id,
                   QString("%1:%2:%3:%4:%5").arg(id).arg(
                       info->get_display_x_old()).arg(info->
                               get_display_y_old()).arg(
                       info->get_display_width_old()).arg(info->
                               get_display_height_old()),
                   info->get_win_id(),
                   info->get_display_x(),
                   info->get_display_y(),
                   info->get_display_width(),
                   info->get_display_height());
    //    qDebug()<<"open";
    m_data_info->add_signal_source_in_tv_wall(info);
    emit sync_win_open_signal(info);
}

void room_signal_sync_worker::win_close(QByteArray res)
{
    PWIN_CLOSE win = (PWIN_CLOSE)res.constData();
    emit sync_win_close_signal(win->win_id);
}

void room_signal_sync_worker::win_close_all(QByteArray res)
{
    emit sync_win_close_all_signal();
}

void room_signal_sync_worker::win_move(QByteArray res)
{
    //    qDebug()<<"move";
    PWIN_UPDATE win = (PWIN_UPDATE)res.constData();
    QSharedPointer<data_info> info = QSharedPointer<data_info>(new data_info); //
    // 信号
    // 设置的都是百分比
    //    qDebug()<<(win->width * 1.0 / m_data_info->get_wall_width());
    info->set_display_x(win->x * 1.0 / m_data_info->get_wall_width());
    info->set_display_y(win->y * 1.0 / m_data_info->get_wall_height());
    if (win->width == 0) {
        info->set_display_width(0.5);
    } else {
        info->set_display_width(win->width * 1.0 / m_data_info->get_wall_width());
    }
    if (win->height == 0) {
        info->set_display_height(0.5);
    } else {
        info->set_display_height(
            win->height * 1.0 / m_data_info->get_wall_height());
    }
    info->set_win_id(win->win_id);
    emit sync_win_move_signal(info);
}

void room_signal_sync_worker::win_level(QByteArray res)
{
    PWIN_LEVEL_CHANGED win = (PWIN_LEVEL_CHANGED)res.constData();
    emit sync_win_level_signal(win->win_id, 1);
}

void room_signal_sync_worker::win_change(QByteArray res)
{
    PSIG_CHANGED win = (PSIG_CHANGED)res.constData();
    QSharedPointer<data_info> info = QSharedPointer<data_info>(new data_info); //
    //
    //
    // 信号
    QString id = QString(QByteArray(win->id, sizeof(win->id)).toHex());
    info->set_id(id);
    info->set_win_id(win->win_id);
    emit sync_win_change_signal(info);
}

void room_signal_sync_worker::mode_change(QByteArray res)
{
    PMODE_CHANGED mode = (PMODE_CHANGED)res.constData();
    // 信号 通过枚举信号墙实现 信号模式同步
    QString id = QString(QByteArray(mode->id, sizeof(mode->id)).toHex());
    emit    sync_mode_change_signal(id);
}

void room_signal_sync_worker::mode_add(QByteArray res)
{
    qDebug() << "add mode";
    QSharedPointer<data_info> info(new data_info());
    PMODE_ADD_EDIT mode = (PMODE_ADD_EDIT)res.constData();
    if(mode->type==2){ //证明添加的是信号源不是场景

        return;
    }
    // 信号
    QString id = QString(QByteArray(mode->id, sizeof(mode->id)).toHex());
    QString mode_name = QString(res.mid(sizeof(MODE_ADD_EDIT), mode->len));
    PNUM    num_st2 = (PNUM)(res.constData() + sizeof(MODE_ADD_EDIT) + mode->len);
    int     win_num = num_st2->num;
    int     pos = sizeof(MODE_ADD_EDIT) + mode->len + 4;
    info->set_id(id); // 信号模式id
    info->set_name(mode_name);
    info->set_logical_wall_row(m_data_info->get_logical_wall_row());
    info->set_logical_wall_column(m_data_info->get_logical_wall_column());
    for (int i = 0; i < win_num; i++) {
        PWIN win_st = (PWIN)(res.constData() + pos);
        QSharedPointer<data_info> sig(new signal_resource_info());
        QString id =
            QString(QByteArray(win_st->signal_id,
                               sizeof(win_st->signal_id)).toHex());
        sig->set_display_x(win_st->left * 1.0 / m_data_info->get_wall_width());
        sig->set_display_y(win_st->top * 1.0 / m_data_info->get_wall_height());
        sig->set_display_width(win_st->width * 1.0 /
                               m_data_info->get_wall_width());
        sig->set_display_height(
            win_st->height * 1.0 / m_data_info->get_wall_height());
        //        qDebug()<<win_st->width;
        sig->set_id(id);
        sig->set_win_id(win_st->win_id);
        info->add_signal_source_in_tv_wall(sig);
        pos = pos + sizeof(WIN);
    }
    emit sync_mode_add_signal(info);
}

void room_signal_sync_worker::mode_edit(QByteArray res)
{
    QSharedPointer<data_info> info(new data_info());
    PMODE_ADD_EDIT mode = (PMODE_ADD_EDIT)res.constData();
    // 信号
    QString id = QString(QByteArray(mode->id, sizeof(mode->id)).toHex());
    QString mode_name = QString(res.mid(sizeof(MODE_ADD_EDIT), mode->len));
    PNUM    num_st2 = (PNUM)(res.constData() + sizeof(MODE_ADD_EDIT) + mode->len);
    int     win_num = num_st2->num;
    int     pos = sizeof(MODE_ADD_EDIT) + mode->len + 4;
    info->set_id(id); // 信号模式id
    info->set_name(mode_name);
    info->set_logical_wall_row(m_data_info->get_logical_wall_row());
    info->set_logical_wall_column(m_data_info->get_logical_wall_column());
    for (int i = 0; i < win_num; i++) {
        PWIN win_st = (PWIN)(res.constData() + pos);
        QSharedPointer<data_info> sig(new signal_resource_info());
        QString id =
            QString(QByteArray(win_st->signal_id,
                               sizeof(win_st->signal_id)).toHex());
        sig->set_display_x(win_st->left * 1.0 / m_data_info->get_wall_width());
        sig->set_display_y(win_st->top * 1.0 / m_data_info->get_wall_height());
        sig->set_display_width(win_st->width * 1.0 /
                               m_data_info->get_wall_width());
        sig->set_display_height(
            win_st->height * 1.0 / m_data_info->get_wall_height());
        sig->set_id(id);
        //
        //          qDebug()<<win_st->width*1.0<<m_data_info->get_wall_width();
        sig->set_win_id(win_st->win_id);
        info->add_signal_source_in_tv_wall(sig);
        pos = pos + sizeof(WIN);
    }
    emit sync_mode_edit_signal(info);
}

void room_signal_sync_worker::mode_remove(QByteArray res)
{
    PMODE_REMOVE mode = (PMODE_REMOVE)res.constData();
    // 信号
    QString res_id = QString(QByteArray(mode->res_id,
                                        sizeof(mode->res_id)).toHex());
    emit sync_mode_remove_signal(res_id);
}

void room_signal_sync_worker::audio_open(QByteArray res)
{
    PAUDIO_OPEN audio = (PAUDIO_OPEN)res.constData();
    QString     id = QString(QByteArray(audio->signal_id,
                                        sizeof(audio->signal_id)).toHex());
    emit sync_audio_open_signal(id,
                                audio->channel,
                                (int)audio->vol,
                                audio->audio_id);
}

void room_signal_sync_worker::audio_close(QByteArray res)
{
    PAUDIO_CLOSE audio = (PAUDIO_CLOSE)res.constData();
    emit sync_audio_close_signal(audio->audio_id);
}

void room_signal_sync_worker::audio_set(QByteArray res)
{
    PAUDIO_SET audio = (PAUDIO_SET)res.constData();
    emit sync_audio_set_signal(audio->audio_id, (int)audio->vol);
}
