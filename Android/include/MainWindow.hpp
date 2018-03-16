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

#include <QLabel>
#include <QToolBar>
#include <QMainWindow>
#include <QGraphicsView>
#include <QStackedWidget>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& user);

public slots:
    void save();

private:
    QString user;

    QToolBar* toolBar;
    QGraphicsView* view;
    QStackedWidget* stackWidget;
    QLabel* titleIcon;
    QLabel* titleLabel;

    Hub* hub;

    void setupHubFunctions();
    void setupListFunctions(List* list);
    void setupTableFunctions(Table* table);
    void setupTreeFunctions(Tree* tree);

signals:
    void exit();
    void openSettings();
};
