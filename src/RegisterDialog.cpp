/*
Copyright (c) 2018 InversePalindrome
DossierTable - RegisterDialog.cpp
InversePalindrome.com
*/


#include "RegisterDialog.hpp"

#include <QFont>
#include <QLabel>
#include <QBoxLayout>
#include <QMessageBox>
#include <QPushButton>


RegisterDialog::RegisterDialog(QWidget* parent) :
    QDialog(parent),
    userEntry(new QLineEdit()),
    passwordEntry(new QLineEdit()),
    rePasswordEntry(new QLineEdit())
{
    setWindowTitle("Registrar Usuario");
    resize(600, 400);

    QFont labelFont("Arial", 12, QFont::Bold);
    QFont entryFont("Arial", 12);

    auto* userLabel = new QLabel("Usuario");
    userLabel->setFont(labelFont);
    userEntry->setFont(entryFont);

    auto* passwordLabel = new QLabel("Contraseña");
    passwordLabel->setFont(labelFont);
    passwordEntry->setFont(entryFont);
    passwordEntry->setEchoMode(QLineEdit::Password);

    auto* rePasswordLabel = new QLabel("Re-Contraseña");
    rePasswordLabel->setFont(labelFont);
    rePasswordEntry->setFont(entryFont);
    rePasswordEntry->setEchoMode(QLineEdit::Password);

    auto* registerButton = new QPushButton("Registrar");

    auto* layout = new QVBoxLayout();

    layout->addWidget(userLabel);
    layout->addWidget(userEntry);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEntry);
    layout->addWidget(rePasswordLabel);
    layout->addWidget(rePasswordEntry);
    layout->addWidget(registerButton);

    setLayout(layout);

   QObject::connect(registerButton, &QPushButton::clicked,
        [this]()
    {
        if(passwordEntry->text() == rePasswordEntry->text())
        {
            emit registerUser(userEntry->text(), passwordEntry->text());
        }
        else
        {
            QMessageBox errorMessage(QMessageBox::Critical, "Error", "Passwords do not match!", QMessageBox::NoButton, this);
            errorMessage.exec();
        }
    });
}

void RegisterDialog::closeEvent(QCloseEvent* event)
{
    userEntry->clear();
    passwordEntry->clear();
    rePasswordEntry->clear();

    QWidget::closeEvent(event);
}
