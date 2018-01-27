/*
Copyright (c) 2018 InversePalindrome
DossierTable - Application.cpp
InversePalindrome.com
*/


#include "Application.hpp"

#include <QThread>
#include <QMessageBox>


Application::Application(int& argc, char** argv) :
    QApplication(argc, argv),
    users("Usuarios.xml"),
    splashScreen(QPixmap(":/Resources/InversePalindromeLogo.png")),
    loginDialog(&mainWindow),
    registerDialog(&mainWindow)
{
    QObject::connect(&users, &Users::userAdded, [this]()
    {
        registerDialog.close();
        loginDialog.show();
    });
    QObject::connect(&users, &Users::loginAccepted,
        [this](auto valid, const auto& user)
    {
        if(valid)
        {
           mainWindow.loadUser(user);

           loginDialog.close();
           mainWindow.show();
        }
        else
        {
            QMessageBox errorMessage(QMessageBox::Critical, "Error", "Invalid Username or Password", QMessageBox::NoButton, &loginDialog);
            errorMessage.exec();
        }
    });
    QObject::connect(&mainWindow, &MainWindow::exit,
        [this]()
    {
        mainWindow.close();
        loginDialog.show();
    });
    QObject::connect(&registerDialog, &RegisterDialog::registerUser, &users, &Users::isRegistrationValid);
    QObject::connect(&loginDialog, &LoginDialog::loginUser, &users, &Users::isLoginValid);
    QObject::connect(&loginDialog, &LoginDialog::registerUser, [this]()
    {
        loginDialog.close();
        registerDialog.show();
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
