/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheet.hpp
InversePalindrome.com
*/


#pragma once

#include <QString>
#include <QTableWidget>


class SpreadSheet : public QTableWidget
{
    Q_OBJECT

public:
    explicit SpreadSheet(QWidget* parent);
    ~SpreadSheet();

    void loadSpreadSheet(const QString& fileName);
    void saveSpreadSheet(const QString& fileName);

    void print();

    void insertCategory(QString categoria);
    void insertItem(QString item);

    void removeSelectedCategory();
    void removeSelectedItem();

    void setFileName(const QString& fileName);

public slots:
    void categorySelcted(int index);
    void itemSelected(int index);

private:
    QString fileName;

    int selectedCategoryIndex;
    int selectedItemIndex;

    void saveToExcel(const QString& fileName);
};
