#include <iostream>
#include "User.h"

QByteArray User::encode(const QByteArray &arr) {
    QByteArray res(arr);
    if (!hasSessionKey_) {
        return res;
    }
    long long key = crypto_.getSessionKey();
    QString str(res);
    long long _long_size = sizeof(long long);
    long long ost = res.size() % _long_size;
    if (ost != 0) {
        str.resize(str.size() + _long_size - ost, '\0');
        res = str.toUtf8();
    }
//    std::cout << res.size() % _long_size << std::endl;
    auto *array = new long long[res.size() / _long_size];
    memcpy(array, res.data(), _long_size);
    for (int i = 0; i < res.size() / _long_size; ++i) {
        array[i] = array[i] ^ key;
    }
    memcpy(res.data(), array, _long_size);
    return res;
}

User::User(QTcpSocket *socket) : hasSessionKey_(false), socket_(socket) {
    contacts_ = QSharedPointer<QMap<QString, QPair<unsigned long long, int>>>(
            new QMap<QString, QPair<unsigned long long, int>>);
}

void User::setKeyFromAlice(const long long &key) {
    crypto_.setKeyFromAlice(key);
}

bool User::hasSessionKey() const {
    return hasSessionKey_;
}

void User::setHasSessionKey(bool hasSessionKey) {
    User::hasSessionKey_ = hasSessionKey;
}

QByteArray User::decode(const QByteArray &arr) const {
    if (!hasSessionKey_) {
        return arr;
    }
    QByteArray res(arr);
    long long key = crypto_.getSessionKey();
//    std::cout << key << std::endl;
    long long long_size = sizeof(long long);
//    std::cout << res.size() % long_size << std::endl;
    auto *array = new long long[arr.size() / long_size];
    memcpy(array, res, long_size);
    for (int i = 0; i < arr.size() / long_size; ++i) {
        array[i] = array[i] ^ key;
    }
    memcpy(res.data(), array, long_size);
    QString str(res);
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '\0') {
            str.resize(i);
            break;
        }
    }
//    std::cout << str.simplified().toStdString() << std::endl;
    return str.toUtf8();
}

long long User::getKeyToAlice() const {
    return crypto_.getKeyToAlice();
}

unsigned int User::getUserIdInDataBase() const {
    return UserIDInDataBase_;
}

void User::setUserIdInDataBase(unsigned int userIdInDataBase) {
    UserIDInDataBase_ = userIdInDataBase;
}

void User::addChannel(const QString &channel, const unsigned long long &id, const int &roots) {
    auto map = contacts_.get();
    (*map)[channel] = QPair<unsigned long long, int>(id, roots);
}

QSharedPointer<QMap<QString, QPair<unsigned long long, int>>> &User::getContacts() {
    return contacts_;
}

QPair<unsigned long long, int> User::getIDRootsFromChannel(const QString &channelName) {
    if (contacts_->contains(channelName)) {
        return (*contacts_)[channelName];
    }
    return {0, 0};
}

const QString &User::getUserName() const {
    return userName_;
}

void User::setUserName(const QString &userName) {
    userName_ = userName;
}

void User::writeJSON(const QJsonDocument &doc) {
    socket_->write(encode(doc.toJson(QJsonDocument::Indented)));
}

