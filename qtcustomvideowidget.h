#ifndef QTCUSTOMVIDEOWIDGET_H
#define QTCUSTOMVIDEOWIDGET_H

#include <QVideoWidget>

class QtCustomVideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    explicit QtCustomVideoWidget(QWidget *parent = 0);

signals:

public slots:
    void paintEvent(QPaintEvent *e);

};

#endif // QTCUSTOMVIDEOWIDGET_H
