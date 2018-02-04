/*
Copyright (c) 2018 InversePalindrome
DossierTable - SpreadSheet.cpp
InversePalindrome.com
*/


#include "SpreadSheet.hpp"
#include "AlignmentUtility.hpp"

#include <QFont>
#include <QFile>
#include <QMenu>
#include <QtXlsx>
#include <QLocale>
#include <QPainter>
#include <QPrinter>
#include <QSettings>
#include <QLineEdit>
#include <QTextStream>
#include <QHeaderView>
#include <QFontDialog>
#include <QPrintDialog>
#include <QColorDialog>
#include <QApplication>

#include <limits>


SpreadSheet::SpreadSheet(QWidget* parent, const QString& directory) :
    QTableWidget(parent),
    directory(directory),
    clipboard(QApplication::clipboard()),
    selectedColumn(-1),
    selectedRow(-1)
{
   setContextMenuPolicy(Qt::CustomContextMenu);
   setSelectionMode(QAbstractItemView::ContiguousSelection);

   verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
   verticalHeader()->setSectionsMovable(true);
   verticalHeader()->setDragEnabled(true);
   verticalHeader()->setDragDropMode(DragDropMode::InternalMove);

   horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
   horizontalHeader()->setSectionsMovable(true);
   horizontalHeader()->setDragEnabled(true);
   horizontalHeader()->setDragDropMode(DragDropMode::InternalMove);

   QObject::connect(horizontalHeader(), &QHeaderView::sectionClicked, [this](auto index) { selectedColumn = index;});
   QObject::connect(horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &SpreadSheet::openHeaderMenu);
   QObject::connect(horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &SpreadSheet::editHeader);
   QObject::connect(verticalHeader(), &QHeaderView::sectionClicked, [this](auto index) { selectedRow = index; });
   QObject::connect(verticalHeader(), &QHeaderView::customContextMenuRequested, this, &SpreadSheet::openHeaderMenu);
   QObject::connect(verticalHeader(), &QHeaderView::sectionDoubleClicked, this, &SpreadSheet::editHeader);
   QObject::connect(this, &SpreadSheet::customContextMenuRequested, this, &SpreadSheet::openCellMenu);

   loadSpreadSheet(directory + "SpreadSheet.xlsx");
}

SpreadSheet::~SpreadSheet()
{
   QSettings settings(directory + "Headers.ini", QSettings::IniFormat);
   settings.setValue("Horizontal", horizontalHeader()->saveState());
   settings.setValue("Vertical", verticalHeader()->saveState());

   saveToExcel(directory + "SpreadSheet.xlsx");
}

