/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheetList.cpp
InversePalindrome.com
*/


#include "SpreadSheetList.hpp"

#include <QFile>
#include <QTextStream>
#include <QDomDocument>


SpreadSheetList::SpreadSheetList(QWidget* parent) :
    parent(parent)
{
}

SpreadSheetList::~SpreadSheetList()
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto spreadSheetsElement = doc.createElement("SpreadSheets");

    for(auto spreadSheetItr = spreadSheets.constBegin(); spreadSheetItr != spreadSheets.constEnd(); ++spreadSheetItr)
    {
        auto spreadSheetElement = doc.createElement("SpreadSheet");
        spreadSheetElement.setAttribute("nombre", spreadSheetItr.key());

        spreadSheetsElement.appendChild(spreadSheetElement);
    }

    doc.appendChild(spreadSheetsElement);

    QFile file(usuario + "/SpreadSheets.xml");

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

void SpreadSheetList::cargarSpreadSheets(const QString& usuario)
{
    this->usuario = usuario;

    QDomDocument doc;
    QFile file(usuario + "/SpreadSheets.xml");

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

    auto spreadSheetsElement = doc.firstChildElement("SpreadSheets");

    auto spreadSheetList = spreadSheetsElement.elementsByTagName("SpreadSheet");

    for(int i = 0; i < spreadSheetList.size(); ++i)
    {
        auto spreadSheetNode = spreadSheetList.at(i);

        if(spreadSheetNode.isElement())
        {
            auto spreadSheetElement = spreadSheetNode.toElement();

            const auto& spreadSheetNombre = spreadSheetElement.attribute("nombre");

            auto* spreadSheet = new SpreadSheet(parent);
            spreadSheet->cargarSpreadSheet(usuario + '/' + spreadSheetNombre + "/SpreadSheet.xlsx");

            spreadSheets.insert(spreadSheetNombre, spreadSheet);
        }
    }
}

void SpreadSheetList::agregarSpreadSheet(const QString& nombre, SpreadSheet* spreadSheet)
{
    spreadSheet->setFileName(usuario + '/' + nombre + "/SpreadSheet.xlsx");
    spreadSheets.insert(nombre, spreadSheet);
}

void SpreadSheetList::removerSpreadSheet(const QString& nombre)
{
    spreadSheets.remove(nombre);
}

SpreadSheet* SpreadSheetList::operator[](const QString& nombre)
{
    return spreadSheets[nombre];
}

SpreadSheet* SpreadSheetList::operator[](const QString& nombre) const
{
    return spreadSheets[nombre];
}

SpreadSheets::iterator SpreadSheetList::begin()
{
    return spreadSheets.begin();
}

SpreadSheets::iterator SpreadSheetList::end()
{
    return spreadSheets.end();
}

SpreadSheets::const_iterator SpreadSheetList::cbegin() const
{
    return spreadSheets.cbegin();
}

SpreadSheets::const_iterator SpreadSheetList::cend() const
{
    return spreadSheets.cend();
}
