/*
Copyright (c) 2018 InversePalindrome
Inverbienes - Application.cpp
InversePalindrome.com
*/


#include "Application.hpp"

#include <QThread>


Application::Application(int& argc, char** argv) :
     QApplication(argc, argv),
     arriendos(""),
     mainWindow(arriendos, 2048, 1536),
     splashScreen(QPixmap(":/Resources/InversePalindromeLogo.png"))
{
}

int Application::run()
{
    auto splashTime = 3u;

    splashScreen.show();
    thread()->sleep(splashTime);
    mainWindow.show();

    splashScreen.finish(&mainWindow);

    return exec();
}
