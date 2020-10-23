#ifndef VIDEO_ITEM_H
#define VIDEO_ITEM_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QPixmap>
#include <QPainter>
#include <QImage>

#include "data_model.h"

class video_item : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit video_item(QObject *parent = nullptr);
    ~video_item();

    Q_PROPERTY(int signal_source_index READ get_index WRITE set_index NOTIFY index_changed)
    Q_PROPERTY(QString video_id READ get_id WRITE set_id NOTIFY id_changed)
    Q_PROPERTY(data_model *model READ get_model WRITE set_model NOTIFY model_changed)

    Q_INVOKABLE void set_pix_map(QPixmap &map);
    QPixmap get_pix_map();

    int get_index();
    void set_index(int val);

    QString get_id();
    void set_id(QString val);


    Q_INVOKABLE data_model *get_model();
    Q_INVOKABLE void set_model(data_model *val);
    Q_INVOKABLE void set_map(int idx, QPixmap map);
    Q_INVOKABLE void set_image(QString id, QImage img);
    Q_INVOKABLE void clear_image();

    void paint(QPainter *painter);
signals:
    void pix_map_changed();
    void index_changed();
    void current_index_changed();
    void current_map_changed();
    void id_changed();
    void model_changed();

public slots:
    void image_incoming_slot(QString id, QImage img);

private:
    QImage m_image;
    QPixmap m_pix_map;
    int m_index;
    QString m_id;
    data_model *m_data_model_signal_source;
    QSharedPointer<data_info> m_data_info;
    void create_data_info();
};

#endif // VIDEO_ITEM_H
