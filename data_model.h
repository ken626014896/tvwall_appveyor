#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QSharedPointer>

#include "data_info.h"



class data_model : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit data_model(QObject *parent = nullptr);

    enum DataRoles{
        NameRole = Qt::UserRole + 1,
        IcoPath,
        ImageRole,
        ImageListActiveRole,
        ImageListIdleRole,
        ImageCtrlTopRole,
        ImageCtrlHorLeftActiveRole,
        ImageCtrlHorCenterActiveRole,
        ImageCtrlHorRightActiveRole,
        ImageCtrlHorLeftIdleRole,
        ImageCtrlHorCenterIdleRole,
        ImageCtrlHorRightIdleRole,
        DeviceCtrlVirsualSliderRole,
        DeviceCtrlVirsualSwitchRole,
        DeviceStatusRole,
        DeviceTypeRole,
        ImageCtrlDirUpActiveRole,
        ImageCtrlDirUpIdleRole,
        ImageCtrlDirDownActiveRole,
        ImageCtrlDirDownIdleRole,
        ImageCtrlDirRightActiveRole,
        ImageCtrlDirRightIdleRole,
        ImageCtrlDirLeftActiveRole,
        ImageCtrlDirLeftIdleRole,
        ImageCtrlMenuActiveRole,
        ImageCtrlMenuIdleRole,
        ImageCtrlHomeActiveRole,
        ImageCtrlHomeIdleRole,
        ImageCtrlReduceActiveRole,
        ImageCtrlReduceIdleRole,
        ImageCtrlAddActiveRole,
        ImageCtrlAddIdleRole,
        CtrlProgressRole,
        DisplayWidthRole,
        DisplayHeightRole,
        DisplayXRole,
        DisplayYRole,
        IndexRole,
        DeviceTypeStringRole,
        VideoIdRole,
        ImageBottomLeftActiveRole,
        ImageBottomLeftIdleRole,
        ImageBottomRightActiveRole,
        ImageBottomRightIdleRole,
        ImageBottomCenterActiveRole,
        ImageBottomCenterIdleRole,
        VisualCtrlRole,
        VisualCtrlLeftRole,
        VisualCtrlCenterRole,
        VisualCtrlCenterButtonRole,
        VisualCtrlRightRole,
        VisualCtrlBottomRole,
        VisualBottomLeftRole,
        VisualBottomCenterRole,
        VisualBottomRightRole,
        MusicButtonsRole,
        MusicSlidesRole,
        GroupChildsRole,
        GroupImageActiveRole,
        GroupImageIdleRole,
        GroupNameRole,
        AudioVolumeRole,
        WinIdRole
    };

    Q_PROPERTY(int current_index READ get_current_index WRITE set_current_index NOTIFY index_changed)
    Q_PROPERTY(int phy_wall_row READ get_phy_wall_row NOTIFY phy_wall_row_changed)
    Q_PROPERTY(int phy_wall_column READ get_phy_wall_column NOTIFY phy_wall_column_changed)
    Q_PROPERTY(QString current_user_name READ get_current_user_name NOTIFY user_name_changed)
    Q_PROPERTY(QString current_user_password READ get_current_user_password NOTIFY user_password_changed)


    QString get_current_user_name();
    QString get_current_user_password();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void insert(int index,const QSharedPointer<data_info> &data) ;

    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void remove_by_index(int index);
    void append(const QSharedPointer<data_info> &val);
    void append(const QList<QSharedPointer<data_info>> &val);
    void clear();
    Q_INVOKABLE int count() const;
    Q_INVOKABLE int get_current_index();
    Q_INVOKABLE void set_current_index(int val);
    Q_INVOKABLE void set_current_index_sync(int val);
    Q_INVOKABLE void set_current_id(QString val);

    QSharedPointer<data_info> get_data_info(int index);
    QSharedPointer<data_info> current_data_info();
    QSharedPointer<data_info> get_data_info(const QString &id);
    QSharedPointer<data_info> current_data_info_by_id();

    Q_INVOKABLE QVariantList get_music_buttons(int index);
    Q_INVOKABLE int get_phy_wall_row();
    Q_INVOKABLE int get_phy_wall_column();
    Q_INVOKABLE int get_device_type(int index);

    Q_INVOKABLE void set_device_status(int index);
    Q_INVOKABLE void set_device_status(int index, bool checked);
    Q_INVOKABLE void set_device_status(const QString &id, bool checked);
    void data_changed_by_id(const QString &id);

    QList<QSharedPointer<data_info>> get_data_list();

    void append_by_sort(const QSharedPointer<data_info> &val);

    Q_INVOKABLE int get_old_index();
//    Q_INVOKABLE void set_current_index(int val);
protected:
    virtual QHash<int, QByteArray> roleNames() const;

signals:
    void index_changed();
    void index_changed_by_myself();
    void phy_wall_row_changed();
    void phy_wall_column_changed();
    void display_position_changed();
    void logical_wall_row_changed();
    void logical_wall_column_changed();
    void user_name_changed();
    void user_password_changed();
    void id_changed(QString val);

public slots:
    Q_INVOKABLE void data_changed_slot(int index);
    void data_changed_by_index_slot(int index);

private slots:



private:
    QList<QSharedPointer<data_info>> m_list_data;
    int m_current_index;
    QString m_current_id;

    QString get_ico_path(const QString &path) const;

    int m_old_index;
};

#endif // DATA_MODEL_H
