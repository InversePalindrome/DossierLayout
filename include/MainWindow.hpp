/*
Copyright (c) 2018 InversePalindrome
DossierLayout - MainWindow.hpp
InversePalindrome.com
*/


#pragma once

#include "Tree.hpp"
#include "List.hpp"
#include "Table.hpp"

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

    void load(const QString& user);

private:
    QGraphicsView* view;
    QMenuBar* menuBar;
    QToolBar* toolBar;
    QTabWidget* tabBar;

    QString user;
    QList<QMetaObject::Connection> connections;

    virtual void closeEvent(QCloseEvent* event) override;

    void setupListFunctions(List* list);
    void setupTableFunctions(Table* table);
    void setupTreeFunctions(Tree* tree);

    bool dataStructureExists(const QString& name) const;

signals:
    void loadDataStructure(const QString& fileName);
    void saveDataStructure(const QString& fileName);
    void print();

    void insertElement(const QString& name);
    void insertColumn(const QString& name);
    void insertRow(const QString& name);
    void insertNode(const QString& name);

    void removeElement();
    void removeColumn();
    void removeRow();
    void removeNode();

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
