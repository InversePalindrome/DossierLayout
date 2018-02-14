/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Application.hpp
InversePalindrome.com
*/


#pragma once

#include "Users.hpp"
#include "MainWindow.hpp"
#include "LoginDialog.hpp"
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

private:
    Users users;

    QSplashScreen splashScreen;

    MainWindow* mainWindow;
    LoginDialog* loginDialog;
    RegisterDialog* registerDialog;

    QTranslator* translator;
};
