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
    registrarDialog(new RegistrarDialog(this)),
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

    auto* usuarioLabel = new QLabel("Usuario");
    auto* usuarioEntry = new QLineEdit();
    usuarioLabel->setFont(labelFont);
    usuarioEntry->setFont(entryFont);

    auto* passwordLabel = new QLabel("Contraseña");
    auto* passwordEntry = new QLineEdit();
    passwordLabel->setFont(labelFont);
    passwordEntry->setFont(entryFont);
    passwordEntry->setEchoMode(QLineEdit::Password);

    auto* ingresarBoton = new QPushButton("Ingresar");
    auto* registrarBoton = new QPushButton("Registrar");

    auto* botonLayout = new QHBoxLayout();
    botonLayout->addWidget(ingresarBoton);
    botonLayout->addWidget(registrarBoton);

    auto* layout = new QVBoxLayout(this);

    layout->addSpacing(15);
    layout->addWidget(logoLabel, 0, Qt::AlignCenter);
    layout->addSpacing(15);
    layout->addWidget(usuarioLabel);
    layout->addWidget(usuarioEntry);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEntry);
    layout->addLayout(botonLayout);

    setLayout(layout);

    QObject::connect(ingresarBoton, &QPushButton::clicked,
       [this, usuarioEntry, passwordEntry]()
    {
        if(usuarios.count(usuarioEntry->text()) &&
           passwordEntry->text() == crypto.decryptToString(usuarios.value(usuarioEntry->text())))
        {
           emit ingresoAceptado(usuarioEntry->text());

           usuarioEntry->clear();
           passwordEntry->clear();
        }
    });
    QObject::connect(registrarBoton, &QPushButton::clicked,
       [this]()
    {
        close();
        registrarDialog->open();
    });
    QObject::connect(registrarDialog, &RegistrarDialog::registrarUsuario, this, &LoginDialog::agregarUsuario);

    cargarUsuarios("usuarios.xml");
}

LoginDialog::~LoginDialog()
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto usuariosElement = doc.createElement("Usuarios");

    for(const auto& [nombre, password] : usuarios)
    {
        auto usuarioElement = doc.createElement("Usuario");

        usuarioElement.setAttribute("nombre", nombre);
        usuarioElement.setAttribute("password", password);

        usuariosElement.appendChild(usuarioElement);
    }

    doc.appendChild(usuariosElement);

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

void LoginDialog::cargarUsuarios(const QString& fileName)
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

    auto usuariosElement = doc.firstChildElement("Usuarios");

    auto usuariosList = usuariosElement.elementsByTagName("Usuario");

    for(int i = 0; i < usuariosList.count(); ++i)
    {
        auto usuario = usuariosList.at(i);

        if(usuario.isElement())
        {
            auto usuarioElement = usuario.toElement();

            usuarios.insert(usuarioElement.attribute("nombre"), usuarioElement.attribute("password"));
        }
    }
}

void LoginDialog::agregarUsuario(const Usuario& usuario)
{
    usuarios.insert(usuario.first, crypto.encryptToString(usuario.second));

    QDir().mkdir(usuario.first);
}
