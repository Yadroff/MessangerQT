#ifndef SERVER_USER_H
#define SERVER_USER_H

#include <QObject>

#include "Cryptograph.h"

class User : public QObject {
Q_OBJECT
public:
    User();

    void setKeyFromAlice(const long long &key);

    QByteArray encode(const QByteArray &arr);

    [[nodiscard]] bool hasSessionKey() const;

    void setHasSessionKey(bool hasSessionKey);

    [[nodiscard]] QByteArray decode(const QByteArray &arr) const;

    [[nodiscard]] long long getKeyToAlice() const;

    unsigned int getUserIdInDataBase() const;

    void setUserIdInDataBase(unsigned int userIdInDataBase);

private:
    bool hasSessionKey_;
    CryptographBob crypto_;
    QString userName_;
    unsigned int UserIDInDataBase_ = 0;
};


#endif //SERVER_USER_H