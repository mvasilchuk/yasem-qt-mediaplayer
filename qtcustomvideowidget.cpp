#include "qtcustomvideowidget.h"
#include "macros.h"

#include <QPainter>
#include <QBrush>
#include <QPaintEvent>
#include <QGraphicsEffect>

using namespace yasem;

QtCustomVideoWidget::QtCustomVideoWidget(QWidget *parent) :
    QVideoWidget(parent)
{

}

void QtCustomVideoWidget::paintEvent(QPaintEvent *e)
{
    QVideoWidget::paintEvent(e);


}
