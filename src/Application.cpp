/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Application.cpp
InversePalindrome.com
*/


#include "Application.hpp"

#include <QThread>
#include <QMessageBox>


Application::Application(int& argc, char** argv) :
    QApplication(argc, argv),
    users("Usuarios.xml"),
    splashScreen(QPixmap(":/Resources/InversePalindromeLogo.jpg"))
{
    QObject::connect(&users, &Users::userRegistered, [this]
    {
        registerDialog.close();
        loginDialog.show();
    });
    QObject::connect(&users, &Users::userAlreadyExists, [this](const auto& user)
    {
        QMessageBox errorMessage(QMessageBox::Critical, "Error", "User " + user + " already exists!", QMessageBox::NoButton, &loginDialog);
        errorMessage.exec();
    });
    QObject::connect(&users, &Users::loginAccepted, [this](const auto& user)
    {
        loginDialog.close();

        mainWindow.load(user);
        mainWindow.show();
    });
    QObject::connect(&users, &Users::loginFailed, [this]
    {
        QMessageBox errorMessage(QMessageBox::Critical, "Error", "Invalid Username or Password", QMessageBox::NoButton, &loginDialog);
        errorMessage.exec();
    });
    QObject::connect(&mainWindow, &MainWindow::exit, [this]
    {
        mainWindow.close();
        loginDialog.show();
    });
    QObject::connect(&registerDialog, &RegisterDialog::registerUser, &users, &Users::isRegistrationValid);
    QObject::connect(&registerDialog, &RegisterDialog::windowClosed, [this] { loginDialog.show(); });
    QObject::connect(&loginDialog, &LoginDialog::loginUser, &users, &Users::isLoginValid);
    QObject::connect(&loginDialog, &LoginDialog::registerUser, [this]
    {
        loginDialog.close();
        registerDialog.show();
    });
}

int Application::run()
{
    auto splashTime = 4u;

    splashScreen.show();
    thread()->sleep(splashTime);
    loginDialog.show();

    splashScreen.finish(&loginDialog);

    return exec();
}
