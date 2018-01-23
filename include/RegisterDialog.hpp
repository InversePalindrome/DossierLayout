/*
Copyright (c) 2018 InversePalindrome
Inverbienes - RegisterDialog.hpp
InversePalindrome.com
*/


#pragma once

#include "User.hpp"

#include <QDialog>
#include <QLineEdit>


class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
   explicit  RegisterDialog(QWidget* parent);

private:
    QLineEdit* userEntry;
    QLineEdit* passwordEntry;
    QLineEdit* rePasswordEntry;

    virtual void closeEvent(QCloseEvent*) override;

signals:
    void registerUser(const User& user);
};
