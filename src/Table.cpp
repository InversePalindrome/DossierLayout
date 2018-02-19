/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Table.cpp
InversePalindrome.com
*/


#include "Table.hpp"
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


Table::Table(QWidget* parent, const QString& directory) :
    QTableWidget(parent),
    directory(directory),
    clipboard(QApplication::clipboard())
{
   setContextMenuPolicy(Qt::CustomContextMenu);
   setSelectionMode(QAbstractItemView::ContiguousSelection);

   horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
   verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

   QObject::connect(horizontalHeader(), &QHeaderView::customContextMenuRequested, this, &Table::openHeaderMenu);
   QObject::connect(horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &Table::editHeader);
   QObject::connect(verticalHeader(), &QHeaderView::customContextMenuRequested, this, &Table::openHeaderMenu);
   QObject::connect(verticalHeader(), &QHeaderView::sectionDoubleClicked, this, &Table::editHeader);
   QObject::connect(this, &Table::customContextMenuRequested, this, &Table::openCellsMenu);

   load(directory + "Table.xlsx");
}

Table::~Table()
{
   QSettings settings(directory + "Headers.ini", QSettings::IniFormat);
   settings.setValue("Horizontal", horizontalHeader()->saveState());
   settings.setValue("Vertical", verticalHeader()->saveState());

   saveToExcel(directory + "Table.xlsx");
}

void Table::load(const QString& fileName)
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
       setSpan(mergedCell.firstRow() - 2, mergedCell.firstColumn() - 2, mergedCell.rowCount(), mergedCell.columnCount());
   }

   QSettings settings(directory + "Headers.ini", QSettings::IniFormat);
   horizontalHeader()->restoreState(settings.value("Horizontal").toByteArray());
   verticalHeader()->restoreState(settings.value("Vertical").toByteArray());
}

void Table::save(const QString& fileName)
{
    if(fileName.endsWith(".pdf"))
    {
        saveToPdf(fileName);
    }
    else if(fileName.endsWith(".xlsx"))
    {
        saveToExcel(fileName);
    }
}

void Table::print()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);

    if(printDialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        render(&painter);
    }
}

void Table::insertColumn(const QString& columnName)
{
    QTableWidget::insertColumn(columnCount());

    auto* header = new QTableWidgetItem(columnName);
    header->setFont(QFont("Arial", 10, QFont::Bold));
    setHorizontalHeaderItem(columnCount() - 1, header);

    for(int row = 0; row < rowCount(); ++row)
    {
        auto* item = new QTableWidgetItem();
        item->setBackgroundColor(Qt::white);
        item->setTextColor(Qt::black);
        setItem(row, columnCount() - 1, item);
    }
}

void Table::insertRow(const QString& rowName)
{
    QTableWidget::insertRow(rowCount());

    auto* header = new QTableWidgetItem(rowName);
    header->setFont(QFont("Arial", 10, QFont::Bold));
    setVerticalHeaderItem(rowCount() - 1, header);

    for(int column = 0; column < columnCount(); ++column)
    {
        auto* item = new QTableWidgetItem();
        item->setBackgroundColor(Qt::white);
        item->setTextColor(Qt::black);
        setItem(rowCount() - 1, column, item);
    }
}

void Table::removeColumn()
{
    QTableWidget::removeColumn(currentColumn());
}

void Table::removeRow()
{
    QTableWidget::removeRow(currentRow());
}

void Table::sortColumn(Qt::SortOrder order)
{
   auto columnItems = takeColumn(currentColumn());

   if(order == Qt::AscendingOrder)
   {
       std::sort(columnItems.begin(), columnItems.end(), &Table::compareCells);
   }
   else
   {
       std::sort(columnItems.rbegin(), columnItems.rend(), &Table::compareCells);
   }

   setColumn(currentColumn(), columnItems);
}

