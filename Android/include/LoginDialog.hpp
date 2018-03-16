/*
Copyright (c) 2018 InversePalindrome
DossierLayout - LoginDialog.hpp
InversePalindrome.com
*/


#pragma once

#include <QDialog>


class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);

signals:
    void loginUser(const QString& name, const QString& password);
    void openRegistration();
    void openSettings();
};
