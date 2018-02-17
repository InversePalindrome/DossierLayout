/*
Copyright (c) 2018 InversePalindrome
DossierLayout - RegisterDialog.cpp
InversePalindrome.com
*/


#include "RegisterDialog.hpp"

#include <QLabel>
#include <QMenuBar>
#include <QLineEdit>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>


RegisterDialog::RegisterDialog(QWidget* parent) :
    QDialog(parent, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint)
{
    setFixedSize(600, 530);
    setWindowTitle(tr("Register - DossierLayout"));

    QPixmap loginIcon(":/Resources/Register.png");
    loginIcon = loginIcon.scaledToHeight(200);

    auto* loginLabel = new QLabel();
    loginLabel->setPixmap(loginIcon);

    auto* userEntry = new QLineEdit();

    auto* passwordEntry = new QLineEdit();
    passwordEntry->setEchoMode(QLineEdit::Password);

    auto* rePasswordEntry = new QLineEdit();
    rePasswordEntry->setEchoMode(QLineEdit::Password);

    auto* formLayout = new QFormLayout();
    formLayout->addRow(tr("Username:"), userEntry);
    formLayout->addRow(tr("Password:"), passwordEntry);
    formLayout->addRow(tr("Re-Passsword:"), rePasswordEntry);

    auto* registerButton = new QPushButton(tr("Register"));
    auto* cancelButton = new QPushButton(tr("Cancel"));

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(cancelButton);

    auto* layout = new QVBoxLayout(this);

    layout->addSpacing(15);
    layout->addWidget(loginLabel, 0, Qt::AlignCenter);
    layout->addSpacing(15);
    layout->addLayout(formLayout);
    layout->addLayout(buttonLayout);

    QObject::connect(registerButton, &QPushButton::clicked, [this, userEntry, passwordEntry, rePasswordEntry]
    {
        emit registerUser(userEntry->text(), passwordEntry->text(), rePasswordEntry->text());
    });
    QObject::connect(cancelButton, &QPushButton::clicked, [this]
    {
        emit cancelRegistration();
    });
}
