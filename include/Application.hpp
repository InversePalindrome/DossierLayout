/*
Copyright (c) 2018 InversePalindrome
Inverbienes - Application.hpp
InversePalindrome.com
*/


#pragma once

#include "MainWindow.hpp"
#include "LoginDialog.hpp"

#include <QApplication>
#include <QSplashScreen>


class Application : public QApplication
{
public:
    Application(int& argc, char** argv);

    int run();

private:
    QSplashScreen splashScreen;
    MainWindow mainWindow;
    LoginDialog loginDialog;
};
