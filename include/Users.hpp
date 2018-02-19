/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Users.hpp
InversePalindrome.com
*/


#pragma once

#include "SimpleCrypt.hpp"

#include <QMap>
#include <QString>


class Users
{
public:
    Users();
    ~Users();

    void load(const QString& fileName);
    void save(const QString& fileName);

    void addUser(const QString& user, const QString& password);

    bool isLoginValid(const QString& user, const QString& password);
    bool isRegistrationValid(const QString& user);

private:
    QMap<QString, QString> users;

    SimpleCrypt crypto;
};
