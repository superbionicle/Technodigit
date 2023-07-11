#include "drawing.h"
#include <iostream>
#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>

using namespace std;

drawing::drawing(QWidget *parent)
    : QWidget{parent}
{

}

void drawing::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton && isThereAnImage && canIDraw)
    {
        this->startPos = event->pos();
        this->canDraw = true;
    }
}

void drawing::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton && isThereAnImage && canIDraw)
    {
        this->endPos = event->pos();
        this->canDraw = false;

        this->beginXDrawing = min(this->startPos.x(), this->endPos.x());
        this->beginYDrawing = min(this->startPos.y(), this->endPos.y());
        this->widthDrawing = abs(this->startPos.x() - this->endPos.x());
        this->heightDrawing = abs(this->startPos.y() - this->endPos.y());
    }
}

void drawing::paintEvent(QPaintEvent *event){
    if(isThereAnImage && canIDraw){
        QWidget::paintEvent(event);
        QPainter painter(this->ui->drawing);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawPixmap(0,0,this->pictureToDisplay);
        painter.drawRect(this->beginXDrawing,this->beginYDrawing,this->widthDrawing,this->heightDrawing);
    }
    update();
}
