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
    explicit Calcul(int i, int j, int la, int he, quint16 **vi, quint16 **p, quint16 **p2, double vD, double vdt, QObject *parent = 0);
    ~Calcul();
    void redemarrer();
    //void arret();

signals:
    void fin();

public slots:

private:
    void run();
    double dist(int distx, int disty);

    int x,y,h,l;
    quint16 **vit,**temp, **t2;
    double dt,D;
    bool act, pause;
    QColor cc;

    QColor couleur(quint16 t);

    static const int ADJACENCY[8][2];
    static const size_t ADJACENCY_SIZE;
};

#endif // CALCUL_H