void Table::sortRow(Qt::SortOrder order)
{
   auto rowItems = takeRow(currentRow());

   if(order == Qt::AscendingOrder)
   {
       std::sort(rowItems.begin(), rowItems.end(), &Table::compareCells);
   }
   else
   {
       std::sort(rowItems.rbegin(), rowItems.rend(), &Table::compareCells);
   }

   setRow(currentRow(), rowItems);
}

void Table::merge()
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

void Table::split()
{
    for(const auto& index : selectedIndexes())
    {
        setSpan(index.row(), index.column(), 1, 1);
    }
}

double Table::getSum()
{
    double sum = 0.;

    for(const auto& cell : selectedItems())
    {
        bool ok;
        auto number = cell->text().toDouble(&ok);

        if(ok)
        {
            sum += number;
        }
     }

     clipboard->setText(QString::number(sum));

     return sum;
}

double Table::getAverage()
{
    double average = getSum() / static_cast<double>(getCount());

    clipboard->setText(QString::number(average));

    return average;
}

double Table::getMin()
{
    double min = std::numeric_limits<double>::max();

    for(const auto& cell : selectedItems())
    {
        bool ok;
        auto number = cell->text().toDouble(&ok);

        if(ok && number < min)
        {
            min = number;
        }
    }

    clipboard->setText(QString::number(min));

    return min;
}

double Table::getMax()
{
    double max = std::numeric_limits<double>::min();

    for(const auto& cell : selectedItems())
    {
       bool ok;
       auto number = cell->text().toDouble(&ok);

       if(ok && number > max)
       {
           max = number;
       }
    }

    clipboard->setText(QString::number(max));

    return max;
}

std::size_t Table::getCount()
{
    std::size_t count = 0u;

    for(const auto& cell : selectedItems())
    {
        bool ok;
        cell->text().toDouble(&ok);

        if(ok)
        {
           ++count;
        }
    }

    clipboard->setText(QString::number(count));

    return count;
}

void Table::saveToPdf(const QString &fileName)
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

void Table::saveToExcel(const QString& fileName)
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

    selectAll();

    for(const auto& cell : selectedItems())
    {
        QXlsx::Format format;
        format.setFont(cell->font());
        format.setFontColor(cell->textColor());
        format.setHorizontalAlignment(Utility::QtToExcelAlignment(cell->textAlignment()).first);
        format.setVerticalAlignment(Utility::QtToExcelAlignment(cell->textAlignment()).second);
        format.setPatternBackgroundColor(cell->backgroundColor());

        doc.write(cell->row() + 2, cell->column() + 2, cell->text(), format);

        auto width = columnSpan(cell->row(), cell->column());
        auto height = rowSpan(cell->row(), cell->column());

        if(width > 1 || height > 1)
        {
            doc.mergeCells(QXlsx::CellRange(cell->row() + 2, cell->column() + 2,
            cell->row() + height + 1, cell->column() + width + 1), format);
        }
    }

    clearSelection();

    doc.saveAs(fileName);
}

Table::ItemList Table::takeColumn(int column)
{
    ItemList items;

    for(int row = 0; row < rowCount(); ++row)
    {
        items << takeItem(row, column);
    }

    return items;
}

Table::ItemList Table::takeRow(int row)
{
    ItemList items;

    for(int column = 0; column < columnCount(); ++column)
    {
        items << takeItem(row, column);
    }

    return items;
}

void Table::setColumn(int column, const ItemList& items)
{
    for(int row = 0; row < items.size(); ++row)
    {
        setItem(row, column, items.at(row));
    }
}

void Table::setRow(int row, const ItemList& items)
{
    for(int column = 0; column < items.size(); ++column)
    {
        setItem(row, column, items.at(column));
    }
}

bool Table::compareCells(const QTableWidgetItem* first, const QTableWidgetItem* second)
{
    bool isFirstOk, isSecondOk;

    auto firstNumber = first->text().toDouble(&isFirstOk);
    auto secondNumber = second->text().toDouble(&isSecondOk);

    if(isFirstOk && isSecondOk)
    {
        return firstNumber < secondNumber;
    }

    return first->text() < second->text();
}

