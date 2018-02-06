/*
Copyright (c) 2018 InversePalindrome
DossierTable - MainWindow.hpp
InversePalindrome.com
*/


#pragma once

#include "Table.hpp"
#include "Tree.hpp"

#include <QList>
#include <QString>
#include <QMenuBar>
#include <QToolBar>
#include <QTabWidget>
#include <QMainWindow>
#include <QGraphicsView>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    virtual void closeEvent(QCloseEvent* event) override;

    void load(const QString& user);

private:
    QGraphicsView* view;
    QMenuBar* menuBar;
    QToolBar* toolBar;
    QTabWidget* tabBar;

    QString user;
    QList<QMetaObject::Connection> connections;

    void setupTableFunctions(Table* table);
    void setupTreeFunctions(Tree* tree);

    bool dataStructureExists(const QString& name) const;

signals:
    void loadSpreadSheet(const QString& fileName);
    void saveSpreadSheet(const QString& fileName);
    void print();

    void insertColumn(const QString& columnName);
    void insertRow(const QString& rowName);

    void removeColumn();
    void removeRow();

    void getSum();
    void getAverage();
    void getMin();
    void getMax();
    void getCount();

    void sortColumn(Qt::SortOrder order);
    void sortRow(Qt::SortOrder order);

    void merge();
    void split();

    void exit();
};
