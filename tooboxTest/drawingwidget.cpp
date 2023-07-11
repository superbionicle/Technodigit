#include "drawingwidget.h"
#include <iostream>
#include <QWidget>
#include <QPixmap>
#include <QPen>
#include <QPainterPath>
#include <QMouseEvent>
#include <QRegion>
#include <QStyleOption>
#include <QStyle>
#include <QVBoxLayout>
#include <QBitmap>

using namespace std;

/**
 * @brief drawingWidget::drawingWidget : constructeur
 * @param parent
 */
drawingWidget::drawingWidget(QWidget *parent)
    : QWidget{parent}
{
    // Permet de dessiner dessus (sans cette ligne, impossible)
    setFocusPolicy(Qt::StrongFocus);
}

/**
 * @brief drawingWidget::initDrawing : permet d'initialiser correctement le widget après le chargement d'une image
 * @param source : chemin de l'image que l'on traite
 * @param width : largeur du widget
 * @param height : hauteur du widget
 */
void drawingWidget::initDrawing(QString source,int width,int height){
    this->image.load(source);
    this->mask=QBitmap(width,height);
    this->mask.fill(Qt::white);
}

/**
 * @brief drawingWidget::mousePressEvent : gestion de la détection d'un clic de la souris
 * @param event
 */
void drawingWidget::mousePressEvent(QMouseEvent *event){
    /*cout<<"Selection"<<this->canIDraw<<endl;
    cout<<"Gomme"<<this->canErase<<endl;
    cout<<"Pinceau"<<this->canBrush<<endl;
    cout<<"Free Hand"<<this->canDoFreeHand<<endl;
    cout<<"Polygone"<<this->canDrawPolygons<<endl<<endl;*/

    // Si bouton gauche + il y a une image + outil dessin sélectionné
    if (event->button() == Qt::LeftButton && this->isThereAnImage && this->canIDraw)
    {
        //this->startPos = event->pos();
        this->canDraw = true;

        // Outil pinceau ou gomme
        if(this->canBrush || this->canErase){
            //this->trace.push_back(event->pos());
            //this->pinceaux.push_back(QPen(Qt::black,this->sizeBrush,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            //QPainter painter(&this->mask);
            //painter.setPen(QPen(Qt::black,this->sizeBrush,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            //this->trace.push_back(QPoint(-1,-1));
            this->trace.push_back(event->pos());

        }

        // Outil gomme
        /*if(this->canErase){
            //this->trace.push_back(event->pos());
            //this->pinceaux.push_back(QPen(Qt::white,this->sizeBrush,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            //QPainter painter(&this->mask);
            //painter.setPen(QPen(Qt::black,this->sizeBrush,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            this->trace.push_back(event->pos());
        }*/

        //cout<<this->pinceaux<<endl;
        /*for(int i=0;i<this->pinceaux.size();i++){
            cout<<this->pinceaux[i].color().name().toStdString()<<endl;
        }*/
        /*// Outil main levée
        if(this->canDoFreeHand){
            this->zones.push_back(event->pos());
        }

        // Outil polygone
        if(this->canDrawPolygons){
            this->polygon.push_back(event->pos());
        }*/
    }
}

/**
 * @brief drawingWidget::mouseMoveEvent : gestion du mouvement de la souris
 * @param event
 */
