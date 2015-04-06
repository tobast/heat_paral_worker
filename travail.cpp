#include "travail.h"

const long double deuxPi(2*3.14159265357989);

Travail::Travail(QObject *parent) :
    QObject(parent)
{
    D = .8;
    dt = 0.02;

    pret = 0;



}

Travail::~Travail(){
    if(pret){
    for(int i(0); i<NB_THREAD; ++i){
        delete thr[i];
    }
    delete[] t2;
    /*delete temp;
    delete vit;*/}
    //qDebug("Fin");
}

void Travail::largeur(int la){
    qDebug("setLongueur");
    l = la;
    pret = 1;

}

void Travail::redimensionner(int ha, long double **a){
    qDebug("Redimensionner");
    if(pret==0){return;}
    if(pret==2){
        for(int i(0);i<h;++i){
            //delete[] vit[i];
            delete[] t2[i];
            //delete[] temp[i];
        }
        //delete[] vit;
        //delete[] temp;
        delete[] t2;
    }
    h=ha;
    //vit = (long double**) malloc(sizeof(long double*)*h);
    t2 = (long double**) malloc(sizeof(long double*)*h);
    //temp = (long double**) malloc(sizeof(long double*)*h);
    for(int i(0);i<h;++i){
        //vit[i] = (long double*) malloc(sizeof(long double)*l);
        t2[i] = (long double*) malloc(sizeof(long double)*l);
        //temp[i] = i && i+1<h ? valeurs[i-1] : (long double*) malloc(sizeof(long double)*l);
        /*for(int j(0);j<l;++j){
            temp[i][j]=-1;
        }*/
    }
    temp = a;
    //delete[] valeurs;




    int x(l/NB_THREAD), y(0);
    for(int i(0); i<NB_THREAD; ++i){
        thr[i]= new Calcul(y,x,l,h,vit,temp,t2,D,dt,this);
        connect(thr[i], SIGNAL(fin()), this, SLOT(finTr()));
        y=x;
        x=(i+2)*l/NB_THREAD;
    }
    pret = 2;
}


void Travail::etape(){
    qDebug("Etape");
    if(pret<2){return;}
    /*delete[] temp[0];
    delete[] temp[h-1];
    temp[0] = bords[0];
    temp[h-1]=bords[1];*/
    f=0;
    qDebug() << (double) temp[50][50];
    for(int i(0); i<NB_THREAD; ++i){
        thr[i]->redemarrer();
    }
    //delete[] bords;
}


void Travail::arret(){
    act = false;
}




void Travail::finTr(){
    qDebug() << "Valeur de f : " << f;
    ++f;
    if(f==4){
        long double *a;
        for(int i(0);i<h;++i){
            a = t2[i];
            t2[i] = temp[i];
            temp[i] = a;
        }

        qDebug("Fin des calculs");
        emit fini();
    }
}
