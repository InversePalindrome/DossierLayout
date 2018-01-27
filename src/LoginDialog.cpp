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


LoginDialog::LoginDialog(QWidget* parent) :
    QDialog(parent),
    userEntry(new QLineEdit()),
    passwordEntry(new QLineEdit())
{
    setFixedSize(600, 600);
    setWindowTitle("Iniciar Sesion");

    auto* logoLabel = new QLabel();

    QPixmap logoPicture(":/Resources/InverbienesLogo.png");
    logoPicture = logoPicture.scaledToHeight(200);

    logoLabel->setPixmap(logoPicture);

    QFont labelFont("Arial", 12, QFont::Bold);
    QFont entryFont("Arial", 12);

    auto* userLabel = new QLabel("Usuario");
    userLabel->setFont(labelFont);
    userEntry->setFont(entryFont);

    auto* passwordLabel = new QLabel("Contraseña");
    passwordLabel->setFont(labelFont);
    passwordEntry->setFont(entryFont);
    passwordEntry->setEchoMode(QLineEdit::Password);

    auto* loginButton = new QPushButton("Ingresar");
    auto* registerButton = new QPushButton("Registrar");

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

    QObject::connect(loginButton, &QPushButton::clicked,
       [this]()
    {
        emit loginUser(userEntry->text(), passwordEntry->text());
    });
    QObject::connect(registerButton, &QPushButton::clicked,
       [this]()
    {
        emit registerUser();
    });
}

void LoginDialog::closeEvent(QCloseEvent* event)
{
    userEntry->clear();
    passwordEntry->clear();

    QWidget::closeEvent(event);
}
