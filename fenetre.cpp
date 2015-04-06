#include "fenetre.h"

Fenetre::Fenetre(QWidget *parent) :
    QWidget(parent), dbg_areaImage(100, 100, QImage::Format_ARGB32)
{
    setupUi(this);
    /*sck = new Networking(this);
    t = new Travail(this);

    connect(t, SIGNAL(fini()), sck, SLOT(sendData()));
    connect(sck, SIGNAL(receivedWidth(int)), t, SLOT(largeur(int)));
    connect(sck, SIGNAL(remap(int, quint16**)), t, SLOT(redimensionner(int, quint16**)));
    connect(sck, SIGNAL(iterate()), t, SLOT(etape()));
    connect(sck, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(message(QAbstractSocket::SocketState)));
*/

    dbg_imgLabel->hide();
    dbg_imgLabel->setPixmap(QPixmap::fromImage(dbg_areaImage));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(connecter()));
    k=0;
    ti = new QTimer(this);
    connect(ti, SIGNAL(timeout()), this, SLOT(nouveau()));


    //connecter();
}

Fenetre::~Fenetre()
{
    //sck->disconnectSocket();
    for(int i(0); i<NB_WORKER; ++i){
        sck[i]->disconnectSocket();
    }
}

void Fenetre::connecter(){
    /*   Débugage   */
    /*

    t->largeur(500);
    quint16 **p = (quint16**) malloc(sizeof(quint16*)*200);
    for(int i(0);i<200;++i){
        p[i] = (quint16*) malloc(sizeof(quint16)*500);
        for(int j(0);j<500;++j){
            p[i][j]=-1;
        }
    }

    t->redimensionner(200, p);

    p = (quint16**) malloc(sizeof(quint16*)*2);
    for(int i(0);i<2;++i){
        p[i] = (quint16*) malloc(sizeof(quint16)*500);
        for(int j(0);j<500;++j){
            p[i][j]=-1;
        }
    }
    t->etape(p);


    return;

    */


    //ti->start(2000);
    nouveau();

    return;

    pushButton->setEnabled(false);
    //sck->connectSocket(QHostAddress(lineEdit->text()), (quint16) spinBox->value());
    for(int i(0); i<k; ++i){
        sck[i]->connectSocket(QHostAddress(lineEdit->text()), (quint16) spinBox->value());
    }
}

void Fenetre::message(QAbstractSocket::SocketState m){
    QString s;
    switch (m) {
    case 0:
        s = "Non connecté";
        break;
    case 1:
        s = "Je farfouille...";
        break;
    case 2:
        s = "Connection...";
        break;
    case 3:
        s = "Connecté";
        break;
    case 5:
        s = "Femeture";
        break;
    default:
        break;
    }
}

void Fenetre::dbg_displayArea()
{
    /// /!\ DEBUG ONLY
    if(groupDebug->isChecked()) {
        quint16** area = qobject_cast<Networking*>(sender())->getArea();
        for(int row=0; row < dbg_areaImage.height(); row++)
            for(int col=0; col < dbg_areaImage.width(); col++)
                dbg_areaImage.setPixel(col,row,dbg_colorOf(area[row][col]));
        dbg_imgLabel->setPixmap(QPixmap::fromImage(dbg_areaImage));
    }
}

void Fenetre::dbg_resizeImgWidth(int width)
{
    dbg_areaImage = QImage(width, 100, QImage::Format_ARGB32);
}

void Fenetre::dbg_resizeImg(int height)
{
    dbg_areaImage = QImage(dbg_areaImage.width(), height, QImage::Format_ARGB32);
    dbg_imgLabel->setPixmap(QPixmap::fromImage(dbg_areaImage));
//    adjustSize();
}


void Fenetre::nouveau(){
    qDebug("Un bébé en plus");
    sck.append(new Networking(this));
    t.append(new Travail(this));

    connect(t[k], SIGNAL(fini()), sck[k], SLOT(sendData()));
    connect(sck[k], SIGNAL(receivedWidth(int)), t[k], SLOT(largeur(int)));
    connect(sck[k], SIGNAL(remap(int, quint16**)), t[k], SLOT(redimensionner(int, quint16**)));
    connect(sck[k], SIGNAL(iterate()), t[k], SLOT(etape()));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(connecter()));
    connect(sck[k], SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(message(QAbstractSocket::SocketState)));
    connect(sck[k], SIGNAL(iterate()), this, SLOT(dbg_displayArea()));
    connect(sck[k], SIGNAL(receivedWidth(int)), this, SLOT(dbg_resizeImgWidth(int)));
    connect(sck[k], SIGNAL(remap(int,quint16**)), this, SLOT(dbg_resizeImg(int)));

    sck[k]->connectSocket(QHostAddress(lineEdit->text()), (quint16) spinBox->value());
    ++k;
}

QRgb Fenetre::dbg_colorOf(quint16 val)
{
    return QColor::fromHsv(
         std::max(0,std::min(240,(int)(240 - val * 240.0/16000.0))),
         0xFF,0xFF).rgb();
}
