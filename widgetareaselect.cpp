#include "widgetareaselect.h"

#include <QApplication>
#include <QResizeEvent>
#include <QPainter>
#include <QDebug>

enum DefaultSizes{
    defaultControlPointHalfEdge = 5,
    defaultControlPointEdge = defaultControlPointHalfEdge * 2
};

WidgetAreaSelect::WidgetAreaSelect(QWidget *parent)
    : QWidget(parent)
{
    init();
}

WidgetAreaSelect::~WidgetAreaSelect()
{

}

void WidgetAreaSelect::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mousemove";
    QWidget::mouseMoveEvent(event);

    const QPoint &pos = event->pos();

    if(_resizeMode == NoResize){
        setCursor(QCursor());
        for(int itemIndex = 0; itemIndex < controlsCount; ++itemIndex){
            Controls item = static_cast<Controls>(itemIndex);
            if(rectOf(item).contains(pos)){
                setCursor(QCursor(shapeOf(item)));
                break;
            }
        }
    }
    else{

    }

}

void WidgetAreaSelect::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button()!=Qt::LeftButton)
        return;

    _resizeMode = NoResize;
    for(int itemIndex = 0; itemIndex < controlsCount; ++itemIndex){
        Controls item = static_cast<Controls>(itemIndex);
        if(rectOf(item).contains(event->pos())){
            _resizeMode = resizeModeOf(item);
            break;
        }
    }
}

void WidgetAreaSelect::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    _resizeMode = NoResize;
}

void WidgetAreaSelect::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    drawBackground(&painter);
    drawSelect(&painter);
}

void WidgetAreaSelect::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(!_interacted)
        _rectInner = QRect(QPoint(10,10),QSize(200,200));// test
}

void WidgetAreaSelect::init()
{
    setMouseTracking(true);
    _interacted = false;
    _resizeMode = NoResize;
}

void WidgetAreaSelect::drawBackground(QPainter *painter)
{
    QPixmap pixmapOuter(size());
    QPainter painterPixmap(&pixmapOuter);
    pixmapOuter.fill(QColor(Qt::gray)); // test
    painterPixmap.setBrush(QBrush(QColor(0,0,0, 128)));
    painterPixmap.drawRect(pixmapOuter.rect());

    painter->drawPixmap(rect(), pixmapOuter);
}

void WidgetAreaSelect::drawSelect(QPainter *painter)
{
    QPixmap pixmapInner(size());
    pixmapInner.fill(QColor(Qt::gray)); // test

    painter->drawPixmap(_rectInner,pixmapInner);
    drawControlItems(painter);
}

void WidgetAreaSelect::drawControlItems(QPainter *painter)
{
    QPixmap pixmapControl(defaultControlPointEdge,defaultControlPointEdge);
//    QPainter painterPixmap(&pixmapControl);
//    painterPixmap.setBrush(QBrush(QColor(0,0,0, 128)));
//    painterPixmap.drawRect(pixmapControl.rect());
    pixmapControl.fill(QColor(Qt::gray));

    painter->save();
    painter->setOpacity(0.5);

    QPoint center = _rectInner.center();

    drawControlItem(leftTopRect(),
                    pixmapControl, painter);
    drawControlItem(centerTopRect(),
                    pixmapControl, painter);
    drawControlItem(rightTopRect(),
                    pixmapControl, painter);
    drawControlItem(rightCenterRect(),
                    pixmapControl, painter);
    drawControlItem(rightBottomRect(),
                    pixmapControl, painter);
    drawControlItem(centerBottomRect(),
                    pixmapControl, painter);
    drawControlItem(leftBottomRect(),
                    pixmapControl, painter);
    drawControlItem(leftCenterRect(),
                    pixmapControl, painter);

    painter->restore();
}

void WidgetAreaSelect::drawControlItem(const QRect &rect, const QPixmap &pixmap, QPainter *painter)
{
    painter->drawPixmap(rect, pixmap);
}

QRect WidgetAreaSelect::leftTopRect() const
{
    return controlItemRectByPos(_rectInner.topLeft());
}

QRect WidgetAreaSelect::centerTopRect() const
{
    return controlItemRectByPos(QPoint(_rectInner.center().x(), _rectInner.top()));
}

QRect WidgetAreaSelect::rightTopRect() const
{
    return controlItemRectByPos(_rectInner.topRight());
}

QRect WidgetAreaSelect::rightCenterRect() const
{
    return controlItemRectByPos(QPoint(_rectInner.right(), _rectInner.center().y()));
}

QRect WidgetAreaSelect::rightBottomRect() const
{
    return controlItemRectByPos(_rectInner.bottomRight());
}

QRect WidgetAreaSelect::centerBottomRect() const
{
    return controlItemRectByPos(QPoint(_rectInner.center().x(), _rectInner.bottom()));
}

QRect WidgetAreaSelect::leftBottomRect() const
{
    return controlItemRectByPos(_rectInner.bottomLeft());
}

QRect WidgetAreaSelect::leftCenterRect() const
{
    return controlItemRectByPos(QPoint(_rectInner.left(), _rectInner.center().y()));
}

QRect WidgetAreaSelect::rectOf(WidgetAreaSelect::Controls item) const
{
    switch(item){
    case controlLeftTop:
        return leftTopRect();
    case controlCenterTop:
        return centerTopRect();
    case controlRightTop:
        return rightTopRect();
    case controlRightCenter:
        return rightCenterRect();
    case controlRightBottom:
        return rightBottomRect();
    case controlCenterBottom:
        return centerBottomRect();
    case controlLeftBottom:
        return leftBottomRect();
    case controlLeftCenter:
        return leftCenterRect();
    default:
        return QRect();
    }
}

Qt::CursorShape WidgetAreaSelect::shapeOf(Controls item) const
{
    switch(item){
    case controlLeftTop:
        return Qt::SizeFDiagCursor;
    case controlCenterTop:
        return Qt::SizeVerCursor;
    case controlRightTop:
        return Qt::SizeBDiagCursor;
    case controlRightCenter:
        return Qt::SizeHorCursor;
    case controlRightBottom:
        return Qt::SizeFDiagCursor;
    case controlCenterBottom:
        return Qt::SizeVerCursor;
    case controlLeftBottom:
        return Qt::SizeBDiagCursor;
    case controlLeftCenter:
        return Qt::SizeHorCursor;
    default:
        return Qt::ArrowCursor;
    }
}

WidgetAreaSelect::ResizeModes WidgetAreaSelect::resizeModeOf(WidgetAreaSelect::Controls item) const
{
    switch(item){
    case controlLeftTop:
    case controlRightTop:
    case controlRightBottom:
    case controlLeftBottom:
        return DiagonalResize;
    case controlCenterTop:
    case controlCenterBottom:
        return HorizontalResize;
    case controlRightCenter:
    case controlLeftCenter:
        return VerticalResize;
    default:
        return NoResize;
    }
}

QRect WidgetAreaSelect::controlItemRectByPos(const QPoint &pos) const
{
    return QRect(pos.x() - defaultControlPointHalfEdge,
                 pos.y() - defaultControlPointHalfEdge,
                 defaultControlPointEdge, defaultControlPointEdge );
}
