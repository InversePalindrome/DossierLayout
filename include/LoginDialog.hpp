/*
Copyright (c) 2018 InversePalindrome
Inverbienes - LoginDialog.hpp
InversePalindrome.com
*/


#pragma once

#include "User.hpp"
#include "SimpleCrypt.hpp"
#include "RegisterDialog.hpp"

#include <QMap>
#include <QDialog>
#include <QString>


class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent);
    ~LoginDialog();

    void loadUser(const QString& fileName);

public slots:
     void addUser(const User& user);

private:
    RegisterDialog* registerDialog;

    SimpleCrypt crypto;

    QString fileName;
    QMap<QString, QString> users;

signals:
    void loginAccepted(const QString& user);
};
