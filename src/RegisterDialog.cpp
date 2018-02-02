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


RegisterDialog::RegisterDialog() :
    userEntry(new QLineEdit()),
    passwordEntry(new QLineEdit()),
    rePasswordEntry(new QLineEdit())
{
    setWindowTitle("Register User");
    resize(600, 400);

    QFont labelFont("Arial", 12, QFont::Bold);
    QFont entryFont("Arial", 12);

    auto* userLabel = new QLabel("User");
    userLabel->setFont(labelFont);
    userEntry->setFont(entryFont);

    auto* passwordLabel = new QLabel("Password");
    passwordLabel->setFont(labelFont);
    passwordEntry->setFont(entryFont);
    passwordEntry->setEchoMode(QLineEdit::Password);

    auto* rePasswordLabel = new QLabel("Re-Password");
    rePasswordLabel->setFont(labelFont);
    rePasswordEntry->setFont(entryFont);
    rePasswordEntry->setEchoMode(QLineEdit::Password);

    auto* registerButton = new QPushButton("Register");

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
        if(userEntry->text().isEmpty())
        {
            QMessageBox errorMessage(QMessageBox::Critical, "Error", "Username is empty!", QMessageBox::NoButton, this);
            errorMessage.exec();
        }
        else if(passwordEntry->text().isEmpty() || rePasswordEntry->text().isEmpty())
        {
            QMessageBox errorMessage(QMessageBox::Critical, "Error", "Password is empty!", QMessageBox::NoButton, this);
            errorMessage.exec();
        }
        else if(passwordEntry->text().compare(rePasswordEntry->text(), Qt::CaseInsensitive) != 0)
        {
            QMessageBox errorMessage(QMessageBox::Critical, "Error", "Passwords do not match!", QMessageBox::NoButton, this);
            errorMessage.exec();
        }
        else
        {
            emit registerUser(userEntry->text(), passwordEntry->text());
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
