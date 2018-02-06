/*
Copyright (c) 2018 InversePalindrome
DossierTable - Table.hpp
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

public slots:
    void loadTable(const QString& fileName);
    void saveTable(const QString& fileName);

    void print();

    void insertColumn(QString columnName);
    void insertRow(QString rowName);

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

    int selectedColumn;
    int selectedRow;

    void saveToExcel(const QString& directory);

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
