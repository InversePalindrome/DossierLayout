/*
Copyright (c) 2018 InversePalindrome
Inverbienes - Application.hpp
InversePalindrome.com
*/


#pragma once

#include "MainWindow.hpp"
#include "ArriendosList.hpp"

#include <QApplication>
#include <QSplashScreen>


class Application : public QApplication
{
public:
    Application(int& argc, char** argv);

    int run();

private:
    ArriendosList arriendos;
    MainWindow mainWindow;
    QSplashScreen splashScreen;
};
