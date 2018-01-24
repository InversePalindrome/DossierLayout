/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheet.cpp
InversePalindrome.com
*/


#include "SpreadSheet.hpp"

#include <QFont>
#include <QFile>
#include <QMenu>
#include <QtXlsx>
#include <QPainter>
#include <QPrinter>
#include <QHeaderView>
#include <QFontDialog>
#include <QPrintDialog>
#include <QColorDialog>
#include <QTableWidgetItem>


SpreadSheet::SpreadSheet(QWidget* parent) :
    QTableWidget(parent),
    selectedCategoryIndex(-1),
    selectedItemIndex(-1)
{
   setContextMenuPolicy(Qt::CustomContextMenu);

   verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
   verticalHeader()->setSectionsMovable(true);
   verticalHeader()->setDragEnabled(true);
   verticalHeader()->setDragDropMode(DragDropMode::InternalMove);

   horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
   horizontalHeader()->setSectionsMovable(true);
   horizontalHeader()->setDragEnabled(true);
   horizontalHeader()->setDragDropMode(DragDropMode::InternalMove);

   QObject::connect(horizontalHeader(), &QHeaderView::sectionClicked, this, &SpreadSheet::categorySelcted);
   QObject::connect(horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &SpreadSheet::openContextMenu);
   QObject::connect(verticalHeader(), &QHeaderView::sectionClicked, this, &SpreadSheet::itemSelected);
   QObject::connect(verticalHeader(), &QHeaderView::customContextMenuRequested, this, &SpreadSheet::openContextMenu);
   QObject::connect(this, &SpreadSheet::customContextMenuRequested, this, &SpreadSheet::openContextMenu);
}

SpreadSheet::~SpreadSheet()
{
   saveToExcel(fileName);
}

void SpreadSheet::loadSpreadSheet(const QString& fileName)
{
   this->fileName = fileName;

   QXlsx::Document doc(fileName);

   setRowCount(doc.dimension().lastRow() - 1);
   setColumnCount(doc.dimension().lastColumn() - 1);

   for(int column = 2; column <= doc.dimension().lastColumn(); ++column)
   {
       auto* cell = doc.cellAt(1, column);

       auto* item = new QTableWidgetItem(cell->value().toString());
       item->setFont(cell->format().font());
       item->setTextColor(cell->format().fontColor());

       setHorizontalHeaderItem(column - 2, item);
   }

   for(int row = 2; row <= doc.dimension().lastRow(); ++row)
   {
       auto* cell = doc.cellAt(row, 1);

       auto* item = new QTableWidgetItem(cell->value().toString());
       item->setFont(cell->format().font());
       item->setTextColor(cell->format().fontColor());

       setVerticalHeaderItem(row - 2, item);
   }

   for(int row = 2; row <= doc.dimension().lastRow(); ++row)
   {
       for(int column = 2; column <= doc.dimension().lastColumn(); ++column)
       {
           auto* cell = doc.cellAt(row, column);

           auto* item = new QTableWidgetItem(cell->value().toString());
           item->setFont(cell->format().font());
           item->setTextColor(cell->format().fontColor());
           item->setBackgroundColor(cell->format().patternBackgroundColor());

           setItem(row - 2, column - 2, item);
       }
   }
}

void SpreadSheet::saveSpreadSheet(const QString& fileName)
{
    if(fileName.endsWith(".pdf"))
    {
        QPrinter printer;

        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(fileName);

        QPainter painter(&printer);

        double xScale = printer.pageRect().width() / static_cast<double>(width());
        double yScale = printer.pageRect().height() / static_cast<double>(height());
        double scale = qMin(xScale, yScale);
        painter.scale(scale, scale);

        render(&painter);
    }
    else if(fileName.endsWith(".xlsx"))
    {
        saveToExcel(fileName);
    }
}

void SpreadSheet::print()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);

    if(printDialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        render(&painter);
    }
}

