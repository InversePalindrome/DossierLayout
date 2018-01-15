/*
Copyright (c) 2018 InversePalindrome
Inverbienes - LoginDialog.cpp
InversePalindrome.com
*/


#include "LoginDialog.hpp"

#include <RapidXML/rapidxml.hpp>
#include <RapidXML/rapidxml_print.hpp>

#include <QDir>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>


#include <sstream>
#include <fstream>


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
           usuarioEntry->clear();
           passwordEntry->clear();

           emit ingresoAceptado(usuarioEntry->text().toStdString());
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
    rapidxml::xml_document<> doc;

    auto* decl = doc.allocate_node(rapidxml::node_declaration);
    decl->append_attribute(doc.allocate_attribute("version", "1.0"));
    decl->append_attribute(doc.allocate_attribute("encoding", "UTF-8"));
    doc.append_node(decl);

    auto* usuariosNode = doc.allocate_node(rapidxml::node_element, "Usuarios");

    auto usuario = usuarios.constBegin();

    while(usuario != usuarios.cend())
    {
        auto* usuarioNode = doc.allocate_node(rapidxml::node_element, "Usuario");

        usuarioNode->append_attribute(doc.allocate_attribute("Nombre", doc.allocate_string(usuario.key().toStdString().c_str())));
        usuarioNode->append_attribute(doc.allocate_attribute("Password", doc.allocate_string(usuario.value().toStdString().c_str())));

        usuariosNode->append_node(usuarioNode);

        ++usuario;
    }

    doc.append_node(usuariosNode);

    std::ofstream outFile(fileName);

    outFile << doc;
}

void LoginDialog::agregarUsuario(const Usuario& usuario)
{
    usuarios.insert(usuario.first, crypto.encryptToString(usuario.second));

    QDir().mkdir(usuario.first);
}

void LoginDialog::cargarUsuarios(const std::string& fileName)
{
    this->fileName = fileName;

    rapidxml::xml_document<> doc;
    std::ifstream inFile(fileName);
    std::ostringstream buffer;

    buffer << inFile.rdbuf();
    inFile.close();

    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    const auto* rootNode = doc.first_node("Usuarios");

    if(rootNode)
    {
       for(const auto* node = rootNode->first_node("Usuario"); node; node = node->next_sibling())
       {
           usuarios.insert(QString::fromStdString(node->first_attribute("Nombre")->value()),
                    QString::fromStdString(node->first_attribute("Password")->value()));
       }
    }
}
