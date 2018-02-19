/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Application.cpp
InversePalindrome.com
*/


#include "Application.hpp"

#include <QFile>
#include <QThread>
#include <QPixmap>
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>


Application::Application(int& argc, char** argv) :
    QApplication(argc, argv),
    splashScreen(QPixmap(":/Resources/InversePalindromeLogo.jpg")),
    translator(new QTranslator(this))
{
    load("Settings.xml");
}

int Application::run()
{
    auto splashTime = 4u;

    splashScreen.show();
    thread()->sleep(splashTime);
    splashScreen.finish(createLoginDialog());

    return exec();
}

void Application::changeStyle(const QString& style)
{
    QFile file("://" + style + ".qss");
    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream stream(&file);

    setStyleSheet(stream.readAll());
}

void Application::changeLanguage(const QString& language)
{
    if(language == "English")
    {
       removeTranslator(translator);
    }
    else
    {
       if(translator->load(":/Translations/" + language + ".qm"))
       {
           installTranslator(translator);
       }
    }
}

void Application::load(const QString& fileName)
{
    QDomDocument doc;
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    else
    {
        if(!doc.setContent(&file))
        {
            return;
        }

        file.close();
    }

    auto settingsElement = doc.firstChildElement("Settings");

    changeStyle(settingsElement.firstChildElement("Style").firstChild().nodeValue());
    changeLanguage(settingsElement.firstChildElement("Language").firstChild().nodeValue());
}

MainWindow* Application::createMainWindow(const QString& user)
{
    auto* mainWindow = new MainWindow(user);

    QObject::connect(mainWindow, &MainWindow::exit, [this, mainWindow]
    {
        mainWindow->deleteLater();
        createLoginDialog();
    });

    mainWindow->show();

    return mainWindow;
}

SettingsDialog* Application::createSettingsDialog()
{
    auto* settingsDialog = new SettingsDialog();

    QObject::connect(settingsDialog, &SettingsDialog::changeStyle, this, &Application::changeStyle);
    QObject::connect(settingsDialog, &SettingsDialog::changeLanguage, this, &Application::changeLanguage);
    QObject::connect(settingsDialog, &SettingsDialog::done, [this, settingsDialog]
    {
        settingsDialog->deleteLater();
        createLoginDialog();
    });

    settingsDialog->show();

    return settingsDialog;
}

LoginDialog* Application::createLoginDialog()
{
    auto* loginDialog = new LoginDialog();

    QObject::connect(loginDialog, &LoginDialog::loginUser, [this, loginDialog](const auto& user, const auto& password)
    {
        if(users.isLoginValid(user, password))
        {
            loginDialog->deleteLater();
            createMainWindow(user);
        }
        else
        {
            QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Invalid username or password!"), QMessageBox::NoButton, loginDialog);
            errorMessage.exec();
        }
    });
    QObject::connect(loginDialog, &LoginDialog::registerUser, [this, loginDialog]
    {
        loginDialog->deleteLater();
        createRegisterDialog();
    });
    QObject::connect(loginDialog, &LoginDialog::openSettings, [this, loginDialog]
    {
        loginDialog->deleteLater();
        createSettingsDialog();
    });

    loginDialog->show();

    return loginDialog;
}

RegisterDialog* Application::createRegisterDialog()
{
    auto* registerDialog = new RegisterDialog();

    QObject::connect(registerDialog, &RegisterDialog::registerUser, [this, registerDialog](const auto& user, const auto& password, const auto& rePassword)
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

            registerDialog->deleteLater();
            createLoginDialog();
        }
    });
    QObject::connect(registerDialog, &RegisterDialog::cancelRegistration, [this, registerDialog]
    {
        registerDialog->deleteLater();
        createLoginDialog();
    });

    registerDialog->show();

    return registerDialog;
}
