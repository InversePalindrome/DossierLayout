/*
Copyright (c) 2018 InversePalindrome
DossierLayout - RegisterDialog.hpp
InversePalindrome.com
*/


#pragma once

#include <QDialog>


class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget* parent = nullptr);

signals:
    void registerUser(const QString& user, const QString& password, const QString& rePassword);
    void cancelRegistration();
};
