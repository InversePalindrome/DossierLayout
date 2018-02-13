/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Application.hpp
InversePalindrome.com
*/


#pragma once

#include "Users.hpp"
#include "MainWindow.hpp"

#include <QApplication>
#include <QDialog>
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

    QDialog* createLoginDialog();
    QDialog* createRegisterDialog();
};
