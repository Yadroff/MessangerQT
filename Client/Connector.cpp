#include "Connector.h"

const QString SERVER_ANS = "SERVER SEND";

#include <iostream>

Connector::Connector(QObject *parent)
        : QObject{parent} {
    socket_ = new QUdpSocket(this);
    socket_->bind(UDP_SERVER_PORT, QUdpSocket::ShareAddress);
    connect(socket_, SIGNAL(readyRead()), this, SLOT(readData()));
    runFlag_ = true;
//    start_ = QTime::currentTime();
//    connect_ = false;
}

Connector::~Connector() {
    delete socket_;
}

void Connector::setRun(bool run) {
    runFlag_ = run;
}

void Connector::readData() {
    QString ans;
    QByteArray datagram;
    QHostAddress address;
    if (socket_->hasPendingDatagrams() and runFlag_) {
        datagram.resize(socket_->pendingDatagramSize());
        socket_->readDatagram(datagram.data(), datagram.size(), &address, &serverPort_);
        std::cout << datagram.data() << std::endl;
        if (QString(datagram.data()) == SERVER_ANS) {
            ans = "CONNECT" + SEPARATOR + "TO" + SEPARATOR + "SERVER" + SEPARATOR +
                  address.toString().split(":").last();
            std::cout << "OK: " << ans.toStdString() << std::endl;
//            connect_ = true;
            runFlag_ = false;
            emit finished(ans);
        }
    }
}

void Connector::run() {
    if (!socket_->waitForReadyRead(LOGIN_TIME)){
        emit finished(CONNECT_ERROR_MESSAGE);
    }
}
