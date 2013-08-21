#include "netreceiver.h"
#include <QStringList>


struct _net_package
{
    int header;		/* static value : NET_PACK_HEADER */
    int data_len;	/* data buf len */
    char data[];
};

typedef struct _net_package NET_PACKAGE;

NetReceiver::NetReceiver(QObject *parent) :
    QTcpSocket(parent), localBuffLen(0)
{
    memset(localBuff, 0, sizeof(localBuff));
    connect(this, SIGNAL(readyRead()), this, SLOT(progressPandingData()));
//    connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SIGNAL()
}

void NetReceiver::connectToLogForwarder(const QString &host, int port)
{
    this->connectToHost(host, port);
}

void NetReceiver::progressPandingData()
{
    QDataStream in(this);

    char readBuff[MAX_NET_DATA_LEN];
    memset(readBuff, 0, MAX_NET_DATA_LEN);

    int readBuffLen = in.readRawData(readBuff, sizeof(readBuff));

    if(localBuffLen + readBuffLen > (int) sizeof(localBuff)){
        //TODO:  need to deal?
        qDebug() << "local buff is full , localBuffLen + readBuffLen  = " ;
        return;
    }

    memcpy(localBuff+localBuffLen, readBuff, readBuffLen);

    localBuffLen += readBuffLen;

    dealLoaclBuff(localBuff);
}


void NetReceiver::dealLoaclBuff(char *buff)
{
    NET_PACKAGE *ppack = NULL;
    ppack = (NET_PACKAGE *)buff;

    if (ppack->header == NET_PACK_HEADER ) {
        int dataLen = ppack->data_len;

        if(dataLen +NET_HEADER_LEN <= localBuffLen){
            QByteArray ba(ppack->data, ppack->data_len);
            QString line = QString::fromUtf8(ba.data());
            dealLogBuffers(line);

            memmove(localBuff, localBuff+dataLen +NET_HEADER_LEN, localBuffLen-(dataLen +NET_HEADER_LEN));

            localBuffLen -= (dataLen +NET_HEADER_LEN);
            dealLoaclBuff(localBuff);
        }
    }else{
        qDebug() << "not the head of packet!!!!!!!!!!";
        memset((void *)&localBuff, 0, sizeof(localBuff));
        localBuffLen = 0;
    }
}

void NetReceiver::dealLogBuffers(const QString &logBuff)
{
    QStringList logs = logBuff.split("#@#");

    if(logs.size() == 2){
        emit singleLogComing(logs.at(1));
    }else{
        for(int i=1; i<logs.size(); ++i){
            emit singleLogComing(logs.at(i));
        }
    }
}
