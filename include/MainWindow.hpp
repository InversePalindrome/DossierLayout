/*
Copyright (c) 2018 InversePalindrome
DossierLayout - MainWindow.hpp
InversePalindrome.com
*/


#pragma once

#include "Hub.hpp"
#include "List.hpp"
#include "Tree.hpp"
#include "Table.hpp"

#include <QMenuBar>
#include <QToolBar>
#include <QBoxLayout>
#include <QMainWindow>
#include <QGraphicsView>
#include <QStackedWidget>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& user);

private:
    QString user;

    QMenuBar* menuBar;
    QToolBar* toolBar;
    QStackedWidget* stackWidget;

    QGraphicsView* view;
    QVBoxLayout* centralLayout;

    void setupHubFunctions(Hub* hub);
    void setupListFunctions(List* list);
    void setupTableFunctions(Table* table);
    void setupTreeFunctions(Tree* tree);

signals:
    void exit();
};
