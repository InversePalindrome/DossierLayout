/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Application.hpp
InversePalindrome.com
*/


#pragma once

#include "Users.hpp"
#include "MainWindow.hpp"
#include "LoginDialog.hpp"
#include "SettingsDialog.hpp"
#include "RegisterDialog.hpp"

#include <QApplication>
#include <QTranslator>
#include <QSplashScreen>


class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int& argc, char** argv);

    int run();

private slots:
    void changeStyle(const QString& style);
    void changeLanguage(const QString& language);

private:
    Users users;

    QSplashScreen splashScreen;
    QTranslator* translator;

    void load(const QString& fileName);

    MainWindow* createMainWindow(const QString& user);
    SettingsDialog* createSettingsDialog();
    LoginDialog* createLoginDialog();
    RegisterDialog* createRegisterDialog();
};
