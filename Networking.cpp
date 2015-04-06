#include "Networking.h"

Networking::Networking(QObject* parent) :
    QTcpSocket(parent), width(-1),tempHeight(-1),area(NULL)
{
	initConnects();
}

Networking::Networking(const QHostAddress& address, quint16 port,
	QObject* parent) :
    QTcpSocket(parent), width(-1), tempHeight(-1), area(NULL)
{
	initConnects();
    connectToHost(address, port);
}

Networking::~Networking()
{
    if(area != NULL)
    {
        for(int row=0; row < height; row++)
            delete[] area[row];
        delete[] area;
    }
}


void Networking::initConnects() {
	connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this,
			SLOT(onError(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(readyRead()), this, SLOT(dataReceived()));
	connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SIGNAL(sockStateChanged(QAbstractSocket::SocketState)));
}

void Networking::sendData() {
	QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

//    stream << (quint8)MESSAGE_DATAFEEDBACK;
	for(int row=1; row < height-1; row++) {
		for(int col=0; col < width; col++) {
            stream << area[row][col];
		}
	}
	write(msg);
}

void Networking::connectSocket(const QHostAddress& addr, const quint16& port) {
	connectToHost(addr,port);
}

void Networking::disconnectSocket() {
	close();
}

void Networking::onDisconnected() {
}

void Networking::onError(QAbstractSocket::SocketError /*err*/) {
	throw std::runtime_error(errorString().toStdString());
}

void Networking::dataReceived() {
	QByteArray ba = readAll();
    partialData += ba;
    QDataStream stream(&partialData, QIODevice::ReadOnly);
	
	quint8 type;
	stream >> type;

    bool status=false;

	switch(type) {
		case Networking::MESSAGE_PROCESS:
            status=handleMessageProcess(stream);
			break;
		case Networking::MESSAGE_REMAP:
            status=handleMessageRemap(stream);
			break;
		case Networking::MESSAGE_WIDTH:
            status=handleMessageWidth(stream);
			break;
        default: // Invalid packet
            status=false;
            partialData.clear();
            break;
	}

    if(status) { // Packet handled
        partialData.clear();

        /*
        // send OK
        QByteArray outBa;
        QDataStream outStream(&outBa, QIODevice::WriteOnly);
        outStream << (quint8)MESSAGE_OK;
        write(outBa);
        */
    }
}

quint16 Networking::readValue(QDataStream& stream) {
	quint16 data;
    stream >> data; // in range [0,16 000]

    return data;
}
bool Networking::handleMessageProcess(QDataStream& stream) {
	if(width < 0) // Not initialized yet
        throw std::runtime_error("No width received!"); // Something better to do?

    if((size_t)partialData.size() < sizeof(quint8)+sizeof(quint16)*2*width)
        return false; // Wait for full packet

    // Must be in range [1000, 1300] (or [-1300,-1000] if reservoir)
    for(int rowId=0; rowId < 2; rowId++) {
		for(int col=0; col < width; col++) {
            area[(rowId==0)?0:height-1][col] = readValue(stream);
		}
    }

    emit iterate();
    return true;
}
bool Networking::handleMessageRemap(QDataStream& stream) {
	if(width < 0)
        throw std::runtime_error("No width received!");

    quint16 nHeight;
    stream >> nHeight;
    tempHeight = nHeight;

    if((size_t)partialData.size() < ((tempHeight-2)*width+1)*sizeof(quint16) + sizeof(quint8))
        return false; // Waiting for the full packet to be read.

    // Cleaning previous mem
    if(area != NULL) {
        for(int row=0; row < height; row++)
            delete[] area[row];
        delete[] area;
    }

    area = (quint16**) malloc(sizeof(quint16*) * nHeight);
    for(int row=0; row < nHeight; row++) {
        area[row] = (quint16*) malloc(sizeof(quint16) * width);
		
        if(row >= 1 && row < nHeight-1) {
            for(int col=0; col < width; col++)
                area[row][col] = readValue(stream);
        }
	}
	
    height = tempHeight;
    tempHeight=-1;

    emit remap(nHeight, area);
    return true;
}
bool Networking::handleMessageWidth(QDataStream& stream) {
    quint16 rWid;
    stream >> rWid;
    width=rWid;

	emit receivedWidth(width);
    return true;
}

