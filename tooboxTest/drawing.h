#ifndef DRAWING_H
#define DRAWING_H

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>


class drawing : public QWidget
{
public:
    drawing();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void updateImage(bool isThereImage){this->isThereAnImage=isThereImage;};
private:
    QPoint startPos;
    QPoint endPos;
    bool canDraw;
    bool canIDraw;
    bool isThereAnImage;
    QVector<QRect> rectangles;
    QPixmap pictureToDisplay;
    QPainter painter;
    int widthDrawing;
    int heightDrawing;
    int beginXDrawing;
    int beginYDrawing;
};

#endif // DRAWING_H
