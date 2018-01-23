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
        spreadSheetElement.setAttribute("name", spreadSheetItr.key());

        spreadSheetsElement.appendChild(spreadSheetElement);
    }

    doc.appendChild(spreadSheetsElement);

    QFile file(user + "/SpreadSheets.xml");

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

void SpreadSheetList::loadSpreadSheets(const QString& usuario)
{
    this->user = usuario;

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

            const auto& spreadSheetNombre = spreadSheetElement.attribute("name");

            auto* spreadSheet = new SpreadSheet(parent);
            spreadSheet->loadSpreadSheet(usuario + '/' + spreadSheetNombre + "/SpreadSheet.xlsx");

            spreadSheets.insert(spreadSheetNombre, spreadSheet);
        }
    }
}

void SpreadSheetList::addSpreadSheet(const QString& name, SpreadSheet* spreadSheet)
{
    spreadSheet->setFileName(user + '/' + name + "/SpreadSheet.xlsx");
    spreadSheets.insert(name, spreadSheet);
}

void SpreadSheetList::removeSpreadSheet(const QString& name)
{
    spreadSheets.remove(name);
}

SpreadSheet* SpreadSheetList::operator[](const QString& name)
{
    return spreadSheets[name];
}

SpreadSheet* SpreadSheetList::operator[](const QString& name) const
{
    return spreadSheets[name];
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
