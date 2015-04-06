/**
 *	NETWORKING :
 *  Handles the network in/out requests.
 **/

#include <QTcpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QDataStream>
#include <stdexcept>

class Networking : public QTcpSocket{
	Q_OBJECT

	public:
		enum MessageType {
			MESSAGE_PROCESS=1,
				/** Asks the slave to iterate, sends new reservoirs.
				 * STRUCTURE: 2*size chars, representing the two lines.
				 **/
			MESSAGE_REMAP=2,
				/** Informs the server of a change in map, and sends relevant info.
                 * STRUCTURE: (qint16) newAmplitude, (quint16)*(newEnd-newBeg+1)*size representing the whole new area.
				 **/
            MESSAGE_WIDTH=3,
                /** Informs the slave of the map width
                 * STRUCTURE: (quint16) new width
                 **/
            MESSAGE_OK=4,
                /** Does not carry any data, informs the peer that the packet was fully received. **/
            MESSAGE_DATAFEEDBACK=5
                /** Sends the processed data upstream
                 * STRUCTURE: (quint16)*(end-beg-1)*width representing the processed area.
                 **/
		};

		Networking(QObject* parent=0);
		Networking(const QHostAddress& address, quint16 port,
				QObject* parent=0);
        ~Networking();
        quint16** getArea() { return area; }

	public slots:
        void sendData();
		void connectSocket(const QHostAddress& addr, const quint16& port);
		void disconnectSocket();
	
	signals:
		void receivedWidth(int width);
        void iterate();
        void remap(int height, quint16**a);
		void sockStateChanged(QAbstractSocket::SocketState st);

	private slots:
		void onDisconnected();
		void onError(QAbstractSocket::SocketError err);
		void dataReceived();

	private: // meth
        void initConnects();
        bool handleMessageProcess(QDataStream& stream);
        bool handleMessageRemap(QDataStream& stream);
        bool handleMessageWidth(QDataStream& stream);

        quint16 readValue(QDataStream& stream);
		
	private:
		int width;
        int height,tempHeight;
        quint16** area;

        QByteArray partialData;
};

