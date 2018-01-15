/*
Copyright (c) 2018 InversePalindrome
Inverbienes - RegistrarDialog.cpp
InversePalindrome.com
*/


#include "RegistrarDialog.hpp"

#include <QFont>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>


RegistrarDialog::RegistrarDialog(QWidget* parent) :
    QDialog(parent),
    usuarioEntry(new QLineEdit()),
    passwordEntry(new QLineEdit()),
    rePasswordEntry(new QLineEdit())
{
    setWindowTitle("Registrar Usuario");
    resize(600, 400);

    QFont labelFont("Arial", 12, QFont::Bold);
    QFont entryFont("Arial", 12);

    auto* usuarioLabel = new QLabel("Usuario");
    usuarioLabel->setFont(labelFont);
    usuarioEntry->setFont(entryFont);

    auto* passwordLabel = new QLabel("Contraseña");
    passwordLabel->setFont(labelFont);
    passwordEntry->setFont(entryFont);
    passwordEntry->setEchoMode(QLineEdit::Password);

    auto* rePasswordLabel = new QLabel("Re-Contraseña");
    rePasswordLabel->setFont(labelFont);
    rePasswordEntry->setFont(entryFont);
    rePasswordEntry->setEchoMode(QLineEdit::Password);

    auto* registrarBoton = new QPushButton("Registrar");

    auto* layout = new QVBoxLayout();

    layout->addWidget(usuarioLabel);
    layout->addWidget(usuarioEntry);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEntry);
    layout->addWidget(rePasswordLabel);
    layout->addWidget(rePasswordEntry);
    layout->addWidget(registrarBoton);

    setLayout(layout);

    QObject::connect(rePasswordEntry, &QLineEdit::textEdited,
        [this]()
    {
         bool isPasswordValid = passwordEntry->text() == rePasswordEntry->text();

         if(isPasswordValid)
           {
              passwordEntry->setStyleSheet("QLineEdit { background: rgb(0, 255, 0); selection-background-color: rgb(255, 255, 255); }");
              rePasswordEntry->setStyleSheet("QLineEdit { background: rgb(0, 255, 0); selection-background-color: rgb(255, 255, 255); }");
           }
         else
           {
              passwordEntry->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 255, 255); }");
              rePasswordEntry->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 255, 255); }");
           }
   });
   QObject::connect(registrarBoton, &QPushButton::clicked,
        [this]()
    {
        bool isPasswordValid = !passwordEntry->text().isEmpty() && passwordEntry->text() == rePasswordEntry->text();

        if(isPasswordValid)
        {
            emit registrarUsuario({usuarioEntry->text(), passwordEntry->text()});

            closeEvent(nullptr);
        }
    });
}

void RegistrarDialog::closeEvent(QCloseEvent*)
{
    usuarioEntry->clear();
    passwordEntry->clear();
    rePasswordEntry->clear();

    close();
    qobject_cast<QWidget*>(parent())->show();
}