void Table::openHeaderMenu(const QPoint& position)
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
     menu->addAction(tr("Text Color"), [this, cell]
     {
         cell->setTextColor(QColorDialog::getColor(Qt::black, this, tr("Text Color")));
     });

     auto* alignment = menu->addMenu(tr("Alignment"));
     alignment->addAction(tr("Left"), [cell] { cell->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); });
     alignment->addAction(tr("Right"), [cell] { cell->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter); });
     alignment->addAction(tr("Center"), [cell] { cell->setTextAlignment(Qt::AlignCenter); });

     menu->exec(mapToGlobal(position));
}

void Table::openCellsMenu(const QPoint& position)
{
    const auto& cells = selectedItems();

    auto* menu = new QMenu(this);

    menu->addAction("Font", [this, cells]
    {
        const auto& font = QFontDialog::getFont(nullptr, QFont("Arial", 10), this);

        for(const auto& cell : cells)
        {
           cell->setFont(font);
        }
    });

    auto* color = menu->addMenu(tr("Color"));
    color->addAction(tr("Background"), [this, cells]
    {
        const auto& color = QColorDialog::getColor(Qt::white, this, tr("Background Color"));

        for(const auto& cell : cells)
        {
           cell->setBackgroundColor(color);
        }
    });
    color->addAction(tr("Text"), [this, cells]
    {
        const auto& color = QColorDialog::getColor(Qt::black, this, tr("Text Color"));

        for(const auto& cell : cells)
        {
           cell->setTextColor(color);
        }
    });

    auto* format = menu->addMenu(tr("Format"));
    format->addAction(tr("Currency"), [cells]
    {
        for(const auto& cell : cells)
        {
            bool ok;
            auto number = cell->text().toLongLong(&ok);

            if(ok)
            {
               cell->setText(QLocale().toCurrencyString(number));
            }
        }
    });
    format->addAction(tr("Percentage"), [cells]
    {
        for(const auto& cell : cells)
        {
            bool ok;
            auto number = cell->text().toDouble(&ok);

            if(ok)
            {
                number *= 100.;

                cell->setText(QString::number(number) + '%');
            }
        }
    });
    format->addAction(tr("Scientific"), [cells]
    {
        for(const auto& cell : cells)
        {
            bool ok;
            auto number = cell->text().toDouble(&ok);

            if(ok)
            {
               QString scientificNumber;
               QTextStream oStream(&scientificNumber);
               oStream.setRealNumberPrecision(2);
               oStream << scientific << number;
               cell->setText(scientificNumber);
            }
        }
    });
    format->addAction(tr("Number"), [cells]
    {
        for(const auto& cell : cells)
        {
            QRegExp expression("(-?\\d+(?:[\\.,]\\d+(?:e\\d+)?)?)");
            expression.indexIn(cell->text());

            const auto& numbers = expression.capturedTexts();

            if(!numbers.empty() && !numbers.front().isEmpty())
            {
               cell->setText(numbers.front());
            }
        }
    });

    auto* alignment = menu->addMenu(tr("Alignment"));
    alignment->addAction(tr("Left"), [cells]
    {
        for(const auto& cell : cells)
        {
           cell->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        }
    });
    alignment->addAction(tr("Right"), [cells]
    {
        for(const auto& cell : cells)
        {
           cell->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        }
    });
    alignment->addAction(tr("Top"), [cells]
    {
        for(const auto& cell : cells)
        {
           cell->setTextAlignment(Qt::AlignTop | Qt::AlignHCenter);
        }
    });
    alignment->addAction(tr("Bottom"), [cells]
    {
        for(const auto& cell : cells)
        {
           cell->setTextAlignment(Qt::AlignBottom | Qt::AlignHCenter);
        }
    });
    alignment->addAction(tr("Center"), [cells]
    {
        for(const auto& cell : cells)
        {
           cell->setTextAlignment(Qt::AlignCenter);
        }
    });

    menu->exec(mapToGlobal(position));
}

void Table::editHeader(int logicalIndex)
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
