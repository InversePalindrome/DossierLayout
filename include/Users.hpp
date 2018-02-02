/*
Copyright (c) 2018 InversePalindrome
DossierTable - Users.hpp
InversePalindrome.com
*/


#pragma once

#include "SimpleCrypt.hpp"

#include <QMap>
#include <QString>
#include <QObject>


class Users : public QObject
{
    Q_OBJECT

public:
    Users(const QString& fileName);
    ~Users();

    void loadUsers(const QString& fileName);

public slots:
    void addUser(const QString& user, const QString& password);

    void isLoginValid(const QString& user, const QString& password);
    void isRegistrationValid(const QString& user, const QString& password);

private:
    QString fileName;
    QMap<QString, QString> users;

    SimpleCrypt crypto;

signals:
    void userRegistered();
    void userAlreadyExists(const QString& user);

    void loginAccepted(const QString& user);
    void loginFailed();
};
