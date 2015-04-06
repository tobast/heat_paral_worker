#ifndef TRAVAIL_H
#define TRAVAIL_H

//#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QObject>
#include <calcul.h>


const int NB_THREAD = 4;

class Travail : public QObject
{
    Q_OBJECT
public:
    explicit Travail(QObject *parent = 0);
    ~Travail();

signals:
    void fini();

public slots:
    void finTr();
    void arret();

    void largeur(int la);
    void redimensionner(int ha, long double **a);
    void etape();

private:
    void run();
    bool act;
    int pret;
    int f;
    int x,y,l,h,k;
    long double **vit,**temp, **t2,dt,D;


    Calcul *thr[NB_THREAD];

};

#endif // TRAVAIL_H
