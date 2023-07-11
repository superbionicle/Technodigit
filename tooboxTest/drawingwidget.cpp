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
    this->color=Qt::red;
    this->colorTool=Qt::black;
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

    // Si bouton gauche + il y a une image + outil dessin sélectionné
    if (event->button() == Qt::LeftButton && this->isThereAnImage)
    {
        this->canDraw = true;

        // Outil pinceau ou gomme
        if(this->canBrush || this->canErase){

            // Ajout du point actuel
            this->trace.push_back(event->pos());
        }

        // Outil main levée
        if(this->canDoFreeHand){
            this->zones.push_back(event->pos());
        }

        // Outil polygone
        if(this->canDrawPolygons){
            if(this->polygon.size()!=0){

                // Painter
                QPainter painter(&this->mask);
                painter.setPen(QPen(this->colorTool,1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

                // Dessin en temps réel
                painter.drawLine(this->polygon.back(),event->pos());
            }

            // Ajout du point actuel
            this->polygon.push_back(event->pos());
        }
    }
}

/**
 * @brief drawingWidget::mouseMoveEvent : gestion du mouvement de la souris
 * @param event
 */
void drawingWidget::mouseMoveEvent(QMouseEvent *event){

    // S'il y a une image
    if (this->isThereAnImage && this->canDraw){

        // Outil pinceau ou gomme
        if(this->canErase || this->canBrush && this->trace.size()!=0){

            // Painter
            QPainter painter(&this->mask);
            painter.setPen(QPen(this->colorTool,this->sizeBrush,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

            // Si le précédent point n'est pas un point d'arrêt
            if(this->trace.back()!=QPoint(-1,-1)){
                painter.drawLine(this->trace.back(),event->pos());
            }

            // Ajout du point actuel
            this->trace.push_back(event->pos());
        }

        if(this->canDoFreeHand && this->zones.size()!=0){
            // Painter
            QPainter painter(&this->mask);
            painter.setPen(QPen(this->colorTool,1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

            // Dessin en temps réel
            painter.drawLine(this->zones.back(),event->pos());

            // Ajout du point actuel
            this->zones.push_back(event->pos());
        }
    }
}

/**
 * @brief drawingWidget::mouseReleaseEvent : gestion du relachement du clic de la souris
 * @param event
 */
void drawingWidget::mouseReleaseEvent(QMouseEvent *event) {
    // Si bouton gauche + il y a une image + outil dessin sélectionné
    if (event->button() == Qt::LeftButton && this->isThereAnImage)
    {
        this->canDraw = false;

        // Outil pinceau ou gomme
        if(this->canBrush){

            // Painter
            QPainter painter(&this->mask);
            painter.setPen(QPen(this->colorTool,this->sizeBrush,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

            // Si le précédent point n'est pas un point d'arrêt
            if(this->trace.back()!=QPoint(-1,-1)){
                painter.drawLine(this->trace.back(),event->pos());
            }

            // Ajout du point actuel
            this->trace.push_back(event->pos());

            // Ajout du point d'arrêt au vector
            this->trace.push_back(QPoint(-1,-1));

        }

        // Outil main levée
        if(this->canDoFreeHand){

            // Ajout du point actuel
            this->zones.push_back(event->pos());

            // Painter
            QPainter painter(&this->mask);
            painter.setPen(QPen(this->colorTool,1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

            // Polygone
            QPolygon polygonTemp;

            // Création du polygone
            for(int i=0;i<this->zones.size()-1;i++){
                polygonTemp<<this->zones[i];
            }

            // On vide la liste
            this->zones.clear();

            // PainterPath
            QPainterPath path;
            path.addPolygon(polygonTemp);
            painter.fillPath(path,QBrush(this->colorTool,Qt::SolidPattern));
            //painter.drawPolygon(polygonTemp);

        }
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

            // Painter
            QPainter painter(&this->mask);
            painter.setPen(QPen(this->colorTool,1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

            // Polygone
            QPolygon polygonTemp;

            // Création du polygone
            for(int i=0;i<this->polygon.size();i++){
                polygonTemp<<this->polygon[i];
            }

            // On vide la liste
            this->polygon.clear();

            // PainterPath
            QPainterPath path;
            path.addPolygon(polygonTemp);
            painter.fillPath(path,QBrush(this->colorTool,Qt::SolidPattern));
            //painter.drawPolygon(polygonTemp);
        }
    }
}

/**
 * @brief drawingWidget::paintEvent : affichage des tracés à l'écran
 * @param event
 */
void drawingWidget::paintEvent(QPaintEvent *event){

    // Painter
    QPainter painter(this);

    // Transparence du masque
    painter.setOpacity(this->opacity);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipRegion(QRegion(this->mask));
    painter.fillRect(0,0,this->width(),this->height(),this->color);

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


