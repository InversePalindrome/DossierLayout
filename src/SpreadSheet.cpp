/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheet.cpp
InversePalindrome.com
*/


#include "SpreadSheet.hpp"
#include "SpreadSheetPrinter.hpp"

#include <QFont>
#include <QStyle>
#include <QPainter>
#include <QHeaderView>
#include <QPushButton>
#include <QApplication>
#include <QTableWidgetItem>
#include <QPrintDialog>

#include <sstream>


SpreadSheet::SpreadSheet(QWidget* parent) :
    QTableWidget(parent)
{
    setRowCount(1);
    setColumnCount(7);
    setSortingEnabled(false);

    QStringList arriendosInfo, totalInfo;

    arriendosInfo << "Local" << "Nombre" << "Telefono" << "Correo" << "Precio" << "IVA" << "";
    totalInfo << "Total";

    setHorizontalHeaderLabels(arriendosInfo);
    setVerticalHeaderLabels(totalInfo);

    QFont font("Arial", 10, QFont::Bold);
    horizontalHeader()->setFont(font);
    verticalHeader()->setFont(font);

    for(int column = 0; column < columnCount(); ++column)
    {
        auto* item = new QTableWidgetItem();
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        setItem(0, column, item);
    }

    auto* precioItem = item(0, columnCount() - 3);
    precioItem->setTextColor(QColor(0, 255, 0));
    precioItem->setFont(font);

    auto* IVAItem = item(0, columnCount() - 2);
    IVAItem->setTextColor(QColor(255, 0, 0));
    IVAItem->setFont(font);
}

void SpreadSheet::agregarArriendo(const Arriendo& arriendo)
{
    insertRow(rowCount() - 1);

    for(int column = 0; column < columnCount() - 1; ++column)
    {
        const auto& category = horizontalHeaderItem(column)->text().toStdString();
        std::string value;
        auto row = rowCount() - 2;

        if(category == "Local")
        {
            value = arriendo.getLocal();
        }
        else if(category == "Nombre")
        {
            value = arriendo.getNombre();
        }
        else if(category == "Telefono")
        {
            value = arriendo.getTelefono();
        }
        else if(category == "Correo")
        {
            value = arriendo.getCorreo();
        }
        else if(category == "Precio")
        {
            value = '$' + std::to_string(arriendo.getPrecio());
        }
        else if(category == "IVA")
        {
            value = '$' + std::to_string(arriendo.getIVA());
        }

        setItem(row, column, new QTableWidgetItem(QString::fromStdString(value)));
    }

    auto* deleteButton = new QPushButton(QIcon(QApplication::style()->
                        standardIcon(QStyle::SP_TrashIcon)), "Delete", this);

    QObject::connect(deleteButton, &QPushButton::clicked, this, &SpreadSheet::removerArriendo);

    setIndexWidget(model()->index(rowCount() - 2, columnCount() - 1), deleteButton);
}

void SpreadSheet::guardarArriendos()
{
    Arriendos arriendos;

    for(int row = 0; row < rowCount() - 1; ++row)
    {
        Arriendo arriendo;

        for(int column = 0; column < columnCount() - 1; ++column)
        {
           const auto& category = horizontalHeaderItem(column)->text().toStdString();
           auto* arriendoItem = item(row, column);

           if(category == "Local")
           {
               arriendo.setLocal(arriendoItem->text().toStdString());
           }
           else if(category == "Nombre")
           {
               arriendo.setNombre(arriendoItem->text().toStdString());
           }
           else if(category == "Telefono")
           {
               arriendo.setTelefono(arriendoItem->text().toStdString());
           }
           else if(category == "Correo")
           {
               arriendo.setCorreo(arriendoItem->text().toStdString());
           }
           else if(category == "Precio")
           {
               std::istringstream iStream(arriendoItem->text().toStdString());

               char moneySign;
               std::size_t precio = 0;

               iStream >> moneySign >> precio;

               arriendo.setPrecio(precio);
           }
           else if(category == "IVA")
           {
               std::istringstream iStream(arriendoItem->text().toStdString());

               char moneySign;
               std::size_t IVA = 0;

               iStream >> moneySign >> IVA;

               arriendo.setPrecio(IVA);
           }
        }

        arriendos.insert(arriendo);
    }

    emit setArriendos(arriendos);
}

void SpreadSheet::cargarArriendos(const Arriendos& arriendos)
{
    for(const auto& arriendo : arriendos)
    {
        agregarArriendo(arriendo);
    }
}

void SpreadSheet::cambiarTotales(std::size_t precioTotal, std::size_t IVATotal)
{
    item(rowCount() - 1, columnCount() - 3)->
         setText(QString::fromStdString('$' + std::to_string(precioTotal)));
    item(rowCount() - 1, columnCount() - 2)->
         setText(QString::fromStdString('$' + std::to_string(IVATotal)));
}

void SpreadSheet::guardarDocumento(const QString& fileName)
{
    QPrinter printer;

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    pintar(printer);
}

void SpreadSheet::imprimir()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);

    if(printDialog.exec() == QDialog::Accepted)
    {
        pintar(printer);
    }
}

void SpreadSheet::pintar(QPrinter& printer)
{
    QPainter painter;
    SpreadSheetPrinter spreadSheetPrinter(&painter, &printer);

    painter.begin(&printer);

    QVector<int> columnStretch;
    columnStretch << 5 << 5 << 5 << 5 << 5 << 5;

    QVector<QString> headers;
    headers << "Local" << "Nombre" << "Telefono" << "Correo" << "Precio" << "IVA";

    spreadSheetPrinter.setHeadersFont(QFont("Arial", 10, QFont::Bold));
    spreadSheetPrinter.printTable(model(), columnStretch, headers);

    painter.end();
}

void SpreadSheet::removerArriendo()
{
    const auto* button = qobject_cast<QPushButton*>(sender());

    for(int row = 0; row < rowCount(); ++row)
    {
        if(cellWidget(row, columnCount() - 1) == button)
        {
            emit arriendoRemovido(item(row, 0)->text().toStdString());

            removeRow(row);

            break;
        }
    }
}
