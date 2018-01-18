/*
Copyright (c) 2018 InversePalindrome
Inverbienes - Application.cpp
InversePalindrome.com
*/


#include "Application.hpp"

#include <QThread>


Application::Application(int& argc, char** argv) :
    QApplication(argc, argv),
    splashScreen(QPixmap(":/Resources/InversePalindromeLogo.png")),
    loginDialog(nullptr)
{
    QObject::connect(&loginDialog, &LoginDialog::ingresoAceptado,
        [this](const auto& usuario)
    {
        mainWindow.setUsuario(usuario);

        loginDialog.close();
        mainWindow.show();
    });
    QObject::connect(&mainWindow, &MainWindow::salir,
        [this]()
    {
        mainWindow.close();
        loginDialog.show();
    });
}

int Application::run()
{
    auto splashTime = 3u;

    splashScreen.show();
    thread()->sleep(splashTime);
    loginDialog.show();

    splashScreen.finish(&loginDialog);

    return exec();
}
