#include "calcul.h"

const int Calcul::ADJACENCY[8][2]={ {0,1},{1,0},{0,-1},{-1,0}, {1,1},{-1,1},{1,-1},{-1,-1} };
const size_t Calcul::ADJACENCY_SIZE=8;

Calcul::Calcul(int i, int j, int la, int he, quint16 **vi, quint16 **p, quint16 **p2, double vD, double vdt, QObject *parent) :
    QThread(parent), x(i),y(j),h(he),l(la), vit(vi),temp(p),t2(p2),dt(vdt),D(vD)
{
    start();
    pause = false;
    while(!pause){
        msleep(10);
    }
}


QColor Calcul::couleur(quint16 t){
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
                if((temp[i][j] & 0x8000) == 0 && i && i+1!=h && j && j+1!=l){
                    /*
                    t2[i][j]=temp[i][j]+0.2*0.8*(std::abs(temp[i-1][j])+std::abs(temp[i+1][j])+std::abs(temp[i][j-1])+
                                              std::abs(temp[i][j+1])+(std::abs(temp[i-1][j-1])+std::abs(temp[i+1][j-1])+
                                              std::abs(temp[i+1][j+1])+std::abs(temp[i-1][j+1]))/1.41421356237-(4+4/1.41421356237)*temp[i][j]);
                    */
                    double delta_t=0;
                    for(size_t pos=0; pos < Calcul::ADJACENCY_SIZE; pos++)
                        delta_t += (double)((temp[i+Calcul::ADJACENCY[pos][0]][j+Calcul::ADJACENCY[pos][1]] & (~0x8000))-temp[i][j]) /
                                dist(Calcul::ADJACENCY[pos][0],ADJACENCY[pos][1]);
                    delta_t /= Calcul::ADJACENCY_SIZE;
                    t2[i][j] = std::max(0, std::min(16000, (int)(temp[i][j] + delta_t)));
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

double Calcul::dist(int distx, int disty)
{
    return sqrt(distx*distx + disty*disty);
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
