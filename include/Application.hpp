/*
Copyright (c) 2018 InversePalindrome
DossierTable - Application.hpp
InversePalindrome.com
*/


#pragma once

#include "Users.hpp"
#include "MainWindow.hpp"
#include "LoginDialog.hpp"
#include "RegisterDialog.hpp"

#include <QApplication>
#include <QSplashScreen>


class Application : public QApplication
{
public:
    Application(int& argc, char** argv);

    int run();

private:
    Users users;

    QSplashScreen splashScreen;
    MainWindow mainWindow;
    LoginDialog loginDialog;
    RegisterDialog registerDialog;
};
