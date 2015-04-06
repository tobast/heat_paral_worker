#include "calcul.h"

Calcul::Calcul(int i, int j, int la, int he, long double **vi, long double **p, long double **p2, long double vD, long double vdt, QObject *parent) : x(i),y(j),l(la), h(he),vit(vi),t2(p2),temp(p),D(vD),dt(vdt),
    QThread(parent)
{
    start();
    pause = false;
    while(!pause){
        msleep(10);
    }
}


QColor Calcul::couleur(long double t){
    return QColor::fromHsv(std::max(0,std::min(std::abs((int) t)-DEC,359)), 255,255);
}

void Calcul::run(){
    act = true;
    while(act){
        pause = true;
        //qDebug("Mise en pause");
        while(pause && act){
            msleep(20);
        }
        //qDebug("c'est parti !");
        /*    Ca, c'est la propagation (mais pas avec des tableaux 2D !)
        for(int i(x);i<y;++i){
            for(int j(0);j<h;++j){
                if(temp[i+j*l]>=0){
                    vit[i+j*l]+=-vit[i+j*l]*dt*.0+dt*D*(std::abs(temp[i+j*l-1])+std::abs(temp[i+j*l+1])+std::abs(temp[i+j*l-l])+std::abs(temp[i+j*l+l])+(std::abs(temp[i+j*l-1-l])+std::abs(temp[i+j*l+1+l])+std::abs(temp[i+j*l-l+1])+std::abs(temp[i+j*l+l-1]))/1.41421356237-(4+4/1.41421356237)*temp[i+j*l]);
                }
                else{
                    vit[i+j*l]=0;
                }
            }
        }
        for(int i(x);i<y;++i){
            for(int j(0);j<h;++j){
                t2[i+j*l]=temp[i+j*l]+vit[i+j*l]*dt;
            }
        }*/

        if(act){
        for(int j(x);j<y;++j){
            for(int i(0);i<h;++i){
                if(temp[i][j]>=0 && i && i+1!=h && j && j+1!=l){
                    t2[i][j]=temp[i][j]+0.2*0.8*(std::abs(temp[i-1][j])+std::abs(temp[i+1][j])+std::abs(temp[i][j-1])+
                                              std::abs(temp[i][j+1])+(std::abs(temp[i-1][j-1])+std::abs(temp[i+1][j-1])+
                                              std::abs(temp[i+1][j+1])+std::abs(temp[i-1][j+1]))/1.41421356237-(4+4/1.41421356237)*temp[i][j]);
                }
                else{
                    t2[i][j]=temp[i][j];
                }
            }
        }
        if(act){
            emit fin();
        }
        }
    }
    act = true;
    //emit fin();
}

void Calcul::redemarrer(){
    //qDebug("demande redemarrage");
    pause = false;
}

Calcul::~Calcul(){
    if(act){
        act = false;
        while(!act){msleep(10);}
    }
}
