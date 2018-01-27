/*
Copyright (c) 2018 InversePalindrome
DossierTable - RegisterDialog.hpp
InversePalindrome.com
*/


#pragma once

#include <QDialog>
#include <QLineEdit>


class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
   explicit RegisterDialog(QWidget* parent);

private:
    QLineEdit* userEntry;
    QLineEdit* passwordEntry;
    QLineEdit* rePasswordEntry;

    virtual void closeEvent(QCloseEvent* event) override;

signals:
    void registerUser(const QString& user, const QString& password);
};
