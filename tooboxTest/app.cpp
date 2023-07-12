#include "app.h"
#include "drawingwidget.h"
#include "./ui_app.h"

#include<iostream>

#include "QFileDialog"
#include <QPixmap>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QColorDialog>

using namespace std;

/**
 * @brief app::app : constructeur
 * @param parent
 */
app::app(QWidget *parent): QMainWindow(parent), ui(new Ui::app)
{
    this->colorDisplay=Qt::red;
    this->pictureToDisplay = QPixmap(this->pathImage);
    ui->setupUi(this);

    // Définition des valeurs du slider de transparence
    this->ui->transparence->setMinimum(0);
    this->ui->transparence->setMaximum(100);
    this->ui->transparence->setValue(50);

    // Définition des valeurs du slider de taille de pinceau
    this->ui->taillePinceau->setMinimum(1);
    this->ui->taillePinceau->setMaximum(50);
    this->ui->taillePinceau->setValue(10);

    // Groupe de boutons radio pour n'en sélectionner qu'un à la fois
    this->tools = new QButtonGroup;
    this->toolMode = new QButtonGroup;

    // Ajout des boutons radios de mode d'outils au groupe
    this->toolMode->addButton(this->ui->selection);
    this->toolMode->addButton(this->ui->eraser);

    // Ajout des boutons radios d'outils au groupe
    this->tools->addButton(this->ui->pinceau);
    this->tools->addButton(this->ui->freeHand);
    this->tools->addButton(this->ui->polygone);

    this->checkUp();
}

/**
 * @brief app::~app : destructeur
 */
app::~app()
{
    delete ui;
}

/**
 * @brief app::checkUp : permet de gérer le comportement de la toolBox selon s'il y a une image chargée
 */
void app::checkUp(){
    // Aucune image
    if(this->pathImage==""){
        // Mise à jour des différents paramètres
        this->ui->drawing->updateImage(false);
        this->isThereAnImage=false;
        //this->ui->drawing->draw(true);
        pathImage="";

        // Suppresion du chemin
        this->ui->nothing->setText("No Image");

        // Radio boutons
            // Aucun bouton sélectionné par défaut
        this->ui->selection->setChecked(false);
        this->ui->eraser->setChecked(false);
        this->ui->pinceau->setChecked(false);
        this->ui->freeHand->setChecked(false);
        this->ui->polygone->setChecked(false);

            // Impossibilité de sélectionner un mode
        this->ui->selection->setCheckable(false);
        this->ui->eraser->setCheckable(false);

            // Impossibilité de sélectionner un mode
        this->ui->pinceau->setCheckable(false);
        this->ui->freeHand->setCheckable(false);
        this->ui->polygone->setCheckable(false);

        // Modes
        this->ui->drawing->updateImage(false);
        this->ui->drawing->updateMode(false);
        this->ui->drawing->clean();
    }

    // On a chargé une image
    else{
        // Mise à jour des différents paramètres
        this->ui->drawing->updateImage(true);
        this->isThereAnImage=true;
        //this->ui->drawing->draw(false);
        this->pictureToDisplay = QPixmap(this->pathImage);

        //QImage image(this->pathImage);
        //float width = /*this->ui->nothing->width();*/ image.width();
        //float height = /*this->ui->nothing->height();*/ image.height();
        //cout<<width<<height<<endl;
        //float ratio = width/height;
        //cout<<ratio<<endl;
        //this->ui->nothing->setGeometry(this->ui->nothing->x(),this->ui->nothing->y(),width,height);

        // Affichage
        this->ui->nothing->setPixmap(this->pictureToDisplay.scaled(this->ui->nothing->width(),
                                                                   this->ui->nothing->height(),
                                                                   Qt::KeepAspectRatio));

        this->ui->drawing->initDrawing(this->pathImage,
                                       this->ui->drawing->width(),
                                       this->ui->drawing->height());

        this->ui->apercu->setPixmap(this->ui->drawing->returnMask().scaled(this->ui->apercu->width(),
                                                                           this->ui->apercu->height(),
                                                                           Qt::KeepAspectRatio));

        // Chargement du chemin
        this->ui->drawing->setSource(this->pathImage);

        // Radio boutons
            // Définition de l'outil et du mode au début
        this->ui->selection->setChecked(true);
        this->ui->pinceau->setChecked(true);

            // Mise à disposition des outils
        this->ui->pinceau->setCheckable(true);
        this->ui->freeHand->setCheckable(true);
        this->ui->polygone->setCheckable(true);

            // Mise à disposition des modes
        this->ui->selection->setCheckable(true);
        this->ui->eraser->setCheckable(true);

        // Modes
        this->ui->drawing->updateImage(true);
        this->ui->drawing->updateMode(true);
        this->ui->drawing->brushMode(true);
        this->ui->drawing->clean();
    }
}

