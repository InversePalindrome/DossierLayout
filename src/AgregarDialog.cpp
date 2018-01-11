/*
Copyright (c) 2018 InversePalindrome
Inverbienes - AgregarDialog.cpp
InversePalindrome.com
*/


#include "AgregarDialog.hpp"

#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QGridLayout>


AgregarDialog::AgregarDialog(QWidget* parent) :
    QDialog(parent),
    layout(new QGridLayout(this)),
    localEntry(new QLineEdit()),
    nombreEntry(new QLineEdit()),
    telefonoEntry(new QLineEdit()),
    correoEntry(new QLineEdit()),
    precioEntry(new QLineEdit()),
    IVAEntry(new QLineEdit())
{
    setWindowTitle("Agregar Arriendo");
    resize(1000, 900);

    QFont labelFont("Arial", 12, QFont::Bold);
    QFont entryFont("Arial", 12);

    auto* localLabel = new QLabel("Local: ");
    localLabel->setFont(labelFont);
    localEntry->setFont(entryFont);

    auto* nombreLabel = new QLabel("Nombre: ");
    nombreLabel->setFont(labelFont);
    nombreEntry->setFont(entryFont);

    auto* telefonoLabel = new QLabel("Telefono: ");
    telefonoLabel->setFont(labelFont);
    telefonoEntry->setFont(entryFont);

    auto* correoLabel = new QLabel("Correo: ");
    correoLabel->setFont(labelFont);
    correoEntry->setFont(entryFont);

    auto* precioLabel = new QLabel("Precio: ");
    precioLabel->setFont(labelFont);
    precioEntry->setFont(entryFont);

    auto* IVALabel = new QLabel("IVA: ");
    IVALabel->setFont(labelFont);
    IVAEntry->setFont(entryFont);

    auto* agregarBoton = new QPushButton("Agregar");

    layout->addWidget(localLabel, 0, 0, 1, 1);
    layout->addWidget(localEntry, 0, 1, 1, 1);
    layout->addWidget(nombreLabel, 1, 0, 1, 1);
    layout->addWidget(nombreEntry, 1, 1, 1, 1);
    layout->addWidget(telefonoLabel, 2, 0, 1, 1);
    layout->addWidget(telefonoEntry, 2, 1, 1, 1);
    layout->addWidget(correoLabel, 3, 0, 1, 1);
    layout->addWidget(correoEntry, 3, 1, 1, 1);
    layout->addWidget(precioLabel, 4, 0, 1, 1);
    layout->addWidget(precioEntry, 4, 1, 1, 1);
    layout->addWidget(IVALabel, 5, 0, 1, 1);
    layout->addWidget(IVAEntry, 5, 1, 1, 1);
    layout->addWidget(agregarBoton, 6, 0, 1, 2);

    setLayout(layout);

    QObject::connect(agregarBoton, &QPushButton::clicked, this, &AgregarDialog::agregoArriendo);
}

void AgregarDialog::agregoArriendo()
{
    const auto& local = localEntry->text().toStdString();
    const auto& nombre = nombreEntry->text().toStdString();
    const auto& telefono = telefonoEntry->text().toStdString();
    const auto& correo = correoEntry->text().toStdString();
    std::size_t precio = 0u;
    std::size_t IVA = 0u;

    try
    {
        precio = std::stoull(precioEntry->text().toStdString());
    }
    catch(const std::invalid_argument&)
    {
        precio = 0;
    }

    try
    {
        IVA = std::stoull(IVAEntry->text().toStdString());
    }
    catch(const std::invalid_argument&)
    {
        IVA = 0;
    }

    emit agregarArriendo(Arriendo(local, nombre, telefono, correo, precio, IVA));

    nombreEntry->clear();
    telefonoEntry->clear();
    correoEntry->clear();
    localEntry->clear();
    precioEntry->clear();
    IVAEntry->clear();

    close();
}
