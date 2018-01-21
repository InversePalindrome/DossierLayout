/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheetList.hpp
InversePalindrome.com
*/


#pragma once

#include "SpreadSheet.hpp"

#include <QMap>
#include <QString>


using SpreadSheets = QMap<QString, SpreadSheet*>;

class SpreadSheetList
{
public:
    SpreadSheetList(QWidget* parent);
    ~SpreadSheetList();

    void cargarSpreadSheets(const QString& usuario);

    void agregarSpreadSheet(const QString& nombre, SpreadSheet* spreadSheet);
    void removerSpreadSheet(const QString& nombre);

    SpreadSheet* operator[](const QString& nombre);
    SpreadSheet* operator[](const QString& nombre) const;

    SpreadSheets::iterator begin();
    SpreadSheets::iterator end();
    SpreadSheets::const_iterator cbegin() const;
    SpreadSheets::const_iterator cend() const;

private:
    QString usuario;
    SpreadSheets spreadSheets;

    QWidget* parent;
};
