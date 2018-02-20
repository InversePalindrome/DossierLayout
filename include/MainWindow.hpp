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
#include <QLineEdit>
#include <QTabWidget>
#include <QMainWindow>
#include <QGraphicsView>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& user);
    ~MainWindow();

    void load(const QString& user);
    void save();

private:
    QGraphicsView* view;
    QMenuBar* menuBar;
    QToolBar* toolBar;
    QTabWidget* tabBar;
    QLineEdit* searchBar;

    QString user;
    QList<QMetaObject::Connection> connections;

    void setupListFunctions(const List* list);
    void setupTableFunctions(const Table* table);
    void setupTreeFunctions(const Tree* tree);

    void updateSearchBar();

    void addTab(const QString& type, const QString& name);

    bool dataStructureExists(const QString& name) const;

private slots:
    void findTab(const QString& tabName);

signals:
    void loadDataStructure(const QString& fileName);
    void saveDataStructure(const QString& fileName);
    void print();
    void exit();

    void insertElement(const QString& name, Qt::ItemFlags flags);
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
};
