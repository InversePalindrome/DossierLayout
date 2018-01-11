/*
Copyright (c) 2018 InversePalindrome
Inverbienes - ArriendosList.cpp
InversePalindrome.com
*/


#include "ArriendosList.hpp"

#include <RapidXML/rapidxml.hpp>
#include <RapidXML/rapidxml_print.hpp>

#include <fstream>
#include <sstream>


ArriendosList::ArriendosList() :
    precioTotal(0),
    IVATotal(0)
{
}

ArriendosList::~ArriendosList()
{
    rapidxml::xml_document<> doc;

    auto* decl = doc.allocate_node(rapidxml::node_declaration);
    decl->append_attribute(doc.allocate_attribute("version", "1.0"));
    decl->append_attribute(doc.allocate_attribute("encoding", "UTF-8"));
    doc.append_node(decl);

    auto* arriendosNode = doc.allocate_node(rapidxml::node_element, "Arriendos");

    for(const auto& arriendo : arriendos)
    {
        auto* arriendoNode = doc.allocate_node(rapidxml::node_element, "Arriendo");

        arriendoNode->append_attribute(doc.allocate_attribute("local", doc.allocate_string(arriendo.getLocal().c_str())));
        arriendoNode->append_attribute(doc.allocate_attribute("nombre", doc.allocate_string(arriendo.getNombre().c_str())));
        arriendoNode->append_attribute(doc.allocate_attribute("telefono", doc.allocate_string(arriendo.getTelefono().c_str())));
        arriendoNode->append_attribute(doc.allocate_attribute("correo", doc.allocate_string(arriendo.getCorreo().c_str())));
        arriendoNode->append_attribute(doc.allocate_attribute("precio", doc.allocate_string(std::to_string(arriendo.getPrecio()).c_str())));
        arriendoNode->append_attribute(doc.allocate_attribute("IVA", doc.allocate_string(std::to_string(arriendo.getIVA()).c_str())));

        arriendosNode->append_node(arriendoNode);
    }

    doc.append_node(arriendosNode);

    std::ofstream outFile(fileName);

    outFile << doc;
}

void ArriendosList::cargarArriendos(const std::string& fileName)
{
    this->fileName = fileName;

    rapidxml::xml_document<> doc;
    std::ifstream inFile(fileName);
    std::ostringstream buffer;

    buffer << inFile.rdbuf();
    inFile.close();

    std::string content(buffer.str());
    doc.parse<0>(&content[0]);

    const auto* rootNode = doc.first_node("Arriendos");

    if(rootNode)
    {
        for(const auto* node = rootNode->first_node("Arriendo"); node; node = node->next_sibling())
        {
            std::string local, nombre, telefono, correo;
            std::size_t precio = 0u, IVA = 0u;

            std::stringstream stream;
            stream << node->first_attribute("local")->value() << ' ' << node->first_attribute("nombre")->value()
                   << ' ' << node->first_attribute("telefono")->value() << ' ' << node->first_attribute("correo")->value()
                   << ' ' << node->first_attribute("precio")->value() << ' ' << node->first_attribute("IVA")->value();
            stream >> local >> nombre >> telefono >> correo >> precio >> IVA;

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

void ArriendosList::removerArriendo(const std::string& local)
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
