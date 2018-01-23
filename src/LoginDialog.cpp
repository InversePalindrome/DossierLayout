/*
Copyright (c) 2018 InversePalindrome
Inverbienes - LoginDialog.cpp
InversePalindrome.com
*/


#include "LoginDialog.hpp"

#include <QDir>
#include <QFont>
#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QBoxLayout>
#include <QPushButton>
#include <QTextStream>
#include <QDomDocument>


LoginDialog::LoginDialog(QWidget* parent) :
    QDialog(parent),
    registerDialog(new RegisterDialog(this)),
    crypto(0x8540abc21e6c485d)
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
    auto* userEntry = new QLineEdit();
    userLabel->setFont(labelFont);
    userEntry->setFont(entryFont);

    auto* passwordLabel = new QLabel("Contraseña");
    auto* passwordEntry = new QLineEdit();
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
       [this, userEntry, passwordEntry]()
    {
        if(users.count(userEntry->text()) &&
           passwordEntry->text() == crypto.decryptToString(users.value(userEntry->text())))
        {
           emit loginAccepted(userEntry->text());

           userEntry->clear();
           passwordEntry->clear();
        }
    });
    QObject::connect(registerButton, &QPushButton::clicked,
       [this]()
    {
        close();
        registerDialog->open();
    });
    QObject::connect(registerDialog, &RegisterDialog::registerUser, this, &LoginDialog::addUser);

    loadUser("usuarios.xml");
}

LoginDialog::~LoginDialog()
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto usersElement = doc.createElement("Users");

    for(auto usuarioItr = users.constBegin(); usuarioItr != users.constEnd(); ++usuarioItr)
    {
        auto userElement = doc.createElement("User");

        userElement.setAttribute("name", usuarioItr.key());
        userElement.setAttribute("password", usuarioItr.value());

        usersElement.appendChild(userElement);
    }

    doc.appendChild(usersElement);

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       return;
    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

void LoginDialog::loadUser(const QString& fileName)
{
    this->fileName = fileName;

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

    auto usersElement = doc.firstChildElement("Users");

    auto usersList = usersElement.elementsByTagName("User");

    for(int i = 0; i < usersList.count(); ++i)
    {
        auto user = usersList.at(i);

        if(user.isElement())
        {
            auto userElement = user.toElement();

            users.insert(userElement.attribute("name"), userElement.attribute("password"));
        }
    }
}

void LoginDialog::addUser(const User& user)
{
    users.insert(user.first, crypto.encryptToString(user.second));

    QDir().mkdir(user.first);
}
