#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <QPainter>
#include <QButtonGroup>
#include "drawingwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class app; }
QT_END_NAMESPACE

using namespace std;

class app : public QMainWindow
{
    Q_OBJECT

public:
    app(QWidget *parent = nullptr);
    ~app();


private slots:
    // Méthodes
    void checkUp();
    // SLOTS
    void on_imageBtn_clicked();
    void on_clear_clicked();
    void on_selection_clicked();
    void on_pinceau_clicked();
    void on_taillePinceau_valueChanged(int value);
    void on_transparence_valueChanged(int value);
    void on_cancel_clicked();
    void on_freeHand_clicked();
    void on_polygone_clicked();
    void on_eraser_clicked();
    void on_update_clicked();
    void on_colorButton_clicked();
    void on_valid_clicked();

private:
    Ui::app *ui;
    // Booléens
    bool isThereAnImage = false;
    bool canDraw;
    bool canIDraw;
    // QPixmap
    QPixmap pictureToDisplay;
    // QColor
    QColor colorDisplay;
    // QString
    QString pathImage = "/Users/arthur/Downloads/IMG_7245.jpg";
    QButtonGroup* tools;
    QButtonGroup* toolMode;

};
#endif // APP_H
