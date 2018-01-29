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

    void insertCategory(QString categoria);
    void insertItem(QString item);

    void removeSelectedCategory();
    void removeSelectedItem();

    QString getSelectedSum() const;
    QString getSelectedAverage() const;
    QString getSelectedMin() const;
    QString getSelectedMax() const;
    QString getSelectedCount() const;

    void setDirectory(const QString& directory);

public slots:
    void categorySelected(int index);
    void itemSelected(int index);

private:
    QString directory;

    int selectedCategoryIndex;
    int selectedItemIndex;

    void saveToExcel(const QString& directory);

private slots:
    void openContextMenu(const QPoint& position);
};
