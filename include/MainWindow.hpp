/*
Copyright (c) 2018 InversePalindrome
Inverbienes - MainWindow.hpp
InversePalindrome.com
*/


#pragma once

#include "SpreadSheet.hpp"

#include <QMenuBar>
#include <QToolBar>
#include <QTabWidget>
#include <QMainWindow>

#include <unordered_map>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();

    void setUsuario(const QString& usuario);

private:
    QMenuBar* menuBar;
    QToolBar* toolBar;
    QTabWidget* tabBar;

    QString usuario;
    std::unordered_map<QString, SpreadSheet*> spreadSheets;

signals:
    void salir();
};
