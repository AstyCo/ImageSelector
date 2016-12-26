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
    : QWidget(parent), _minWidth(50),_minHeight(50)
{
    init();
}

WidgetAreaSelect::~WidgetAreaSelect()
{

}

void WidgetAreaSelect::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    const QPoint &pos = event->pos();
    updateMasterItem(pos);

    switch(_resizeMode){
    case NoResize:
        setCursor(QCursor());
        for(int itemIndex = 0; itemIndex < controlsCount; ++itemIndex){
            Controls item = static_cast<Controls>(itemIndex);
            if(rectOf(item).contains(pos)){
                setCursor(QCursor(shapeOf(item)));
                break;
            }
        }
        break;
    case HorizontalResize:
        switch(_masterItem){
        case controlLeftCenter:
            trySetLeft(pos.x());
            break;
        case controlRightCenter:
            trySetRight(pos.x());
            break;
        default:
            Q_ASSERT(false);
        }
        break;
    case VerticalResize:
        switch(_masterItem){
        case controlCenterTop:
            trySetTop(pos.y());
            break;
        case controlCenterBottom:
            trySetBottom(pos.y());
            break;
        default:
            Q_ASSERT(false);
        }
        break;
    case DiagonalResize:
        switch(_masterItem){
        case controlLeftTop:
            trySetLeft(pos.x());
            trySetTop(pos.y());
            break;
        case controlRightTop:
            trySetRight(pos.x());
            trySetTop(pos.y());
            break;
        case controlRightBottom:
            trySetRight(pos.x());
            trySetBottom(pos.y());
            break;
        case controlLeftBottom:
            qDebug() << "case Left Bottom";
            trySetLeft(pos.x());
            trySetBottom(pos.y());
            break;
        default:
            Q_ASSERT(false);
        }
        break;
    }



}

void WidgetAreaSelect::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button()!=Qt::LeftButton)
        return;

    _resizeMode = NoResize;
    _masterItem = controlNothing;
    for(int itemIndex = 0; itemIndex < controlsCount; ++itemIndex){
        Controls item = static_cast<Controls>(itemIndex);
        if(rectOf(item).contains(event->pos())){
            _masterItem = item;
            _resizeMode = resizeModeOf(item);
            break;
        }
    }
}

void WidgetAreaSelect::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    _resizeMode = NoResize;
    _masterItem = controlNothing;
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
    _masterItem = controlNothing;
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
        return VerticalResize;
    case controlRightCenter:
    case controlLeftCenter:
        return HorizontalResize;
    default:
        return NoResize;
    }
}

void WidgetAreaSelect::trySetLeft(int pos)
{
    qDebug() << "trySetLeft "<< pos;
    if(pos < rect().left())
        pos = rect().left();
    if(pos > _rectInner.right() - _minWidth)
        pos = _rectInner.right() - _minWidth;

    _rectInner.setLeft(pos);
    update();
}

void WidgetAreaSelect::trySetTop(int pos)
{
    qDebug() << "trySetTop "<< pos;
    if(pos < rect().top())
        pos = rect().top();
    if(pos > _rectInner.bottom() - _minHeight)
        pos = _rectInner.bottom() - _minHeight;

    _rectInner.setTop(pos);
    update();
}


void WidgetAreaSelect::trySetRight(int pos)
{
    qDebug() << "trySetRight "<< pos;

    if(pos > rect().right())
        pos = rect().right();
    if(pos < _rectInner.left() + _minWidth)
        pos = _rectInner.left() + _minWidth;

    _rectInner.setRight(pos);
    update();
}

void WidgetAreaSelect::trySetBottom(int pos)
{
    qDebug() << "trySetBottom "<< pos;

    if(pos > rect().bottom())
        pos = rect().bottom();
    if(pos < _rectInner.top() + _minHeight)
        pos = _rectInner.top() + _minHeight;

    _rectInner.setBottom(pos);
    update();
}

void WidgetAreaSelect::updateMasterItem(const QPoint &pos)
{
    Controls newMasterItem = masterItem(pos);
    if(newMasterItem != _masterItem){
        _masterItem = newMasterItem;
        qDebug() << "new MasterItem: "<< _masterItem;
        setCursor(QCursor(shapeOf(_masterItem)));
    }
}

WidgetAreaSelect::Controls WidgetAreaSelect::masterItem(const QPoint &pos) const
{
    const QPoint &center = _rectInner.center();
    switch(_resizeMode){
    case DiagonalResize:
        if(pos.x() < center.x())
            if(pos.y() < center.y())
                return controlLeftTop;
            else
                return controlLeftBottom;
        else
            if(pos.y() > center.y())
                return controlRightBottom;
            else
                return controlRightTop;
        break;
    case HorizontalResize:
        if(pos.x() < center.x())
            return controlLeftCenter;
        else
            return controlRightCenter;
        break;
    case VerticalResize:
        if(pos.y() < center.y())
            return controlCenterTop;
        else
            return controlCenterBottom;
    case NoResize:
        return controlNothing;
    default:
        Q_ASSERT(false);
        return controlNothing;
    }
}


QRect WidgetAreaSelect::controlItemRectByPos(const QPoint &pos) const
{
    return QRect(pos.x() - defaultControlPointHalfEdge,
                 pos.y() - defaultControlPointHalfEdge,
                 defaultControlPointEdge, defaultControlPointEdge );
}