void SpreadSheet::loadSpreadSheet(const QString& fileName)
{
   QXlsx::Document doc(fileName);

   setRowCount(doc.dimension().lastRow() - 1);
   setColumnCount(doc.dimension().lastColumn() - 1);

   for(int column = 2; column <= doc.dimension().lastColumn(); ++column)
   {
       const auto* cell = doc.cellAt(1, column);

       auto* item = new QTableWidgetItem(cell->value().toString());
       item->setFont(cell->format().font());
       item->setTextColor(cell->format().fontColor());

       setHorizontalHeaderItem(column - 2, item);
   }

   for(int row = 2; row <= doc.dimension().lastRow(); ++row)
   {
       const auto* cell = doc.cellAt(row, 1);

       auto* item = new QTableWidgetItem(cell->value().toString());
       item->setFont(cell->format().font());
       item->setTextColor(cell->format().fontColor());

       setVerticalHeaderItem(row - 2, item);
   }

   for(int row = 2; row <= doc.dimension().lastRow(); ++row)
   {
       for(int column = 2; column <= doc.dimension().lastColumn(); ++column)
       {
           const auto* cell = doc.cellAt(row, column);

           auto* item = new QTableWidgetItem(cell->value().toString());
           item->setFont(cell->format().font());
           item->setTextColor(cell->format().fontColor());
           item->setTextAlignment(Utility::ExcelToQtAlignment
           (qMakePair(cell->format().horizontalAlignment(), cell->format().verticalAlignment())));
           item->setBackgroundColor(cell->format().patternBackgroundColor());

           setItem(row - 2, column - 2, item);
       }
   }

   for(const auto& mergedCell : doc.currentWorksheet()->mergedCells())
   {
       setSpan(mergedCell.firstRow() - 2, mergedCell.firstColumn() - 2, mergedCell.columnCount(), mergedCell.rowCount());
   }

   QSettings settings(directory + "Headers.ini", QSettings::IniFormat);
   horizontalHeader()->restoreState(settings.value("Horizontal").toByteArray());
   verticalHeader()->restoreState(settings.value("Vertical").toByteArray());
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

void SpreadSheet::insertColumn(QString columnName)
{ 
    QTableWidget::insertColumn(columnCount());

    if(columnName.isEmpty())
    {
        columnName = QString::number(columnCount());
    }

    QFont defaultHeaderFont("Arial", 10, QFont::Bold);

    auto* header = new QTableWidgetItem(columnName);
    header->setFont(defaultHeaderFont);
    setHorizontalHeaderItem(columnCount() - 1, header);

    for(int row = 0; row < rowCount(); ++row)
    {
        auto* item = new QTableWidgetItem();
        item->setBackgroundColor(Qt::white);

        setItem(row, columnCount() - 1, item);
    }
}

void SpreadSheet::insertRow(QString rowName)
{
    QTableWidget::insertRow(rowCount());

    if(rowName.isEmpty())
    {
        rowName = QString::number(rowCount());
    }

    QFont defaultHeaderFont("Arial", 10, QFont::Bold);

    auto* header = new QTableWidgetItem(rowName);
    header->setFont(defaultHeaderFont);
    setVerticalHeaderItem(rowCount() - 1, header);

    for(int column = 0; column < columnCount(); ++column)
    {
        auto* item = new QTableWidgetItem();
        item->setBackgroundColor(Qt::white);

        setItem(rowCount() - 1, column, item);
    }
}

void SpreadSheet::removeColumn()
{
    QTableWidget::removeColumn(selectedColumn);
    selectedColumn = -1;
}

void SpreadSheet::removeRow()
{
    QTableWidget::insertRow(selectedRow);
    selectedRow = -1;
}

void SpreadSheet::sortColumn(Qt::SortOrder order)
{
   auto columnItems = takeColumn(selectedColumn);

   if(order == Qt::AscendingOrder)
   {
       std::sort(columnItems.begin(), columnItems.end(), &SpreadSheet::compareCells);
   }
   else
   {
       std::sort(columnItems.rbegin(), columnItems.rend(), &SpreadSheet::compareCells);
   }

   setColumn(selectedColumn, columnItems);
}

void SpreadSheet::sortRow(Qt::SortOrder order)
{
   auto rowItems = takeRow(selectedRow);

   if(order == Qt::AscendingOrder)
   {
       std::sort(rowItems.begin(), rowItems.end(), &SpreadSheet::compareCells);
   }
   else
   {
       std::sort(rowItems.rbegin(), rowItems.rend(), &SpreadSheet::compareCells);
   }

   setRow(selectedRow, rowItems);
}

void SpreadSheet::merge()
{
   int top = rowCount();
   int left = columnCount();
   int bottom = 0;
   int right = 0;

   for(const auto& index : selectedIndexes())
   {
       if(top > index.row())
       {
          top = index.row();
       }
       if(left > index.column())
       {
          left = index.column();
       }
       if(bottom < index.row())
       {
          bottom = index.row();
       }
       if(right < index.column())
       {
          right = index.column();
       }
    }

    setSpan(top, left, bottom - top + 1, right - left + 1);
}

void SpreadSheet::split()
{
    for(const auto& index : selectedIndexes())
    {
        setSpan(index.row(), index.column(), 1, 1);
    }
}

double SpreadSheet::getSum()
{
    double sum = 0.;

    for(const auto& cell : selectedItems())
    {
        sum += cell->data(Qt::UserRole).toDouble();
    }

    clipboard->setText(QString::number(sum));

    return sum;
}

double SpreadSheet::getAverage()
{
    double average = getSum() / static_cast<double>(getCount());

    clipboard->setText(QString::number(average));

    return average;
}

double SpreadSheet::getMin()
{
    double min = std::numeric_limits<double>::max();

    for(const auto& cell : selectedItems())
    {
        auto number = cell->data(Qt::UserRole).toDouble();

        if(number < min)
        {
            min = number;
        }
    }

    clipboard->setText(QString::number(min));

    return min;
}

double SpreadSheet::getMax()
{
    double max = std::numeric_limits<double>::min();

    for(const auto& cell : selectedItems())
    {
        auto number = cell->data(Qt::UserRole).toDouble();

        if(number > max)
        {
            max = number;
        }
    }

    clipboard->setText(QString::number(max));

    return max;
}

std::size_t SpreadSheet::getCount()
{
    std::size_t count = selectedItems().size();

    clipboard->setText(QString::number(count));

    return count;
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

    QList<QRect> mergedCells;

    for(int row = 0; row < rowCount(); ++row)
    {
        for(int column = 0; column < columnCount(); ++column)
        {
            const auto* cell = item(row, column);

            QXlsx::Format format;
            format.setFont(cell->font());
            format.setFontColor(cell->textColor());
            format.setHorizontalAlignment(Utility::QtToExcelAlignment(cell->textAlignment()).first);
            format.setVerticalAlignment(Utility::QtToExcelAlignment(cell->textAlignment()).second);
            format.setPatternBackgroundColor(cell->backgroundColor());

            doc.write(row + 2, column + 2, cell->text(), format);

            if(columnSpan(row, column) > 0 || rowSpan(row, column) > 0)
            {
                bool alreadyExists = false;

                for(const auto& mergedCell : mergedCells)
                {
                    if(mergedCell.contains(column, row))
                    {
                        alreadyExists = true;
                        break;
                    }
                }

                if(!alreadyExists)
                {
                    mergedCells  << QRect(column, row, columnSpan(row, column), rowSpan(row, column));
                }
            }
        }
    }

    for(const auto& mergedCell : mergedCells)
    {
        doc.mergeCells(QXlsx::CellRange(mergedCell.top() + 2, mergedCell.left() + 2, mergedCell.bottom() + 2, mergedCell.right() + 2));
    }

    doc.saveAs(fileName);
}

SpreadSheet::ItemList SpreadSheet::takeColumn(int column)
{
    ItemList items;

    for(int row = 0; row < rowCount(); ++row)
    {
        items << takeItem(row, column);
    }

    return items;
}

SpreadSheet::ItemList SpreadSheet::takeRow(int row)
{
    ItemList items;

    for(int column = 0; column < columnCount(); ++column)
    {
        items << takeItem(row, column);
    }

    return items;
}

void SpreadSheet::setColumn(int column, const ItemList& items)
{
    for(int row = 0; row < items.size(); ++row)
    {
        setItem(row, column, items.at(row));
    }
}

void SpreadSheet::setRow(int row, const ItemList& items)
{
    for(int column = 0; column < items.size(); ++column)
    {
        setItem(row, column, items.at(column));
    }
}

bool SpreadSheet::compareCells(const QTableWidgetItem* item1, const QTableWidgetItem* item2)
{
    return item1->data(Qt::UserRole).toDouble() < item2->data(Qt::UserRole).toDouble();
}

void SpreadSheet::openHeaderMenu(const QPoint& position)
{
     auto* header = qobject_cast<QHeaderView*>(sender());

     QTableWidgetItem* cell;

     if(header->orientation() == Qt::Horizontal)
     {
         cell = horizontalHeaderItem(header->logicalIndexAt(position));
     }
     else
     {
         cell = verticalHeaderItem(header->logicalIndexAt(position));
     }

     auto* menu = new QMenu(this);
     menu->addAction("Font", [this, cell]
     {
         cell->setFont(QFontDialog::getFont(nullptr, cell->font(), this));
     });
     menu->addAction("Text Color", [this, cell]
     {
         cell->setTextColor(QColorDialog::getColor(Qt::black, this, "Text Color"));
     });

     auto* alignment = menu->addMenu("Alignment");
     alignment->addAction("Left", [cell] { cell->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); });
     alignment->addAction("Right", [cell] { cell->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); });
     alignment->addAction("Center", [cell] { cell->setTextAlignment(Qt::AlignCenter); });

     menu->exec(mapToGlobal(position));
}

