#include "widgetareaselect.h"

#include <QApplication>
#include <QResizeEvent>
#include <QPainter>
#include <QTransform>
#include <QResizeEvent>
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

void WidgetAreaSelect::setImagePath(const QString &path)
{
    loadPixmaps(path);
}

void WidgetAreaSelect::setActualSize(int width, int height)
{
    setActualSize(QSize(width,height));
}

void WidgetAreaSelect::setActualSize(const QSize &size)
{
    _actualSize = size;
}

QRect WidgetAreaSelect::selectedRect() const
{
    if(_actualSize.isValid()){
        QTransform matrix;
        matrix.scale(
                    _actualSize.width() / rect().width(),
                    _actualSize.height() / rect().height() );
        return matrix.mapRect(_rectInner);
    }
    return _rectInner;
}

int WidgetAreaSelect::selectedX() const
{
    return selectedRect().x();
}

int WidgetAreaSelect::selectedY() const
{
    return selectedRect().y();
}

int WidgetAreaSelect::selectedWidth() const
{
    return selectedRect().width();
}

int WidgetAreaSelect::selectedHeight() const
{
    return selectedRect().height();
}

void WidgetAreaSelect::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    const QPoint &pos = event->pos();
    updateMasterItem(pos);

    switch(_resizeMode){
    case NoResize:
        setCursor(QCursor());
        if(_rectInner.contains(pos))
            setCursor(QCursor(Qt::SizeAllCursor));
        for(int itemIndex = 0; itemIndex < controlsCount; ++itemIndex){
            Controls item = static_cast<Controls>(itemIndex);
            if(rectOf(item).contains(pos)){
                setCursor(QCursor(shapeOf(item)));
                break;
            }
        }
        break;
    case AllResize:
        if(_lastMouseEventAt.isNull())
            break;
        tryMoveLeft(pos.x() - _lastMouseEventAt.x());
        tryMoveTop(pos.y() - _lastMouseEventAt.y());
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

    _lastMouseEventAt = event->pos();
}

void WidgetAreaSelect::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button()!=Qt::LeftButton)
        return;

    if(_rectInner.contains(event->pos()))
        _resizeMode = AllResize;
    else
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
    painter.setRenderHint(QPainter::Antialiasing);          // antialiasing
    painter.setRenderHint(QPainter::SmoothPixmapTransform); // QPixmap proper transformation

    drawBackground(&painter);
    drawSelect(&painter);
}

void WidgetAreaSelect::resizeEvent(QResizeEvent *event)
{
    if(!event->oldSize().isValid()){    //  fits QWidget's rect(),
        _rectInner = rect();            //  by default
        QWidget::resizeEvent(event);    //
        return;                         //
    }                                   //
    QTransform matrix;
    matrix.scale(event->size().width() * 1.0 / event->oldSize().width(),
                 event->size().height() * 1.0 / event->oldSize().height());

    _rectInner = matrix.mapRect(_rectInner);
    QWidget::resizeEvent(event);
    update();
}

void WidgetAreaSelect::init()
{
    setMouseTracking(true);
    _resizeMode = NoResize;
    _masterItem = controlNothing;

    _pixmapControl = QPixmap(defaultControlPointEdge,defaultControlPointEdge);
    _pixmapControl.fill(QColor(Qt::gray));
    loadPixmaps();
//    loadPixmaps(":/resource/Capture001.png");
}

void WidgetAreaSelect::drawBackground(QPainter *painter)
{
    painter->drawPixmap(rect(), _pixmapOuter);
}

void WidgetAreaSelect::drawSelect(QPainter *painter)
{
    QTransform matrix;
    matrix.scale(_pixmapInner.rect().width() * 1.0 / rect().width(),
                 _pixmapInner.rect().height() * 1.0 / rect().height());

    painter->drawPixmap(_rectInner,_pixmapInner,matrix.mapRect(_rectInner));
    drawControlItems(painter);
}

