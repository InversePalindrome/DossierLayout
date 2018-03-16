/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Application.cpp
InversePalindrome.com
*/


#include "Application.hpp"
#include "AndroidUtility.hpp"

#include <QFile>
#include <QThread>
#include <QPixmap>
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>
#include <QSplashScreen>


Application::Application(int& argc, char** argv) :
    QApplication(argc, argv),
    translator(new QTranslator(this))
{
    Utility::setRotation(1);

    QObject::connect(this, &Application::applicationStateChanged, [this](Qt::ApplicationState state)
    {
        if(state == Qt::ApplicationInactive)
        {
            users.save(Utility::appPath() + "Users.xml");

            emit save();
        }
    });

    load(Utility::appPath() + "Settings.xml");
}

int Application::run()
{
    auto* splashWindow = new QDialog(nullptr, Qt::Window);
    splashWindow->setStyleSheet("background-color:white;");
    splashWindow->show();

    auto* splashScreen = new QSplashScreen(splashWindow, QPixmap(":/Resources/InversePalindromeLogo.jpg"));

    auto splashTime = 2u;

    splashScreen->show();
    thread()->sleep(splashTime);
    splashScreen->finish(createLoginDialog());

    delete splashWindow;

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

    auto connection = QObject::connect(this, &Application::save, mainWindow, &MainWindow::save);
    QObject::connect(mainWindow, &MainWindow::exit, [this, mainWindow, connection]
    {
        disconnect(connection);

        createLoginDialog();
        mainWindow->close();
    });
    QObject::connect(mainWindow, &MainWindow::openSettings, [this, mainWindow]
    {
        createSettingsDialog(mainWindow);
        mainWindow->close();
    });

    mainWindow->show();

    return mainWindow;
}

SettingsDialog* Application::createSettingsDialog(QWidget* parent)
{
    auto* settingsDialog = new SettingsDialog();

    const auto& name = parent->property("name").toString();
    const auto& user = parent->property("user").toString();

    QObject::connect(settingsDialog, &SettingsDialog::changeStyle, this, &Application::changeStyle);
    QObject::connect(settingsDialog, &SettingsDialog::changeLanguage, this, &Application::changeLanguage);
    QObject::connect(settingsDialog, &SettingsDialog::closeSettings, [this, settingsDialog, name, user]
    {
        settingsDialog->deleteLater();

        if(name == "Login")
        {
            createLoginDialog();
        }
        else if(name == "MainWindow")
        {
            createMainWindow(user);
        }
   });

   settingsDialog->show();

   return settingsDialog;
}

LoginDialog* Application::createLoginDialog()
{
    auto* loginDialog = new LoginDialog();

    QObject::connect(loginDialog, &LoginDialog::loginUser, [this, loginDialog](const QString& username, const QString& password)
    {
        if(!users.isLoginValid(User(username.toLower()), password))
        {
            QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Invalid username or password!"), QMessageBox::NoButton, loginDialog);
            errorMessage.exec();
        }
        else
        {
            createMainWindow(username);
            loginDialog->close();
        }
    });
    QObject::connect(loginDialog, &LoginDialog::openRegistration, [this, loginDialog]
    {
        createRegisterDialog();
        loginDialog->close();
    });
    QObject::connect(loginDialog, &LoginDialog::openSettings, [this, loginDialog]
    {
        createSettingsDialog(loginDialog);
        loginDialog->close();
    });

    loginDialog->show();

    return loginDialog;
}

RegisterDialog* Application::createRegisterDialog()
{
    auto* registerDialog = new RegisterDialog();

    QObject::connect(registerDialog, &RegisterDialog::registerUser, [this, registerDialog](const QString& user, const QString& password, const QString& rePassword)
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
        else if(password.compare(rePassword) != 0)
        {
            QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Passwords do not match!"), QMessageBox::NoButton, registerDialog);
            errorMessage.exec();
        }
        else if(!users.isRegistrationValid(User(user)))
        {
            QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Username is already taken!"), QMessageBox::NoButton, registerDialog);
            errorMessage.exec();
        }
        else
        {
            users.addUser(User(user), password);

            createLoginDialog();
            registerDialog->close();
        }
    });
    QObject::connect(registerDialog, &RegisterDialog::cancelRegistration, [this, registerDialog]
    {
        createLoginDialog();
        registerDialog->close();
    });

    registerDialog->show();

    return registerDialog;
}