void drawingWidget::mouseMoveEvent(QMouseEvent *event){

    // S'il y a une image
    if (this->isThereAnImage){
        //this->endPos = event->pos();

        // Outil pinceau ou gomme
        if(this->canErase || this->canBrush){
            cout<<"Selection"<<this->canIDraw<<endl;
            cout<<"Gomme"<<this->canErase<<endl;
            cout<<"Pinceau"<<this->canBrush<<endl;
            QPainter painter(&this->mask);
            painter.save();
            if(this->canBrush){
                painter.setPen(QPen(Qt::black,this->sizeBrush,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            }
            if(this->canErase){
                cout<<this->canErase<<endl;
                painter.setPen(QPen(Qt::black,this->sizeBrush,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            }
            cout<<this->trace.back().x()<<" "<<this->trace.back().y()<<endl;
            if(this->trace.back()!=QPoint(-1,-1)){
                painter.drawLine(this->trace.back(),event->pos());
                this->trace.push_back(event->pos());
            }else{
                cout<<"Blop"<<endl;
            }
            painter.restore();
        }

        /*// Outil main levée
        if(canDoFreeHand){
            this->zones.push_back(event->pos());
        }*/
    }
}

/**
 * @brief drawingWidget::mouseReleaseEvent : gestion du relachement du clic de la souris
 * @param event
 */
void drawingWidget::mouseReleaseEvent(QMouseEvent *event) {
    // Si bouton gauche + il y a une image + outil dessin sélectionné
    if (event->button() == Qt::LeftButton && this->isThereAnImage && this->canIDraw)
    {
        //this->endPos = event->pos();
        this->canDraw = false;

        // Outil pinceau ou gomme
        if(this->canBrush || this->canErase){
            /*QPainter painter(&this->mask);
            painter.setPen(this->pinceaux.back());
            painter.drawLine(this->trace.back(),event->pos());
            this->trace.push_back(QPoint(-1,-1));*/
            QPainter painter(&this->mask);
            painter.save();
            if(this->canBrush){
                painter.setPen(QPen(Qt::black,this->sizeBrush,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            }
            else if(this->canErase){
                painter.setPen(QPen(Qt::white,this->sizeBrush,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            }
            if(this->trace.back()!=QPoint(-1,-1)){
                painter.drawLine(this->trace.back(),event->pos());
                this->trace.push_back(event->pos());
            }
            painter.restore();
            this->trace.push_back(QPoint(-1,-1));
        }

        /*// Outil main levée
        if(this->canDoFreeHand){
            this->zones.push_back(event->pos());
            this->zones.push_back(QPoint(-1,-1));
            QPolygon polygone;

            // Création du polygone
            for(int i=0;i<this->zones.size()-1;i++){
                polygone<<this->zones[i];
            }

            this->polygons.push_back(polygone);

            // On vide la liste
            this->zones.clear();

        }*/
    }
}

/**
 * @brief drawingWidget::keyPressEvent : gestion de la pression d'une touche du clavier
 * @param event
 */
void drawingWidget::keyPressEvent(QKeyEvent *event){
    // Touche espace ou Enter
    if(event->key() == Qt::Key_Space || event->key() == Qt::Key_Enter){
        // Outil Polygone
        if(this->canDrawPolygons){
            QPolygon polygonTemp;

            // Création du polygone
            for(int i=0;i<this->polygon.size();i++){
                polygonTemp<<this->polygon[i];
            }

            this->polygons.push_back(polygonTemp);
            this->polygon.clear();
        }
    }
}

/**
 * @brief drawingWidget::paintEvent : affichage des tracés à l'écran
 * @param event
 */
void drawingWidget::paintEvent(QPaintEvent *event){
    // Code nécessaire pour gérer le fond du widget
    //---------------------------------------------------------
    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.initFrom(this);

    // Painter
    QPainter painter(this);
    painter.setOpacity(this->opacity);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipRegion(QRegion(this->mask));
    painter.fillRect(0,0,this->width(),this->height(),this->color);

    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    //---------------------------------------------------------

    // Si on a une image
    //if(isThereAnImage){
        //painter.setOpacity(this->opacity);

        // Si on a dessiné au pinceau ou à la gomme
        /*if(this->trace.size()>0){
            int j = 0;
            for(int i=0;i<this->trace.size()-1;i++){
                if(this->trace[i+1] == QPoint(-1,-1)){
                    i+=2;
                    j++;
                }
                else{
                    painter.setPen(this->pinceaux[j]);
                    // Outil gomme
                    if(this->pinceaux[j].color()==Qt::white){
                        //cout<<"Pouet";
                        //painter.save();
                        //painter.setPen(this->pinceaux[j]);
                        //QRegion erased(QRect(this->trace[i],this->trace[i+1]).normalized());
                        QPainterPath erased;
                        erased.moveTo(this->trace[i]);
                        erased.lineTo(this->trace[i+1]);
                        //painter.setClipRect(QRect(this->trace[i],this->trace[i+1]));
                        //painter.drawLine(this->trace[i],this->trace[i+1]);
                        //painter.eraseRect(QRect(this->trace[i],this->trace[i+1]));
                        //painter.eraseEllipse();
                        //painter.eraseRect();

                        //this->mask=this->
                        //painter.setRenderHint(QPainter::Antialiasing);
                        //painter.restore();

                    }
                    // Outil pinceau
                    else{
                        painter.save();
                        painter.setPen(this->pinceaux[j]);
                        //painter.setCompositionMode();
                        painter.drawLine(this->trace[i],this->trace[i+1]);
                        painter.restore();
                    }
                }
            }
        }*/
        // Si on a fait des dessins à main levée (dessin en temp réel des contours)
        /*if(this->zones.size()>0){
            for(int i=0;i<this->zones.size()-1;i++){
                painter.setPen(QPen(Qt::red,5,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
                if(this->zones[i+1] != QPoint(-1,-1)){
                    painter.drawLine(this->zones[i],this->zones[i+1]);
                }
            }
        }
        // Si on a dessiné des polygones (dessin en temp réel des contours)
        if(this->polygon.size()>0){
            painter.setPen(QPen(Qt::red,5,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            for(int i=0;i<this->polygon.size()-1;i++){
                painter.drawLine(this->polygon[i],this->polygon[i+1]);
            }
        }
        // Affichage de tous les polygones et dessins à main levée
        for(int i=0;i<this->polygons.size();i++){
            QPainterPath path;
            painter.setPen(QPen(Qt::red,5,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
            path.addPolygon(polygons[i]);
            painter.fillPath(path,QBrush(Qt::red,Qt::SolidPattern));
            painter.drawPolygon(this->polygons[i]);
        }
    }*/
    // Mise à jour de l'écran
    this->update();
}

/**
 * @brief drawingWidget::clean : nettoyage de ce qui a été dessiné
 */
void drawingWidget::clean(){
    this->pinceaux.clear();
    this->trace.clear();
    this->polygons.clear();
    this->mask.fill(Qt::white);
}

/**
 * @brief drawingWidget::clearModes : on remet à false les modes de dessin et de gomme
 */
void drawingWidget::clearToolModes(){
    this->canDraw=false;
    this->canIDraw=false;
    this->canErase=false;
}

/**
 * @brief drawingWidget::clearModes : on remet à false les modes pinceaux, freeHand et polygone
 */
void drawingWidget::clearModes(){
    this->canBrush=false;
    this->canDoFreeHand=false;
    this->canDrawPolygons=false;
}