void WidgetAreaSelect::drawControlItems(QPainter *painter)
{
    painter->save();
    painter->setOpacity(0.5);

    QPoint center = _rectInner.center();

    drawControlItem(leftTopRect(),
                    _pixmapControl, painter);
    drawControlItem(centerTopRect(),
                    _pixmapControl, painter);
    drawControlItem(rightTopRect(),
                    _pixmapControl, painter);
    drawControlItem(rightCenterRect(),
                    _pixmapControl, painter);
    drawControlItem(rightBottomRect(),
                    _pixmapControl, painter);
    drawControlItem(centerBottomRect(),
                    _pixmapControl, painter);
    drawControlItem(leftBottomRect(),
                    _pixmapControl, painter);
    drawControlItem(leftCenterRect(),
                    _pixmapControl, painter);

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
        if(_resizeMode == AllResize)
            return Qt::SizeAllCursor;
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

void WidgetAreaSelect::trySetRight(int pos)
{
    if(pos > rect().right())
        pos = rect().right();
    if(pos < _rectInner.left() + _minWidth)
        pos = _rectInner.left() + _minWidth;

    _rectInner.setRight(pos);
    update();
}

void WidgetAreaSelect::trySetBottom(int pos)
{
    if(pos > rect().bottom())
        pos = rect().bottom();
    if(pos < _rectInner.top() + _minHeight)
        pos = _rectInner.top() + _minHeight;

    _rectInner.setBottom(pos);
    update();
}

void WidgetAreaSelect::trySetLeft(int pos)
{
    if(pos < rect().left())
        pos = rect().left();
    if(pos > _rectInner.right() - _minWidth)
        pos = _rectInner.right() - _minWidth;

    _rectInner.setLeft(pos);
    update();
}

void WidgetAreaSelect::trySetTop(int pos)
{
    if(pos < rect().top())
        pos = rect().top();
    if(pos > _rectInner.bottom() - _minHeight)
        pos = _rectInner.bottom() - _minHeight;

    _rectInner.setTop(pos);
    update();
}

void WidgetAreaSelect::tryMoveLeft(int dx)
{
    int pos = _rectInner.x() + dx;
    if(pos < rect().left())
        pos = rect().left();
    if(pos >  rect().right() - _rectInner.width())
        pos = rect().right() - _rectInner.width() + 1;

    _rectInner.moveLeft(pos);
    update();
}

void WidgetAreaSelect::tryMoveTop(int dy)
{
    int pos = _rectInner.y() + dy;
    if(pos < rect().top())
        pos = rect().top();
    if(pos > rect().bottom() - _rectInner.height())
        pos = rect().bottom() - _rectInner.height() + 1;

    _rectInner.moveTop(pos);
    qDebug() << "rect().bottom() - _rectInner.height() " << rect().bottom() - _rectInner.height();
    qDebug() << "bottom " << rect().bottom();
    qDebug() << "inner bottom " << _rectInner.bottom();

    update();
}

//void WidgetAreaSelect::tryMoveTo(const QPoint &pos)
//{

//    if(_pressedAt.isNull()){
//        qWarning("WidgetAreaSelect::tryMoveTo:: _pressedAt isNull");
//        return;
//    }
//    int relX = pos.x() - _pressedAt.x(),
//        relY = pos.y() - _pressedAt.y();
//    QPoint leftTopAt = _pressedAt - _pointInsideRect;

//    tryMoveLeft(leftTopAt.x() + relX);
//    tryMoveTop(leftTopAt.y() + relY);
////    tryMoveRight(_rectInner.width() + leftTopAt.x() + relX);
////    tryMoveBottom(_rectInner.height() + leftTopAt.y() + relY);
//}



void WidgetAreaSelect::updateMasterItem(const QPoint &pos)
{
    Controls newMasterItem = masterItem(pos);
    if(newMasterItem != _masterItem){
        _masterItem = newMasterItem;
        qDebug() << "new MasterItem: "<< _masterItem;
        setCursor(QCursor(shapeOf(_masterItem)));
    }
}

void WidgetAreaSelect::loadPixmaps(const QString &imagePath)
{
    _pixmapInner = QPixmap(10,10);
    _pixmapOuter = QPixmap(10,10);

    if(imagePath.isEmpty() ||
            !_pixmapInner.load(imagePath) ||
            !_pixmapOuter.load(imagePath))
    {
        _pixmapInner.fill(QColor(Qt::white));
        _pixmapOuter.fill(QColor(Qt::white));
    }
    QPainter painterOuter(&_pixmapOuter);
    painterOuter.setPen(QPen(Qt::NoPen));
//    painterOuter.setPen(QPen(QBrush(QColor(Qt::white)),0));
    painterOuter.setBrush(QBrush(QColor(0,0,0, 128)));
    painterOuter.drawRect(_pixmapOuter.rect());
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
    case AllResize:
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
