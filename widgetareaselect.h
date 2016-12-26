#ifndef WIDGETAREASELECT_H
#define WIDGETAREASELECT_H

#include <QWidget>

class WidgetAreaSelect : public QWidget
{
    Q_OBJECT

public:
    enum ResizeModes{
        DiagonalResize,
        HorizontalResize,
        VerticalResize,
        NoResize
    };

    enum Controls{
        controlFirst=0,
        controlLeftTop=0,
        controlCenterTop,   //  1
        controlRightTop,    //  2
        controlRightCenter, //  3
        controlRightBottom, //  4
        controlCenterBottom,//  5
        controlLeftBottom,  //  6
        controlLeftCenter,  //  7

        controlNothing,     //  8
        controlsCount = controlNothing
    };

    WidgetAreaSelect(QWidget *parent = 0);
    virtual ~WidgetAreaSelect();


protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

private:
    void init();

    // painting
    void drawBackground(QPainter *painter);
    void drawSelect(QPainter *painter);
    void drawControlItems(QPainter *painter);
    void drawControlItem(const QRect &rect, const QPixmap &pixmap, QPainter *painter);

    QRect leftTopRect() const;
    QRect centerTopRect() const;
    QRect rightTopRect() const;
    QRect rightCenterRect() const;
    QRect rightBottomRect() const;
    QRect centerBottomRect() const;
    QRect leftBottomRect() const;
    QRect leftCenterRect() const;

    QRect rectOf(Controls item) const;
    Qt::CursorShape shapeOf(Controls item) const;
    ResizeModes resizeModeOf(Controls item) const;

    // QRect set* functions with check limits
    void trySetLeft(int pos);
    void trySetBottom(int pos);
    void trySetRight(int pos);
    void trySetTop(int pos);

    Controls masterItem(const QPoint &pos) const;
    void updateMasterItem(const QPoint &pos);

    //


    QRect controlItemRectByPos(const QPoint &pos) const;

private:
    const int _minWidth,_minHeight;

    Controls _masterItem;
    ResizeModes _resizeMode;
    QRect _rectInner;
    bool _interacted;
};

#endif // WIDGETAREASELECT_H