void SpreadSheet::openCellMenu(const QPoint& position)
{
    auto* cell = itemAt(position);

    auto* menu = new QMenu(this);

    menu->addAction("Font", [this, cell]
    {
        cell->setFont(QFontDialog::getFont(nullptr, cell->font(), this));
    });

    auto* color = menu->addMenu("Color");
    color->addAction("Background", [this, cell]
    {
        cell->setBackgroundColor(QColorDialog::getColor(Qt::white, this, "Background Color"));
    });
    color->addAction("Text", [this, cell]
    {
        cell->setTextColor(QColorDialog::getColor(Qt::black, this, "Text Color"));
    });

    auto* format = menu->addMenu("Format");
    format->addAction("Currency", [cell]
    {
        cell->setText(QLocale().toCurrencyString(cell->data(Qt::UserRole).toLongLong()));
    });
    format->addAction("Percentage", [cell]
    {
        auto number = cell->data(Qt::UserRole).toDouble() * 100.;

        cell->setText(QString::number(number) + '%');
    });
    format->addAction("Scientific", [cell]
    {
        auto number = cell->data(Qt::UserRole).toDouble();

        QString scientificNumber;
        QTextStream oStream(&scientificNumber);
        oStream.setRealNumberPrecision(2);
        oStream << scientific << number;
        cell->setData(Qt::UserRole + 1, "FormatChange");
        cell->setText(scientificNumber);
    });
    format->addAction("Number", [cell]
    {
        cell->setText(cell->data(Qt::UserRole).toString());
    });

    auto* alignment = menu->addMenu("Alignment");
    alignment->addAction("Left", [cell] { cell->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); });
    alignment->addAction("Right", [cell] { cell->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); });
    alignment->addAction("Top", [cell] { cell->setTextAlignment(Qt::AlignTop | Qt::AlignHCenter); });
    alignment->addAction("Bottom", [cell] { cell->setTextAlignment(Qt::AlignBottom | Qt::AlignHCenter); });
    alignment->addAction("Center", [cell] { cell->setTextAlignment(Qt::AlignCenter); });

    menu->exec(mapToGlobal(position));
}

