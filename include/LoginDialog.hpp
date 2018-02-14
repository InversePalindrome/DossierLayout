/*
Copyright (c) 2018 InversePalindrome
DossierLayout - LoginDialog.hpp
InversePalindrome.com
*/


#pragma once

#include <QEvent>
#include <QDialog>
#include <QMenuBar>
#include <QLineEdit>


class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);

private:
    QMenuBar* menuBar;
    QLineEdit* userEntry;
    QLineEdit* passwordEntry;

    virtual void changeEvent(QEvent* event) override;
    virtual void closeEvent(QCloseEvent* event) override;

signals:
    void loginUser(const QString& name, const QString& password);
    void registerUser();
    void changeLanguage(const QString& language);
};
