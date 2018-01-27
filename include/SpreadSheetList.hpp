/*
Copyright (c) 2018 InversePalindrome
DossierTable - SpreadSheetList.hpp
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
    explicit SpreadSheetList(QWidget* parent);
    ~SpreadSheetList();

    void loadSpreadSheets(const QString& user);

    void addSpreadSheet(const QString& name, SpreadSheet* spreadSheet);
    void removeSpreadSheet(const QString& name);

    SpreadSheet* operator[](const QString& name);
    SpreadSheet* operator[](const QString& name) const;

    SpreadSheets::iterator begin();
    SpreadSheets::iterator end();
    SpreadSheets::const_iterator cbegin() const;
    SpreadSheets::const_iterator cend() const;

private:
    QString user;
    SpreadSheets spreadSheets;

    QWidget* parent;
};
