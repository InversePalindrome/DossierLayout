/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheet.cpp
InversePalindrome.com
*/


#include "SpreadSheet.hpp"
#include "SpreadSheetPrinter.hpp"

#include <QFont>
#include <QFile>
#include <QStyle>
#include <QPainter>
#include <QHeaderView>
#include <QPushButton>
#include <QApplication>
#include <QTextStream>
#include <QTableWidgetItem>
#include <QPrintDialog>
#include <QDomDocument>


SpreadSheet::SpreadSheet(QWidget* parent) :
    QTableWidget(parent)
{
    QFont headerFont("Arial", 10);
    verticalHeader()->setFont(headerFont);
    horizontalHeader()->setFont(headerFont);
}

SpreadSheet::~SpreadSheet()
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto spreadSheetElement = doc.createElement("SpreadSheet");

    QStringList categories;

    auto categoriesElement = doc.createElement("Categories");

    for(int column = 0; column < columnCount(); ++column)
    {
        categories.push_back(horizontalHeaderItem(column)->text());
        categoriesElement.appendChild(doc.createElement(categories.back()));
    }

    spreadSheetElement.appendChild(categoriesElement);

    QStringList items;

    for(int row = 0; row < rowCount(); ++row)
    {
        items.push_back(verticalHeaderItem(row)->text());
    }

    for(int row = 0; row < rowCount(); ++row)
    {
        auto itemElement = doc.createElement("Item");

        for(int column = 0; column < columnCount(); ++column)
        {
           itemElement.setAttribute(categories.at(column), item(row, column)->text());
        }

        itemElement.setAttribute("name", items.at(row));

        spreadSheetElement.appendChild(itemElement);
    }

    doc.appendChild(spreadSheetElement);

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

void SpreadSheet::cargarSpreadSheet(const QString& fileName)
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
}

void SpreadSheet::insertarCategoria(QString categoria)
{ 
    insertColumn(columnCount());

    if(categoria.isEmpty())
    {
        categoria = QString::number(columnCount());
    }

    setHorizontalHeaderItem(columnCount() - 1, new QTableWidgetItem(categoria));

    for(int row = 0; row < rowCount(); ++row)
    {
        setItem(row, columnCount() - 1, new QTableWidgetItem());
    }
}

void SpreadSheet::insertarItem(QString item)
{
    insertRow(rowCount());

    if(item.isEmpty())
    {
        item = QString::number(rowCount());
    }

    setVerticalHeaderItem(rowCount() - 1, new QTableWidgetItem(item));

    for(int column = 0; column < columnCount(); ++column)
    {
        setItem(rowCount() - 1, column, new QTableWidgetItem());
    }
}
