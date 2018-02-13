/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Application.cpp
InversePalindrome.com
*/


#include "Application.hpp"

#include <QLabel>
#include <QThread>
#include <QPixmap>
#include <QLineEdit>
#include <QFormLayout>
#include <QMessageBox>
#include <QPushButton>


Application::Application(int& argc, char** argv) :
    QApplication(argc, argv),
    users("Users.xml"),
    splashScreen(QPixmap(":/Resources/InversePalindromeLogo.jpg"))
{
    QObject::connect(&mainWindow, &MainWindow::exit, [this]
     {
         mainWindow.close();
         createLoginDialog();
     });
}

int Application::run()
{
    auto splashTime = 4u;

    splashScreen.show();
    thread()->sleep(splashTime);
    splashScreen.finish(createLoginDialog());

    return exec();
}

QDialog* Application::createLoginDialog()
{
    auto* loginDialog = new QDialog(nullptr, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
    loginDialog->setFixedSize(600, 480);
    loginDialog->setWindowTitle("Login");

    auto* logoLabel = new QLabel();

    QPixmap logoPicture(":/Resources/LoginTitle.png");
    logoPicture = logoPicture.scaledToHeight(240);

    logoLabel->setPixmap(logoPicture);

    auto* userEntry = new QLineEdit();

    auto* passwordEntry = new QLineEdit();
    passwordEntry->setEchoMode(QLineEdit::Password);

    auto* formLayout = new QFormLayout();
    formLayout->addRow("Username:", userEntry);
    formLayout->addRow("Password:", passwordEntry);
    auto* loginButton = new QPushButton("Login");
    auto* registerButton = new QPushButton("Register");

    auto* layoutButton = new QHBoxLayout();
    layoutButton->addWidget(loginButton);
    layoutButton->addWidget(registerButton);

    auto* layout = new QVBoxLayout(loginDialog);

    layout->addSpacing(15);
    layout->addWidget(logoLabel, 0, Qt::AlignCenter);
    layout->addSpacing(15);
    layout->addLayout(formLayout);
    layout->addLayout(layoutButton);

    QObject::connect(loginButton, &QPushButton::clicked, [this, loginDialog, userEntry, passwordEntry]
    {
         if(users.isLoginValid(userEntry->text(), passwordEntry->text()))
         {
             mainWindow.load(userEntry->text());

             delete loginDialog;
             mainWindow.show();
         }
    });
    QObject::connect(registerButton, &QPushButton::clicked, [this, loginDialog]
    {
        delete loginDialog;
        createRegisterDialog();
    });

    loginDialog->open();

    return loginDialog;
}

QDialog* Application::createRegisterDialog()
{
    auto* registerDialog = new QDialog(nullptr, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
    registerDialog->setFixedSize(600, 530);
    registerDialog->setWindowTitle("Register User");

    QPixmap loginIcon(":/Resources/Login.png");
    loginIcon = loginIcon.scaledToHeight(200);

    auto* loginLabel = new QLabel();
    loginLabel->setPixmap(loginIcon);

    auto* userEntry = new QLineEdit();

    auto* passwordEntry = new QLineEdit();
    passwordEntry->setEchoMode(QLineEdit::Password);

    auto* rePasswordEntry = new QLineEdit();
    rePasswordEntry->setEchoMode(QLineEdit::Password);

    auto* formLayout = new QFormLayout();
    formLayout->addRow("Username:", userEntry);
    formLayout->addRow("Password:", passwordEntry);
    formLayout->addRow("Re-Passsword:", rePasswordEntry);

    auto* registerButton = new QPushButton("Register");
    auto* cancelButton = new QPushButton("Cancel");

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(cancelButton);

    auto* layout = new QVBoxLayout(registerDialog);

    layout->addSpacing(15);
    layout->addWidget(loginLabel, 0, Qt::AlignCenter);
    layout->addSpacing(15);
    layout->addLayout(formLayout);
    layout->addLayout(buttonLayout);

    QObject::connect(registerButton, &QPushButton::clicked, [this, registerDialog, userEntry, passwordEntry, rePasswordEntry]
    {
        if(userEntry->text().isEmpty())
        {
            QMessageBox errorMessage(QMessageBox::Critical, "Error", "Username is empty!", QMessageBox::NoButton, registerDialog);
            errorMessage.exec();
        }
        else if(passwordEntry->text().isEmpty() || rePasswordEntry->text().isEmpty())
        {
            QMessageBox errorMessage(QMessageBox::Critical, "Error", "Password is empty!", QMessageBox::NoButton, registerDialog);
            errorMessage.exec();
        }
        else if(passwordEntry->text().compare(rePasswordEntry->text(), Qt::CaseSensitive) != 0)
        {
            QMessageBox errorMessage(QMessageBox::Critical, "Error", "Passwords do not match!", QMessageBox::NoButton, registerDialog);
            errorMessage.exec();
        }
        else if(!users.isRegistrationValid(userEntry->text()))
        {
            QMessageBox errorMessage(QMessageBox::Critical, "Error", "Username is already taken!", QMessageBox::NoButton, registerDialog);
            errorMessage.exec();
        }
        else
        {
            users.addUser(userEntry->text(), passwordEntry->text());

            delete registerDialog;
            createLoginDialog();
        }
    });
    QObject::connect(cancelButton, &QPushButton::clicked, [this, registerDialog]
    {
        delete registerDialog;
        createLoginDialog();
    });

    registerDialog->open();

    return registerDialog;
}