void SpreadSheet::editHeader(int logicalIndex)
{
    auto* header = qobject_cast<QHeaderView*>(sender());

    QRect rect;

    if(header->orientation() == Qt::Horizontal)
    {
        rect.setLeft(header->sectionPosition(logicalIndex));
        rect.setWidth(header->sectionSize(logicalIndex));
        rect.setTop(0);
        rect.setHeight(header->height());
    }
    else
    {
        rect.setTop(header->sectionPosition(logicalIndex));
        rect.setHeight(header->sectionSize(logicalIndex));
        rect.setLeft(0);
        rect.setWidth(header->width());
    }

    rect.adjust(1, 1, -1, -1);

    auto* headerEditor = new QLineEdit(header->viewport());
    headerEditor->move(rect.topLeft());
    headerEditor->resize(rect.size());
    headerEditor->setFrame(false);
    headerEditor->setText(header->model()->headerData(logicalIndex, header->orientation()).toString());
    headerEditor->setFocus();
    headerEditor->show();

    auto setData = [logicalIndex, header, headerEditor]
    {
       header->model()->setHeaderData(logicalIndex, header->orientation(), headerEditor->text());
       headerEditor->deleteLater();
    };

    QObject::connect(headerEditor, &QLineEdit::returnPressed, setData);
    QObject::connect(headerEditor, &QLineEdit::editingFinished, setData);
}
