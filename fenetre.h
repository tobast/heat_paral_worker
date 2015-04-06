#ifndef FENETRE_H
#define FENETRE_H


#include <QTimer>
#include <QWidget>
#include <QImage>
#include <Networking.h>
#include <travail.h>
#include "ui_fenetre.h"

const int NB_WORKER = 1;

class Fenetre : public QWidget, private Ui::Fenetre
{
    Q_OBJECT

public:
    explicit Fenetre(QWidget *parent = 0);
    ~Fenetre();

private slots:
    void connecter();
    void message(QAbstractSocket::SocketState m);
    void dbg_displayArea();
    void dbg_resizeImgWidth(int width);
    void dbg_resizeImg(int height);

    void nouveau();

private:
    QRgb dbg_colorOf(long double val);
    //Networking sck;
    //Travail t;

    QTimer *ti;
    QVector<Networking*> sck;
    QVector<Travail *> t;
    int k;

    QImage dbg_areaImage;
};

#endif // FENETRE_H
