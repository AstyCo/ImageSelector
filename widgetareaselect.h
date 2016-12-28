#ifndef WIDGETAREASELECT_H
#define WIDGETAREASELECT_H

#include <QWidget>

class WidgetAreaSelect : public QWidget
{
    Q_OBJECT

public:
    enum ResizeModes {
        DiagonalResize,
        HorizontalResize,
        VerticalResize,
        AllResize, // drag mode
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

    void setImagePath(const QString &path);     ///< Путь к файлу изображения

    void setActualSize(int width, int height);  ///< Устанавливает размер области (QWidget отображает относительный размер)h, int height); ///< Устанавливает размер области (QWidget отображает относительный размер)
    void setActualSize(const QSize &size);      ///< Устанавливает размер области (QWidget отображает относительный размер)ize &size);     ///< Устанавливает размер области (QWidget отображает относительный размер)

    QRect selectedRect() const;                 ///< возвращает выбранную область                                                          ///< возвращает выбранную область
    int selectedX() const;                      ///< для удобства
    int selectedY() const;                      ///< для удобства
    int selectedWidth() const;                  ///< для удобства
    int selectedHeight() const;                 ///< для удобства

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

    // QRect set*/move* functions with check limits
    void trySetLeft(int pos);
    void trySetBottom(int pos);
    void trySetRight(int pos);
    void trySetTop(int pos);

    void tryMoveLeft(int dx);
    void tryMoveTop(int dy);

    //
    Controls masterItem(const QPoint &pos) const;
    void updateMasterItem(const QPoint &pos);
    void loadPixmaps(const QString &imagePath = QString());


    QRect controlItemRectByPos(const QPoint &pos) const;

private:
    const int _minWidth,_minHeight;
    QPoint _lastMouseEventAt;

    Controls _masterItem;
    ResizeModes _resizeMode;
    QRect _rectInner;
    QSize _actualSize;

    QPixmap _pixmapInner, _pixmapOuter, _pixmapControl;
};

#endif // WIDGETAREASELECT_H
