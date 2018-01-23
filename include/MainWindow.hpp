/*
Copyright (c) 2018 InversePalindrome
Inverbienes - MainWindow.hpp
InversePalindrome.com
*/


#pragma once

#include "SpreadSheetList.hpp"

#include <QString>
#include <QMenuBar>
#include <QToolBar>
#include <QTabWidget>
#include <QMainWindow>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void loadUser(const QString& user);

private:
    QMenuBar* menuBar;
    QToolBar* toolBar;
    QTabWidget* tabBar;

    QString user;
    SpreadSheetList spreadSheets;

    SpreadSheet* getCurrentSpreadSheet();

signals:
    void exit();
};
