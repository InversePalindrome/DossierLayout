/*
Copyright (c) 2018 InversePalindrome
DossierTable - SpreadSheet.hpp
InversePalindrome.com
*/


#pragma once

#include <QString>
#include <QTableWidget>


class SpreadSheet : public QTableWidget
{
    Q_OBJECT

    using ItemList = QList<QTableWidgetItem*>;

public:
    SpreadSheet(QWidget* parent, const QString& directory);
    ~SpreadSheet();

    void loadSpreadSheet(const QString& fileName);
    void saveSpreadSheet(const QString& fileName);

    void print();

    void insertColumn(QString columnName);
    void insertRow(QString rowName);

    void removeColumn();
    void removeRow();

    void sortSelectedColumn(Qt::SortOrder order);
    void sortSelectedRow(Qt::SortOrder order);

    void mergeSelected();
    void splitSelected();

    QString getSelectedSum() const;
    QString getSelectedAverage() const;
    QString getSelectedMin() const;
    QString getSelectedMax() const;
    QString getSelectedCount() const;

    void setDirectory(const QString& directory);

public slots:
    void columnSelected(int index);
    void rowSelected(int index);

private:
    QString directory;

    int selectedColumnIndex;
    int selectedRowIndex;

    void saveToExcel(const QString& directory);

    ItemList takeColumn(int column);
    ItemList takeRow(int row);

    void setColumn(int column, const ItemList& items);
    void setRow(int row, const ItemList& items);

private slots:
    void openHeaderMenu(const QPoint& position);
    void openCellMenu(const QPoint& position);
    void editHeader(int logicalIndex);
};
