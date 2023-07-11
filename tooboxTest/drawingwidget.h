#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <vector>
#include <QLabel>
#include <iostream>

using namespace std;

using namespace std;

class drawingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit drawingWidget(QWidget *parent = nullptr);
    // Events
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    // Méthodes
    /**
     * @brief updateImage : permet de savoir s'il y a une image ou non
     * @param isThereImage : booléen
     */
    void updateImage(bool isThereImage){this->isThereAnImage=isThereImage;}
    /**
     * @brief updateMode : permet de mettre à jour le mode, ici on dessine
     * @param drawMode : booléen
     */
    void updateMode(bool drawMode){this->canIDraw=drawMode;}
    /**
     * @brief brushMode : permet de mettre à jour l'outil, ici le pinceau
     * @param brush : booléen
     */
    void brushMode(bool brush){this->canBrush=brush;}
    /**
     * @brief changeSizeBrush : permet de changer la taille du pinceau
     * @param size : entier
     */
    void changeSizeBrush(int size){this->sizeBrush=size;}
    /**
     * @brief changeTransparence : permet de mettre à jour l'opacité du masque (entre 0% et 100%)
     * @param transparence : entier entre 0 et 100
     */
    void changeTransparence(float transparence){this->opacity=transparence/100;}
    /**
     * @brief clean : permet d'effacer tous les traits
     */
    void clean();
    /**
     * @brief freeMode : permet de mettre à jour l'outil, ici le freeHand
     * @param free : booléen
     */
    void freeMode(bool free){this->canDoFreeHand=free;}
    /**
     * @brief polygonMode : permet de mettre à jour l'outil, ici le polygone
     * @param polygon : booléen
     */
    void polygonMode(bool polygon){this->canDrawPolygons=polygon;}
    /**
     * @brief eraseMode : permet de mettre à jour le mode, ici on efface
     * @param erase : booléen
     */
    void eraseMode(bool erase){this->canErase=true;}

    //void setBackground(QPixmap background);
    void clearModes();
    void clearToolModes();
    //void setSource(QPixmap source){this->pictureToDisplay=source;}
    /**
     * @brief setSource : permet de définir l'image dont on souhaite traiter le masque
     * @param source : QString donnant le chemin de l'image à traiter
     */
    void setSource(QString source){this->source=source;}
    void setupBitmap(int width,int height);
    void initDrawing(QString source,int width,int height);
    /**
     * @brief returnMask : permet de récupèrer le masque que l'on a généré
     * @return this->mask : masque que l'on génère
     */
    QBitmap returnMask(){return(this->mask);}
private:
    // QPoints
    QPoint startPos;
    QPoint endPos;
    // Booléens
    bool canDraw;
    bool canIDraw;
    bool isThereAnImage;
    bool canBrush;
    bool canDoFreeHand;
    bool canDrawPolygons;
    bool canErase;
    QString source = "/Users/arthur/Downloads/IMG_7245.jpg";
    QImage image;
    QBitmap mask;
    // QVector
    QVector<QRect> rectangles;
    // paintEvent
    QColor color=Qt::red;
    QPixmap pictureToDisplay;
    QPainter painter;
    QPixmap pixmap;
    QBitmap bitmap;
    // entiers et flotants
    int sizeBrush;
    float opacity=.5;
    // vector
    vector<QPoint> trace;
    vector<QPen> pinceaux;
    vector<QPoint> zones;
    vector<QPolygon> polygons;
    vector<QPoint> polygon;


signals:

};

#endif // DRAWINGWIDGET_H
