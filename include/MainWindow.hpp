/*
Copyright (c) 2018 InversePalindrome
DossierTable - MainWindow.hpp
InversePalindrome.com
*/


#pragma once

#include "SpreadSheetList.hpp"

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

    void loadUser(const QString& user);

private:
    QGraphicsView* view;
    QMenuBar* menuBar;
    QToolBar* toolBar;
    QTabWidget* tabBar;

    QString user;
    SpreadSheetList spreadSheets;

    SpreadSheet* getCurrentSpreadSheet();

signals:
    void exit();
};
