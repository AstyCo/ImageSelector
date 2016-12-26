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
        controlCenterTop,
        controlRightTop,
        controlRightCenter,
        controlRightBottom,
        controlCenterBottom,
        controlLeftBottom,
        controlLeftCenter,

        controlNothing,
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

    QRect controlItemRectByPos(const QPoint &pos) const;

private:

    ResizeModes _resizeMode;
    QRect _rectInner;
    bool _interacted;
};

#endif // WIDGETAREASELECT_H
