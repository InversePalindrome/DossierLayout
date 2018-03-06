/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Users.hpp
InversePalindrome.com
*/


#pragma once

#include "SimpleCrypt.hpp"

#include <QMap>
#include <QString>


class User : public QString
{
public:
    explicit User(const QString& user);
};

class Users
{
public:
    Users();
    ~Users();

    void load(const QString& fileName);
    void save(const QString& fileName);

    void addUser(const User& user, const QString& password);

    bool isLoginValid(const User& user, const QString& password);
    bool isRegistrationValid(const User& user);

private:
    QMap<User, QString> users;

    SimpleCrypt crypto;
};

bool operator<(const User& user1, const User& user2);
