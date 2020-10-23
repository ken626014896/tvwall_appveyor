#include "data_model.h"
#include "sy_utils.h"

#include <QtDebug>
#include <QCoreApplication>
#include <QDir>
#include <QString>

data_model::data_model(QObject *parent) :
    QAbstractListModel(parent),
    m_current_index(-1),
    m_current_id(QString()),
    m_old_index(-1)
{

}

QString data_model::get_current_user_name()
{
    QSharedPointer<data_info> info = current_data_info();
    if(info.isNull()){
        return QString();
    }
    return info->get_user_name();
}

QString data_model::get_current_user_password()
{
    QSharedPointer<data_info> info = current_data_info();
    if(info.isNull()){
        return QString();
    }
    return info->get_user_pwd();
}

int data_model::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_list_data.count();
}

QVariant data_model::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_list_data.count()) {
//        qDebug()<<1;
        return QVariant();
    }
    QSharedPointer<data_info> data = m_list_data.value(row);
    if(data.isNull()){
//        qDebug()<<2;
        return QVariant();
    }
    QVariant rv = QVariant();
    switch (role) {
    case NameRole:
        rv =data->get_name();
        break;
    case IcoPath:
        rv = data->get_ico_path();
        break;
    case ImageRole:
        rv = data->get_image();
        break;
   case AudioVolumeRole:
        rv = data->get_volume();
        break;
    case ImageCtrlTopRole:
        rv = get_ico_path(QString("images/%1/ctrl-label.png").arg(data->get_device_type()));
        break;
    case ImageCtrlHorLeftActiveRole:
        rv = get_ico_path(QString("images/%1/left-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlHorLeftIdleRole:
        rv = get_ico_path(QString("images/%1/left-idle.png").arg(data->get_device_type()));
        break;
    case ImageCtrlHorRightActiveRole:
        rv = get_ico_path(QString("images/%1/right-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlHorRightIdleRole:
        rv = get_ico_path(QString("images/%1/right-idle.png").arg(data->get_device_type()));
        break;
    case ImageCtrlHorCenterActiveRole:
        rv = get_ico_path(QString("images/%1/center-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlHorCenterIdleRole:
        rv = get_ico_path(QString("images/%1/center-idle.png").arg(data->get_device_type()));
        break;
    case ImageListActiveRole:
//        rv = get_ico_path(QString("images/%1/list-active.png").arg(data->get_device_type()));
        rv = QString("qrc:/images/%1/list-active.png").arg(data->get_device_type());


        break;
    case ImageListIdleRole:{
//        rv = get_ico_path(QString("images/%1/list-idle.png").arg(data->get_device_type()));
        rv = QString("qrc:/images/%1/list-idle.png").arg(data->get_device_type());

        break;}
    case ImageCtrlDirUpActiveRole:
        rv = get_ico_path(QString("images/%1/dir-up-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlDirUpIdleRole:
        rv = get_ico_path(QString("images/%1/dir-up-idle.png").arg(data->get_device_type()));
        break;
    case ImageCtrlDirDownActiveRole:
        rv = get_ico_path(QString("images/%1/dir-down-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlDirDownIdleRole:
        rv = get_ico_path(QString("images/%1/dir-down-idle.png").arg(data->get_device_type()));
        break;
    case ImageCtrlDirRightActiveRole:
        rv = get_ico_path(QString("images/%1/dir-right-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlDirRightIdleRole:
        rv = get_ico_path(QString("images/%1/dir-right-idle.png").arg(data->get_device_type()));
        break;
    case ImageCtrlDirLeftActiveRole:
        rv = get_ico_path(QString("images/%1/dir-left-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlDirLeftIdleRole:
        rv = get_ico_path(QString("images/%1/dir-left-idle.png").arg(data->get_device_type()));
        break;
    case ImageCtrlMenuActiveRole:
        rv = get_ico_path(QString("images/%1/menu-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlMenuIdleRole:
        rv = get_ico_path(QString("images/%1/menu-idle.png").arg(data->get_device_type()));
        break;
    case ImageCtrlHomeActiveRole:
        rv = get_ico_path(QString("images/%1/home-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlHomeIdleRole:
        rv = get_ico_path(QString("images/%1/home-idle.png").arg(data->get_device_type()));
        break;
    case ImageCtrlReduceActiveRole:
        rv = get_ico_path(QString("images/%1/reduce-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlReduceIdleRole:
        rv = get_ico_path(QString("images/%1/reduce-idle.png").arg(data->get_device_type()));
        break;
    case ImageCtrlAddActiveRole:
        rv = get_ico_path(QString("images/%1/add-active.png").arg(data->get_device_type()));
        break;
    case ImageCtrlAddIdleRole:
        rv = get_ico_path(QString("images/%1/add-idle.png").arg(data->get_device_type()));
        break;
    case ImageBottomLeftActiveRole:
        rv = get_ico_path(QString("images/%1/bottom-left-active.png").arg(data->get_device_type()));
        break;
    case ImageBottomLeftIdleRole:
        rv = get_ico_path(QString("images/%1/bottom-left-idle.png").arg(data->get_device_type()));
        break;
    case ImageBottomRightActiveRole:
        rv = get_ico_path(QString("images/%1/bottom-right-active.png").arg(data->get_device_type()));
        break;
    case ImageBottomRightIdleRole:
        rv = get_ico_path(QString("images/%1/bottom-right-idle.png").arg(data->get_device_type()));
        break;
    case ImageBottomCenterActiveRole:
        rv = get_ico_path(QString("images/%1/bottom-center-active.png").arg(data->get_device_type()));
        break;
    case ImageBottomCenterIdleRole:
        rv = get_ico_path(QString("images/%1/bottom-center-idle.png").arg(data->get_device_type()));
        break;
    case CtrlProgressRole:
        rv = data->get_ctrl_progress();
        break;
    case MusicButtonsRole:
        rv = data->get_music_buttons();
        break;
    case MusicSlidesRole:
        rv = data->get_music_sliders();
        break;
    case GroupChildsRole:
        rv = data->get_group_childs();
        break;
    case GroupNameRole:
        rv = data->get_group_name();
        break;
    case GroupImageActiveRole:
        rv = get_ico_path(QString("images/group/group-active.png"));
        break;
    case GroupImageIdleRole:
        rv = get_ico_path(QString("images/group/group-idle.png"));
        break;
    case DeviceCtrlVirsualSliderRole:
        if(data->get_device_type() == 0){
            rv = true;
        }
        else {
            rv =false;
        }
        break;
    case VisualCtrlRole:
        if(data->get_device_type() <= 5){
            rv = true;
        }
        else {
            rv =false;
        }
        break;
    case VisualCtrlLeftRole: //0，1，2，3的左按钮
        if(data->get_device_type() <= 3){
            rv = true;
        }
        else {
            rv =false;
        }
        break;
    case VisualCtrlCenterRole:   //0，1的中间数值  空调和灯光中间变化的值区域
        if(data->get_device_type() <= 1){
            rv = true;
        }
        else {
            rv =false;
        }
        break;
    case VisualCtrlRightRole: //0，1，2，3的右按钮
        if(data->get_device_type() <= 3){
            rv = true;
        }
        else {
            rv =false;
        }
        break;
    case VisualBottomLeftRole: //0的左下按钮，只有空调有
        if(data->get_device_type() == 0){
            rv = true;
        }
        else {
            rv =false;
        }
        break;
    case VisualBottomCenterRole:  //中间下面的按钮
//        if(data->get_device_type() <= 5){
//            rv = true;
//        }
        if(data->get_device_type() ==0||data->get_device_type() == 1){
            rv = true;
        }
        else {
            rv =false;
        }
        break;
    case VisualBottomRightRole:  //0的右下按钮，只有空调有
        if(data->get_device_type() == 0){
            rv = true;
        }
        else {
            rv =false;
        }
        break;

    case VisualCtrlCenterButtonRole: //2，3，4，5的中间按钮
        if(data->get_device_type() >= 2 &&
                data->get_device_type() <= 5){
            rv = true;
        }
        else {
            rv =false;
        }
        break;
    case VisualCtrlBottomRole:  // 0，1的
        if(data->get_device_type() <= 1){
            rv = true;
        }
        else {
            rv =false;
        }
        break;
    case DeviceCtrlVirsualSwitchRole:
        if(data->get_device_type() == 1){
            rv = true;
        }
        else {
            rv =false;
        }
        break;
    case DeviceStatusRole:
        rv = data->get_device_status();
        break;
    case DeviceTypeRole:
        rv = data->get_device_type();
        break;
    case DisplayXRole:
        rv = data->get_display_x();
        break;
    case DisplayYRole:
        rv = data->get_display_y();
        break;
    case DisplayWidthRole:
        rv = data->get_display_width();
        break;
    case DisplayHeightRole:
        rv = data->get_display_height();
        break;
    case IndexRole:
        rv = data->get_index();
        break;
    case DeviceTypeStringRole:
        rv = data->get_device_type_str();
        break;
    case VideoIdRole:
        rv = data->get_id();
        break;
    case WinIdRole:
        rv = data->get_win_id();
        break;
    default:
        rv = QVariant();
        break;
    }

    return rv;
}

void data_model::insert(int index, const QSharedPointer<data_info> &data)
{
    if(index < 0 || index > m_list_data.count()) {
        return;
    }

    emit beginInsertRows(QModelIndex(), index, index);
    m_list_data.insert(index, data);
    emit endInsertRows();
}

void data_model::remove(int index)
{
    if(index < 0 || index >= m_list_data.count()) {
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_list_data.removeAt(index);
    endRemoveRows();
}

void data_model::remove_by_index(int index)
{
    for (int i=0; i < m_list_data.count(); i++) {
        QSharedPointer<data_info> info = m_list_data.at(i);
        if(info.isNull() || info->get_index() != index){
            continue;
        }
        beginRemoveRows(QModelIndex(), i, i);
        m_list_data.removeAt(i);
        endRemoveRows();
        break;
    }
}

void data_model::append(const QSharedPointer<data_info> &val)
{
    if(val.isNull()){
        return;
    }
    insert(m_list_data.count(), val);
}

void data_model::append_by_sort(const QSharedPointer<data_info> &val)
{
    if(val.isNull()){
        return;
    }
    bool s_insert = false;
    for (int j=0;j<m_list_data.count();j++) {
        QSharedPointer<data_info> ii = m_list_data.at(j);
        if(ii.isNull()){
            continue;
        }
        if(val->get_device_type() == ii->get_device_type()){  //去掉相同的设备

            s_insert = true;
            break;
        }
//        if(val->get_sort() < ii->get_sort()){
//            insert(j, val);
//            s_insert = true;
//            break;
//        }
    }
    if(!s_insert){
        insert(m_list_data.count(), val);
    }
}

int data_model::get_old_index()
{
    return m_old_index;
}


void data_model::append(const QList<QSharedPointer<data_info> > &val)
{
    clear();
    for (int i=0; i<val.count(); i++) {
        insert(i, val.at(i));
    }
}

void data_model::clear()
{
    while(m_list_data.count() > 0){
        remove(0);
    }
    m_list_data.clear();
}

int data_model::count() const
{
    return rowCount(QModelIndex());
}

int data_model::get_current_index()
{
    return m_current_index;
}

void data_model::set_current_index(int val)
{
    if(m_current_index == val){
        return;
    }
    m_old_index=m_current_index;
    m_current_index = val;
    emit index_changed();
    emit index_changed_by_myself();
}
void data_model::set_current_index_sync(int val)
{
    if(m_current_index == val){
        return;
    }
    m_current_index = val;
     emit index_changed();  //改变index时该数据结构要求发送的吧 大概

}
void data_model::set_current_id(QString val)
{
    if(m_current_id == val){
        return;
    }
    m_current_id = val;
    emit id_changed(val);
//    QSharedPointer<data_info> data = current_data_info_by_id();
//    if(data.isNull()){
//        return;
//    }
//    set_current_index(data->get_index());
//    data->send_start_image_request_event();
}

QSharedPointer<data_info> data_model::get_data_info(int index)
{
    if(index < 0 || index > m_list_data.count()){
        return QSharedPointer<data_info>();
    }
    return m_list_data.at(index);
}

QSharedPointer<data_info> data_model::current_data_info()
{
    return get_data_info(m_current_index);
}

QSharedPointer<data_info> data_model::get_data_info(const QString &id)
{
    for (int i=0; i < m_list_data.count(); i++) {
        QSharedPointer<data_info> info = m_list_data.at(i);
        if(info.isNull() || info->get_id() != id){
            continue;
        }
        return info;
    }
    return QSharedPointer<data_info>();
}

QSharedPointer<data_info> data_model::current_data_info_by_id()
{
    return get_data_info(m_current_id);
}

QVariantList data_model::get_music_buttons(int index)
{
    QVariantMap map;
    map.insert("dmMusicName", "for teset");
    map.insert("dmMusicIco", "/images/add-active.png");
    map.insert("dmMusicImage", "/images/add-active.png");
    QVariantList list;
    list.append(map);
    return list;
}

int data_model::get_phy_wall_row()
{
    QSharedPointer<data_info> info = current_data_info();
    if(info.isNull()){
        qDebug() << "failed to find room:" << m_current_index;
        return 2;
    }
    return info->get_phy_wall_row();
}

int data_model::get_phy_wall_column()
{
    QSharedPointer<data_info> info = current_data_info();
    if(info.isNull()){
        qDebug() << "failed to find room:" << m_current_index;
        return 2;
    }
    return info->get_phy_wall_column();
}

int data_model::get_device_type(int index)
{
    QSharedPointer<data_info> info = get_data_info(index);
    if(info.isNull()){
        qDebug() << "failed to find room:" << m_current_index;
        return 1;
    }

    return info->get_device_type();
}

void data_model::set_device_status(int index) //修改设备状态
{
    QSharedPointer<data_info> info = get_data_info(index);
    if(info.isNull()){
        qDebug() << "failed to find room:" << m_current_index;
        return;
    }
//    qDebug()<<!info->get_device_status();
    info->set_device_status(!info->get_device_status());
    data_changed_slot(index);
}

void data_model::set_device_status(int index, bool checked)
{
    QSharedPointer<data_info> info = get_data_info(index);
    if(info.isNull()){
        qDebug() << "failed to find room:" << m_current_index;
        return;
    }

    info->set_device_status(checked);
    data_changed_slot(index);
}

void data_model::set_device_status(const QString &id, bool checked) //温度等设备状态发生改变
{
    for (int i=0; i < m_list_data.count(); i++) {
        QSharedPointer<data_info> info = m_list_data.at(i);
        if(info.isNull() || info->get_id() != id){
            continue;
        }
        info->set_device_status(checked);
        data_changed_slot(i);
        break;
    }
}

void data_model::data_changed_by_id(const QString &id)
{
    for (int i=0; i < m_list_data.count(); i++) {
        QSharedPointer<data_info> info = m_list_data.at(i);
        if(info.isNull() || info->get_id() != id){
            continue;
        }
        data_changed_slot(i);
        break;
    }
}

QList<QSharedPointer<data_info> > data_model::get_data_list()
{
    return m_list_data;
}

QString data_model::get_ico_path(const QString &path) const
{
    QDir appdir = directoryOf(qApp->applicationDirPath());
    appdir.cdUp();
    QString icoFile = appdir.absoluteFilePath(QString("%1").arg(path));
//    qDebug() << "get_ico_path:" << icoFile << ":" << path;
    if(QFile::exists(icoFile)){
//        qDebug() << QString("file:/%1").arg(icoFile);
        return QString("file:/%1").arg(icoFile);
    }
    else if(QFile::exists(path.left(1)=="/"?QString(":%1").arg(path):(path.left(1)==":"?path:QString(":/%1").arg(path)))){
//        qDebug() << QString("%1").arg(path);
        return path.left(1)=="/"?QString("%1").arg(path):QString("/%1").arg(path);
    }
//    qDebug() << path;
    return path;
}





QHash<int, QByteArray> data_model::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "dmName";
    roles[IcoPath] = "dmIco";
    roles[ImageRole] = "dmImage";
    roles[AudioVolumeRole] = "dmVolume";
    roles[ImageCtrlTopRole] = "dmImageTop";
    roles[ImageCtrlHorLeftActiveRole] = "dmImageLeftActive";
    roles[ImageCtrlHorCenterActiveRole] = "dmImageCenterActive";
    roles[ImageCtrlHorRightActiveRole] = "dmImageRightActive";
    roles[ImageCtrlHorLeftIdleRole] = "dmImageLeftIdle";
    roles[ImageCtrlHorCenterIdleRole] = "dmImageCenterIdle";
    roles[ImageCtrlHorRightIdleRole] = "dmImageRightIdle";
    roles[DeviceCtrlVirsualSliderRole] = "dmDeviceCtrlSliderVirsual";
    roles[DeviceCtrlVirsualSwitchRole] = "dmDeviceCtrlSwitchVirsual";
    roles[DeviceStatusRole] = "dmDeviceStatus";
    roles[DeviceTypeRole] = "dmDeviceType";
    roles[ImageListActiveRole] = "dmListActive";
    roles[ImageListIdleRole] = "dmListIdle";


    roles[ImageCtrlDirUpActiveRole] = "dmImageCtrlDirUpActive";
    roles[ImageCtrlDirUpIdleRole] = "dmImageCtrlDirUpIdle";
    roles[ImageCtrlDirDownActiveRole] = "dmImageCtrlDirDownActive";
    roles[ImageCtrlDirDownIdleRole] = "dmImageCtrlDirDownIdle";
    roles[ImageCtrlDirRightActiveRole] = "dmImageCtrlDirRightActive";
    roles[ImageCtrlDirRightIdleRole] = "dmImageCtrlDirRightIdle";
    roles[ImageCtrlDirLeftActiveRole] = "dmImageCtrlDirLeftActive";
    roles[ImageCtrlDirLeftIdleRole] = "dmImageCtrlDirLeftIdle";
    roles[ImageCtrlMenuActiveRole] = "dmImageCtrlMenuActive";
    roles[ImageCtrlMenuIdleRole] = "dmImageCtrlMenuIdle";
    roles[ImageCtrlHomeActiveRole] = "dmImageCtrlHomeActive";
    roles[ImageCtrlHomeIdleRole] = "dmImageCtrlHomeIdle";
    roles[ImageCtrlReduceActiveRole] = "dmImageCtrlReduceActive";
    roles[ImageCtrlReduceIdleRole] = "dmImageCtrlReduceIdle";
    roles[ImageCtrlAddActiveRole] = "dmImageCtrlAddActive";
    roles[ImageCtrlAddIdleRole] = "dmImageCtrlAddIdle";
    roles[CtrlProgressRole] = "dmCtrlProgress";

    roles[DisplayWidthRole] = "dmDisplayWidth";
    roles[DisplayHeightRole] = "dmDisplayHeight";
    roles[DisplayXRole] = "dmDisplayX";
    roles[DisplayYRole] = "dmDisplayY";
    roles[VideoIdRole] = "dmVideoId";
    roles[ImageBottomLeftActiveRole] = "dmImageBottomLeftActive";
    roles[ImageBottomLeftIdleRole] = "dmImageBottomLeftIdle";
    roles[ImageBottomRightActiveRole] = "dmImageBottomRightActive";
    roles[ImageBottomRightIdleRole] = "dmImageBottomRightIdle";
    roles[ImageBottomCenterActiveRole] = "dmImageBottomCenterActive";
    roles[ImageBottomCenterIdleRole] = "dmImageBottomCenterIdle";

    roles[VisualCtrlRole] = "dmVisualCtrl";
    roles[VisualCtrlLeftRole] = "dmVisualCtrlLeft";
    roles[VisualCtrlCenterRole] = "dmVisualCtrlCenter";
    roles[VisualCtrlRightRole] = "dmVisualCtrlRight";
    roles[VisualBottomLeftRole] = "dmVisualBottomLeft";
    roles[VisualBottomCenterRole] = "dmVisualBottomCenter";
    roles[VisualBottomRightRole] = "dmVisualBottomRight";

    roles[VisualBottomCenterRole] = "dmVisualBottomCenter";
    roles[VisualBottomRightRole] = "dmVisualBottomRight";

    roles[VisualCtrlCenterButtonRole] = "dmVisualCtrlCenterButton";
    roles[VisualCtrlBottomRole] = "dmVisualCtrlBottom";
    roles[MusicButtonsRole] = "dmMusicButtons";
    roles[MusicSlidesRole] = "dmMusicSlides";

    roles[IndexRole] = "dmIndex";
    roles[DeviceTypeStringRole] = "dmDeviceTypeString";

    roles[GroupChildsRole] = "dmGroupChilds";
    roles[GroupImageActiveRole] = "dmGroupImageActive";
    roles[GroupImageIdleRole] = "dmGroupImageIdle";

    roles[GroupNameRole] = "dmGroupName";

    roles[WinIdRole]="dmWinId";
    return roles;
}

void data_model::data_changed_slot(int index)
{
    if(index < 0 || index >= m_list_data.count()){

        return;
    }
//    qDebug() << "data_changed_slot:" << index;
    emit dataChanged(this->index(index), this->index(index));  //内置信号  如果改变的是roleNames其中的值时一定要发出这个信号   为什么？？？
//    emit display_position_changed();
}

void data_model::data_changed_by_index_slot(int index)
{
    for (int i=0; i < m_list_data.count(); i++) {
        QSharedPointer<data_info> info = m_list_data.at(i);
        if(info.isNull() || info->get_index() != index){
            continue;
        }
        emit dataChanged(this->index(i), this->index(i));
        break;
    }
}

