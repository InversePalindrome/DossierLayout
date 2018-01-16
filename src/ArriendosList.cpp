/*
Copyright (c) 2018 InversePalindrome
Inverbienes - ArriendosList.cpp
InversePalindrome.com
*/


#include "ArriendosList.hpp"

#include <QFile>
#include <QTextStream>
#include <QDomDocument>


ArriendosList::ArriendosList() :
    ArriendosList("")
{
}

ArriendosList::ArriendosList(const QString& fileName) :
    precioTotal(0),
    IVATotal(0)
{
    cargarArriendos(fileName);
}

ArriendosList::~ArriendosList()
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto arriendosElement = doc.createElement("Arriendos");

    for(const auto& arriendo : arriendos)
    {
        auto arriendoElement = doc.createElement("Arriendo");

        arriendoElement.setAttribute("local", arriendo.getLocal());
        arriendoElement.setAttribute("nombre", arriendo.getNombre());
        arriendoElement.setAttribute("telefono", arriendo.getTelefono());
        arriendoElement.setAttribute("correo", arriendo.getCorreo());
        arriendoElement.setAttribute("precio", QString::number(arriendo.getPrecio()));
        arriendoElement.setAttribute("IVA", QString::number(arriendo.getIVA()));

        arriendosElement.appendChild(arriendoElement);
    }

    doc.appendChild(arriendosElement);

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

void ArriendosList::cargarArriendos(const QString& fileName)
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

    auto arriendosElement = doc.firstChildElement("Arriendos");

    auto arriendosList = arriendosElement.elementsByTagName("Arriendo");

    for(int i = 0; i < arriendosList.count(); ++i)
    {
        auto arriendo = arriendosList.at(i);

        if(arriendo.isElement())
        {
            auto arriendoElement = arriendo.toElement();

            const auto& local = arriendoElement.attribute("local");
            const auto& nombre = arriendoElement.attribute("nombre");
            const auto& telefono = arriendoElement.attribute("telefono");
            const auto& correo = arriendoElement.attribute("correo");
            auto precio = arriendoElement.attribute("precio").toULongLong();
            auto IVA = arriendoElement.attribute("IVA").toULongLong();

            agregarArriendo(Arriendo(local, nombre, telefono, correo, precio, IVA));
        }
    }

    emit enviarArriendos(arriendos);
}

void ArriendosList::agregarArriendo(const Arriendo& arriendo)
{
    arriendos.insert(arriendo);

    precioTotal += arriendo.getPrecio();
    IVATotal += arriendo.getIVA();

    emit cambiaronTotales(precioTotal, IVATotal);
}

void ArriendosList::removerArriendo(const QString& local)
{
    auto arriendoItr = arriendos.find(local);

    if(arriendoItr != std::end(arriendos))
    {
        precioTotal -= arriendoItr->getPrecio();
        IVATotal -= arriendoItr->getIVA();

        arriendos.erase(arriendoItr);

        emit cambiaronTotales(precioTotal, IVATotal);
    }
}

void ArriendosList::setArriendos(const Arriendos& arriendos)
{
    this->arriendos = arriendos;
}
