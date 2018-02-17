/*
Copyright (c) 2018 InversePalindrome
DossierLayout - LoginDialog.cpp
InversePalindrome.com
*/


#include "LoginDialog.hpp"

#include <QLabel>
#include <QMenuBar>
#include <QLineEdit>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>


LoginDialog::LoginDialog(QWidget* parent) :
    QDialog(parent, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint)
{
    setFixedSize(600, 480);
    setWindowTitle(tr("Login - DossierLayout"));

    auto* menuBar = new QMenuBar(this);
    auto* settings = menuBar->addAction(tr("Settings"));

    auto* layout = new QVBoxLayout(this);
    layout->setMenuBar(menuBar);

    auto* logoLabel = new QLabel(this);

    QPixmap logoPicture(":/Resources/DossierLayoutIcon.png");
    logoPicture = logoPicture.scaledToHeight(250);

    logoLabel->setPixmap(logoPicture);

    auto* userEntry = new QLineEdit(this);

    auto* passwordEntry = new QLineEdit(this);
    passwordEntry->setEchoMode(QLineEdit::Password);

    auto* formLayout = new QFormLayout();
    formLayout->addRow(tr("Username:"), userEntry);
    formLayout->addRow(tr("Password:"), passwordEntry);
    auto* loginButton = new QPushButton(tr("Login"));
    auto* registerButton = new QPushButton(tr("Register"));

    auto* layoutButton = new QHBoxLayout();
    layoutButton->addWidget(loginButton);
    layoutButton->addWidget(registerButton);

    layout->addSpacing(15);
    layout->addWidget(logoLabel, 0, Qt::AlignCenter);
    layout->addSpacing(15);
    layout->addLayout(formLayout);
    layout->addLayout(layoutButton);

    QObject::connect(loginButton, &QPushButton::clicked, [this, userEntry, passwordEntry]
    {
        emit loginUser(userEntry->text(), passwordEntry->text());
    });
    QObject::connect(registerButton, &QPushButton::clicked, [this]
    {
        emit registerUser();
    });
    QObject::connect(settings, &QAction::triggered, [this]
    {
        emit openSettings();
    });
}
