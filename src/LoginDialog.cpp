/*
Copyright (c) 2018 InversePalindrome
DossierTable - LoginDialog.cpp
InversePalindrome.com
*/


#include "LoginDialog.hpp"

#include <QFont>
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>


LoginDialog::LoginDialog() :
    userEntry(new QLineEdit()),
    passwordEntry(new QLineEdit())
{
    setFixedSize(600, 600);
    setWindowTitle("Login");
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);

    auto* logoLabel = new QLabel();

    QPixmap logoPicture(":/Resources/LoginTitle.png");
    logoPicture = logoPicture.scaledToHeight(240);

    logoLabel->setPixmap(logoPicture);

    QFont labelFont("Arial", 11, QFont::Bold);
    QFont entryFont("Arial", 11);

    auto* userLabel = new QLabel("User");
    userLabel->setFont(labelFont);
    userEntry->setFont(entryFont);

    auto* passwordLabel = new QLabel("Password");
    passwordLabel->setFont(labelFont);
    passwordEntry->setFont(entryFont);
    passwordEntry->setEchoMode(QLineEdit::Password);

    auto* loginButton = new QPushButton("Login");
    auto* registerButton = new QPushButton("Register");

    auto* layoutButton = new QHBoxLayout();
    layoutButton->addWidget(loginButton);
    layoutButton->addWidget(registerButton);

    auto* layout = new QVBoxLayout(this);

    layout->addSpacing(15);
    layout->addWidget(logoLabel, 0, Qt::AlignCenter);
    layout->addSpacing(15);
    layout->addWidget(userLabel);
    layout->addWidget(userEntry);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEntry);
    layout->addLayout(layoutButton);

    setLayout(layout);

    QObject::connect(loginButton, &QPushButton::clicked, [this]
    {
        emit loginUser(userEntry->text(), passwordEntry->text());
    });
    QObject::connect(registerButton, &QPushButton::clicked, [this] { emit registerUser(); });
}

void LoginDialog::closeEvent(QCloseEvent* event)
{
    userEntry->clear();
    passwordEntry->clear();

    QWidget::closeEvent(event);
}
