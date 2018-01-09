/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheet.cpp
InversePalindrome.com
*/


#include "SpreadSheet.hpp"

#include <QFont>
#include <QStyle>
#include <QHeaderView>
#include <QPushButton>
#include <QApplication>
#include <QTableWidgetItem>


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

    auto* precioItem = new QTableWidgetItem();
    precioItem->setTextColor(QColor(0, 255, 0));
    precioItem->setFont(font);
    precioItem->setFlags(precioItem->flags() & ~Qt::ItemIsEditable);

    auto* IVAItem = new QTableWidgetItem();
    IVAItem->setTextColor(QColor(255, 0, 0));
    IVAItem->setFont(font);
    IVAItem->setFlags(IVAItem->flags() & ~Qt::ItemIsEditable);

    setItem(rowCount() - 1, columnCount() - 3, precioItem);
    setItem(rowCount() - 1, columnCount() - 2, IVAItem);
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

void SpreadSheet::cambiarTotales(std::size_t precioTotal, std::size_t IVATotal)
{
    item(rowCount() - 1, columnCount() - 3)->
         setText(QString::fromStdString('$' + std::to_string(precioTotal)));
    item(rowCount() - 1, columnCount() - 2)->
         setText(QString::fromStdString('$' + std::to_string(IVATotal)));
}

void SpreadSheet::removerArriendo()
{
    auto* button = qobject_cast<QPushButton*>(sender());

    for(int row = 0; row < rowCount(); ++row)
    {
        if(cellWidget(row, columnCount() - 1) == button)
        {
            removeRow(row);
        }
    }
}
