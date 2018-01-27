/*
Copyright (c) 2018 InversePalindrome
DossierTable - Users.cpp
InversePalindrome.com
*/


#include "Users.hpp"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>


Users::Users(const QString& fileName) :
    crypto(0x0c2ad4a4acb9f023)
{
   loadUsers(fileName);
}

Users::~Users()
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto usersElement = doc.createElement("Users");

    for(auto usuarioItr = users.constBegin(); usuarioItr != users.constEnd(); ++usuarioItr)
    {
        auto userElement = doc.createElement("User");

        userElement.setAttribute("name", usuarioItr.key());
        userElement.setAttribute("password", usuarioItr.value());

        usersElement.appendChild(userElement);
    }

    doc.appendChild(usersElement);

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       return;
    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

void Users::loadUsers(const QString& fileName)
{
    this->fileName = fileName;

    QDomDocument doc;
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    else
    {
        if(!doc.setContent(&file))
        {
            return;
        }

        file.close();
    }

    auto usersElement = doc.firstChildElement("Users");

    auto usersList = usersElement.elementsByTagName("User");

    for(int i = 0; i < usersList.count(); ++i)
    {
        auto user = usersList.at(i);

        if(user.isElement())
        {
            auto userElement = user.toElement();

            users.insert(userElement.attribute("name"), userElement.attribute("password"));
        }
    }
}

void Users::addUser(const QString& user, const QString& password)
{
    users.insert(user, crypto.encryptToString(password));

    QDir().mkdir(user);

    emit userAdded();
}

void Users::isLoginValid(const QString& user, const QString& password)
{
    if(users.count(user) && password == crypto.decryptToString(users.value(user)))
    {
        emit loginAccepted(true, user);
    }
    else
    {
        emit loginAccepted(false, user);
    }
}

void Users::isRegistrationValid(const QString& user, const QString& password)
{
    if(!users.count(user) && !user.isEmpty() && !password.isEmpty())
    {
        addUser(user, password);
    }
}
