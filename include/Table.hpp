/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Table.hpp
InversePalindrome.com
*/


#pragma once

#include <QTableWidget>
#include <QClipboard>


class Table : public QTableWidget
{
    Q_OBJECT

    using ItemList = QList<QTableWidgetItem*>;

public:
    Table(QWidget* parent, const QString& directory);
    ~Table();

    void load(const QString& fileName);
    void save(const QString& fileName);

    void print();

    void insertColumn(const QString& columnName);
    void insertRow(const QString& rowName);

    void removeColumn();
    void removeRow();

    void sortColumn(Qt::SortOrder order);
    void sortRow(Qt::SortOrder order);

    void merge();
    void split();

    double getSum();
    double getAverage();
    double getMin();
    double getMax();
    std::size_t getCount();

private:
    QString directory;
    QClipboard* clipboard;

    void saveToPdf(const QString& fileName);
    void saveToExcel(const QString& fileName);

    ItemList takeColumn(int column);
    ItemList takeRow(int row);

    void setColumn(int column, const ItemList& items);
    void setRow(int row, const ItemList& items);

    static bool compareCells(const QTableWidgetItem* first, const QTableWidgetItem* second);

private slots:
    void openHeaderMenu(const QPoint& position);
    void openCellsMenu(const QPoint& position);
    void editHeader(int logicalIndex);
};
