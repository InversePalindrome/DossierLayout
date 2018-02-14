/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Application.cpp
InversePalindrome.com
*/


#include "Application.hpp"

#include <QThread>
#include <QPixmap>
#include <QMessageBox>


Application::Application(int& argc, char** argv) :
    QApplication(argc, argv),
    users("Users.xml"),
    splashScreen(QPixmap(":/Resources/InversePalindromeLogo.jpg")),
    mainWindow(new MainWindow()),
    loginDialog(new LoginDialog()),
    registerDialog(new RegisterDialog()),
    translator(new QTranslator(this))
{
    QObject::connect(mainWindow, &MainWindow::exit, [this]
    {
        mainWindow->close();
        loginDialog->open();
    });
    QObject::connect(loginDialog, &LoginDialog::loginUser, [this](const auto& user, const auto& password)
    {
        if(users.isLoginValid(user, password))
        {
            mainWindow->load(user);

            loginDialog->close();
            mainWindow->show();
        }
        else
        {
            QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Invalid username or password!"), QMessageBox::NoButton, loginDialog);
            errorMessage.exec();
        }
    });
    QObject::connect(loginDialog, &LoginDialog::registerUser, [this]
    {
        loginDialog->close();
        registerDialog->open();
    });
    QObject::connect(loginDialog, &LoginDialog::changeLanguage, [this](const auto& language)
    {
        if(language == "English")
        {
           removeTranslator(translator);
        }
        else if(language == "Español")
        {
           if(translator->load(":/Translations/spanish.qm"))
           {
               installTranslator(translator);
           }
        }
    });
    QObject::connect(registerDialog, &RegisterDialog::registerUser, [this](const auto& user, const auto& password, const auto& rePassword)
    {
        if(user.isEmpty())
        {
            QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Username is empty!"), QMessageBox::NoButton, registerDialog);
            errorMessage.exec();
        }
        else if(password.isEmpty() || rePassword.isEmpty())
        {
            QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Password is empty!"), QMessageBox::NoButton, registerDialog);
            errorMessage.exec();
        }
        else if(password.compare(rePassword, Qt::CaseSensitive) != 0)
        {
            QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Passwords do not match!"), QMessageBox::NoButton, registerDialog);
            errorMessage.exec();
        }
        else if(!users.isRegistrationValid(user))
        {
            QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Username is already taken!"), QMessageBox::NoButton, registerDialog);
            errorMessage.exec();
        }
        else
        {
            users.addUser(user, password);

            registerDialog->close();
            loginDialog->open();
        }
    });
    QObject::connect(registerDialog, &RegisterDialog::cancelRegistration, [this]
    {
        registerDialog->close();
        loginDialog->open();
    });
}

int Application::run()
{
    auto splashTime = 4u;

    splashScreen.show();
    thread()->sleep(splashTime);
    splashScreen.finish(loginDialog);

    loginDialog->show();

    return exec();
}

