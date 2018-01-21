/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheet.cpp
InversePalindrome.com
*/


#include "SpreadSheet.hpp"
#include "SpreadSheetPrinter.hpp"

#include <QFont>
#include <QFile>
#include <QtXlsx>
#include <QStyle>
#include <QPainter>
#include <QHeaderView>
#include <QPushButton>
#include <QApplication>
#include <QTextStream>
#include <QTableWidgetItem>
#include <QPrintDialog>
#include <QDomDocument>
#include <qDebug>

SpreadSheet::SpreadSheet(QWidget* parent) :
    QTableWidget(parent),
    indexCategoriaSeleccionada(-1),
    indexItemSeleccionado(-1)
{
    QObject::connect(horizontalHeader(), &QHeaderView::sectionClicked, this, &SpreadSheet::categoriaSeleccionada);
    QObject::connect(verticalHeader(), &QHeaderView::sectionClicked, this, &SpreadSheet::itemSeleccionado);
//    QObject::connect(this, &SpreadSheet::)
}

SpreadSheet::~SpreadSheet()
{
    QXlsx::Document doc;
    QXlsx::Format headerFormat;
    headerFormat.setFontBold(true);

    for(int column = 0; column < columnCount(); ++column)
    {
        doc.write(1, column + 2, horizontalHeaderItem(column)->text(), headerFormat);
    }

    for(int row = 0; row < rowCount(); ++row)
    {
        doc.write(row + 2, 1, verticalHeaderItem(row)->text(), headerFormat);
    }

    for(int row = 0; row < rowCount(); ++row)
    {
        for(int column = 0; column < columnCount(); ++column)
        {
            doc.write(row + 2, column + 2, item(row, column)->text());
        }
    }

    doc.saveAs(fileName);
}

void SpreadSheet::cargarSpreadSheet(const QString& fileName)
{
   this->fileName = fileName;

   QXlsx::Document doc(fileName);

   QStringList horizontalHeaders;

   setRowCount(doc.dimension().lastRow() - 1);
   setColumnCount(doc.dimension().lastColumn() - 1);

   for(int column = 2; column <= doc.dimension().lastColumn(); ++column)
   {
       horizontalHeaders.push_back(doc.read(1, column).toString());
   }

   QStringList verticalHeaders;

   for(int row = 2; row <= doc.dimension().lastRow(); ++row)
   {
       verticalHeaders.push_back(doc.read(row, 1).toString());
   }

   setHorizontalHeaderLabels(horizontalHeaders);
   setVerticalHeaderLabels(verticalHeaders);

   QFont headerFont("Arial", 10, QFont::Bold);
   horizontalHeader()->setFont(headerFont);
   verticalHeader()->setFont(headerFont);

   for(int row = 2; row <= doc.dimension().lastRow(); ++row)
   {
       for(int column = 2; column <= doc.dimension().lastColumn(); ++column)
       {
           setItem(row - 2, column - 2, new QTableWidgetItem(doc.read(row, column).toString()));
       }
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

void SpreadSheet::removerCategoriaSeleccionada()
{
   removeColumn(indexCategoriaSeleccionada);
   indexCategoriaSeleccionada = -1;
}

void SpreadSheet::removerItemSeleccionado()
{
   removeRow(indexItemSeleccionado);
   indexItemSeleccionado = -1;
}

void SpreadSheet::setFileName(const QString& fileName)
{
    this->fileName = fileName;
}

void SpreadSheet::categoriaSeleccionada(int index)
{
   indexCategoriaSeleccionada = index;
}

void SpreadSheet::itemSeleccionado(int index)
{
    indexItemSeleccionado = index;
}