// SLOTS

/**
 * @brief app::on_imageBtn_clicked : permet de sélectionner un fichier au format image que l'on souhaite traiter
 */
void app::on_imageBtn_clicked()
{
    QString temp = QFileDialog::getOpenFileName(this,tr("Open File"),"C:\\",tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    this->pathImage=temp;
    this->checkUp();
}

/**
 * @brief app::on_clear_clicked : permet de supprimer le chemin vers l'image
 */
void app::on_clear_clicked()
{
    this->pathImage="";
    this->checkUp();
    this->ui->drawing->clean();
}

/**
 * @brief app::on_selection_clicked : permet de dire qu'on passe en mode dessin
 */
void app::on_selection_clicked()
{
    if(this->isThereAnImage){
        this->ui->drawing->clearToolModes();
        this->ui->drawing->updateMode(true);
        this->ui->drawing->changeColorTool(Qt::black);
    }
}

/**
 * @brief app::on_eraser_clicked : permet de dire qu'on passe en mode effaçage
 */
void app::on_eraser_clicked()
{
    if(this->isThereAnImage){
        this->ui->drawing->clearToolModes();
        this->ui->drawing->updateMode(true);
        this->ui->drawing->changeColorTool(Qt::white);
    }
}

/**
 * @brief app::on_pinceau_clicked : permet de dire qu'on passe en outil pinceau
 */
void app::on_pinceau_clicked(){
    if(this->isThereAnImage){
        this->ui->drawing->clearModes();
        this->ui->drawing->brushMode(true);
    }
}

/**
 * @brief app::on_taillePinceau_valueChanged : permet de récupérer la valeur du slider de transparence
 * @param value : entier entre 1 et 50
 */
void app::on_taillePinceau_valueChanged(int value)
{
    this->ui->drawing->changeSizeBrush(value);
}

/**
 * @brief app::on_transparence_valueChanged : permet de récupérer la valeur du slider de transparence
 * @param value : entier entre 0 et 100
 */
void app::on_transparence_valueChanged(int value)
{
    this->ui->drawing->changeTransparence(value);
}

/**
 * @brief app::on_cancel_clicked : permet de nettoyer tout ce qui a été dessiné
 */
void app::on_cancel_clicked()
{
    cout<<"Reset"<<endl;
    this->ui->drawing->clean();
}

/**
 * @brief app::on_freeHand_clicked : permet de dire qu'on passe en outil freeHand
 */
void app::on_freeHand_clicked()
{
    if(this->isThereAnImage){
        this->ui->drawing->clearModes();
        this->ui->drawing->freeMode(true);
    }
}

/**
 * @brief app::on_polygone_clicked : permet de dire qu'on passe en outil Polygone
 */
void app::on_polygone_clicked()
{
    if(this->isThereAnImage){
        this->ui->drawing->clearModes();
        this->ui->drawing->polygonMode(true);
    }
}

/**
 * @brief app::on_update_clicked : permet d'actualiser l'aperçu du masque généré
 */
void app::on_update_clicked()
{
    cout<<"MàJ"<<endl;
    this->ui->apercu->setPixmap(this->ui->drawing->returnMask().scaled(this->ui->apercu->width(),this->ui->apercu->height(),Qt::KeepAspectRatio));
}

/**
 * @brief app::on_colorButton_clicked : permet de sélectionner la couleur des tracés à afficher
 */
void app::on_colorButton_clicked()
{
    QColor color = QColorDialog::getColor(this->colorDisplay,nullptr,"Pouet");
    this->colorDisplay=color;
    this->ui->drawing->changeColorDisplay(color);
}


void app::on_valid_clicked()
{
    //QFile file("mask.png");
    //file.open(QIODevice::WriteOnly);
    cout<<"Enregistrement"<<endl;
    //this->ui->drawing->returnMask().save("mask.png","PNG");
}

