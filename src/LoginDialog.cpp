/*
Copyright (c) 2018 InversePalindrome
DossierLayout - LoginDialog.cpp
InversePalindrome.com
*/


#include "LoginDialog.hpp"

#include <QLabel>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>


LoginDialog::LoginDialog(QWidget* parent) :
    QDialog(parent, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint),
    menuBar(new QMenuBar(this)),
    userEntry(new QLineEdit()),
    passwordEntry(new QLineEdit())
{
    setFixedSize(600, 480);
    setWindowTitle(tr("Login - DossierLayout"));

    auto* layout = new QVBoxLayout(this);
    layout->setMenuBar(menuBar);

    auto* language = menuBar->addMenu(tr("Language"));
    language->addAction(QIcon(":/Resources/English.png"), "English", [this]
    {
       emit changeLanguage("English");
    });
    language->addAction(QIcon(":/Resources/Spanish.png"), "Español", [this]
    {
       emit changeLanguage("Español");
    });

    auto* logoLabel = new QLabel();

    QPixmap logoPicture(":/Resources/DossierLayoutIcon.png");
    logoPicture = logoPicture.scaledToHeight(250);

    logoLabel->setPixmap(logoPicture);

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

    QObject::connect(loginButton, &QPushButton::clicked, [this]
    {
        emit loginUser(userEntry->text(), passwordEntry->text());
    });
    QObject::connect(registerButton, &QPushButton::clicked, [this]
    {
        emit registerUser();
    });
}

void LoginDialog::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {

    }
}

void LoginDialog::closeEvent(QCloseEvent* event)
{
    userEntry->clear();
    passwordEntry->clear();

    QDialog::closeEvent(event);
}
