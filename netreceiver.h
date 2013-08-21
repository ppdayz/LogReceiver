#ifndef NETRECEIVER_H
#define NETRECEIVER_H

#include <QTcpSocket>

#define NET_PACK_HEADER (int)(0xe39ff93e)

#define NET_HEADER_LEN (8)
#define MAX_NET_DATA_LEN (10*1024)
#define CACHE_DATA_LEN (MAX_NET_DATA_LEN*20)

class NetReceiver : public QTcpSocket
{
    Q_OBJECT
public:
    explicit NetReceiver(QObject *parent = 0);
    void connectToLogForwarder(const QString &host = "192.168.1.2", int port = 12345);
signals:
    void singleLogComing(const QString &log);
//    void netReceiverStateChanged(QAbstractSocket::SocketState state);
public slots:

private slots:
    void progressPandingData();
//    void slotNetError(QAbstractSocket::SocketError error);

private:
    char localBuff[CACHE_DATA_LEN];
    int localBuffLen;
    void dealLoaclBuff(char *buff);
    void dealLogBuffers(const QString &logBuff);
};

#endif // NETRECEIVER_H
