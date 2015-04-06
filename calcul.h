#ifndef CALCUL_H
#define CALCUL_H

#include <QDebug>

#include <QThread>
#include <QColor>
#include <cmath>

const int DEC = 5000;

class Calcul : public QThread
{
    Q_OBJECT
public:
    explicit Calcul(int i, int j, int la, int he, long double **vi, long double **p, long double **p2, long double vD, long double vdt, QObject *parent = 0);
    ~Calcul();
    void redemarrer();
    //void arret();

signals:
    void fin();

public slots:

private:
    void run();
    int x,y,h,l;
    long double **vit,**temp, **t2,dt,D;
    bool act, pause;
    QColor cc;

    QColor couleur(long double t);
};

#endif // CALCUL_H
