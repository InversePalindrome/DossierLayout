/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheet.hpp
InversePalindrome.com
*/


#pragma once

#include <QString>
#include <QPrinter>
#include <QTableWidget>


class SpreadSheet : public QTableWidget
{
    Q_OBJECT

public:
    explicit SpreadSheet(QWidget* parent);
    ~SpreadSheet();

    void cargarSpreadSheet(const QString& fileName);

    void insertarCategoria(QString categoria);
    void insertarItem(QString item);

private:
    QString fileName;

    void pintar(QPrinter& printer);
};