void SpreadSheet::insertCategory(QString category)
{ 
    insertColumn(columnCount());

    if(category.isEmpty())
    {
        category = QString::number(columnCount());
    }

    QFont defaultHeaderFont("Arial", 10, QFont::Bold);

    auto* header = new QTableWidgetItem(category);
    header->setFont(defaultHeaderFont);
    setHorizontalHeaderItem(columnCount() - 1, header);

    for(int row = 0; row < rowCount(); ++row)
    {
        auto* item = new QTableWidgetItem();
        item->setBackgroundColor(Qt::white);

        setItem(row, columnCount() - 1, item);
    }
}

void SpreadSheet::insertItem(QString item)
{
    insertRow(rowCount());

    if(item.isEmpty())
    {
        item = QString::number(rowCount());
    }

    QFont defaultHeaderFont("Arial", 10, QFont::Bold);

    auto* header = new QTableWidgetItem(item);
    header->setFont(defaultHeaderFont);
    setVerticalHeaderItem(rowCount() - 1, header);

    for(int column = 0; column < columnCount(); ++column)
    {
        auto* item = new QTableWidgetItem();
        item->setBackgroundColor(Qt::white);

        setItem(rowCount() - 1, column, item);
    }
}

void SpreadSheet::removeSelectedCategory()
{
    removeColumn(selectedCategoryIndex);
    selectedCategoryIndex = -1;
}

void SpreadSheet::removeSelectedItem()
{
    removeRow(selectedItemIndex);
    selectedItemIndex = -1;
}

void SpreadSheet::setFileName(const QString& fileName)
{
    this->fileName = fileName;
}

void SpreadSheet::categorySelcted(int index)
{
    selectedCategoryIndex = index;
}

void SpreadSheet::itemSelected(int index)
{
    selectedItemIndex = index;
}

void SpreadSheet::saveToExcel(const QString& fileName)
{
    QXlsx::Document doc;

    for(int column = 0; column < columnCount(); ++column)
    {
        const auto* cell = horizontalHeaderItem(column);

        QXlsx::Format format;
        format.setFont(cell->font());
        format.setFontColor(cell->textColor());

        doc.write(1, column + 2, cell->text(), format);
    }

    for(int row = 0; row < rowCount(); ++row)
    {
        const auto* cell = verticalHeaderItem(row);

        QXlsx::Format format;
        format.setFont(cell->font());
        format.setFontColor(cell->textColor());

        doc.write(row + 2, 1, cell->text(), format);
    }

    for(int row = 0; row < rowCount(); ++row)
    {
        for(int column = 0; column < columnCount(); ++column)
        {
            const auto* cell = item(row, column);

            QXlsx::Format format;
            format.setFont(cell->font());
            format.setFontColor(cell->textColor());
            format.setPatternBackgroundColor(cell->backgroundColor());

            doc.write(row + 2, column + 2, cell->text(), format);
        }
    }

    doc.saveAs(fileName);
}

void SpreadSheet::openContextMenu(const QPoint& position)
{
    auto* table = qobject_cast<SpreadSheet*>(sender());
    auto* header = qobject_cast<QHeaderView*>(sender());

    QTableWidgetItem* cell;

    bool isHeader = false;

    if(table)
    {
       cell = itemAt(position);
    }
    else if(header)
    {
       if(header->orientation() == Qt::Horizontal)
       {
           cell = horizontalHeaderItem(header->logicalIndexAt(position));
       }
       else
       {
           cell = verticalHeaderItem(header->logicalIndexAt(position));
       }

       isHeader = true;
    }

    auto* menu = new QMenu(this);

    auto* color = menu->addMenu("Color");
    auto* backgroundColor = color->addAction("Background", [this, cell]()
    {
        cell->setBackgroundColor(QColorDialog::getColor(Qt::white, this, "Background Color"));
    });
    color->addAction("Text", [this, cell]()
    {
        cell->setTextColor(QColorDialog::getColor(Qt::black, this, "Text Color"));
    });
    menu->addAction("Font", [this, cell]()
    {
        cell->setFont(QFontDialog::getFont(nullptr, cell->font(), this));
    });

    if(isHeader)
    {
        backgroundColor->setEnabled(false);
    }

    menu->exec(mapToGlobal(position));
}
