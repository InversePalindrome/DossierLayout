/*
Copyright (c) 2018 InversePalindrome
Inverbienes - MainWindow.hpp
InversePalindrome.com
*/


#pragma once

#include "SpreadSheet.hpp"
#include "ArriendosList.hpp"
#include "AgregarArriendoDialog.hpp"

#include <QMenuBar>
#include <QMainWindow>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
MainWindow(ArriendosList& arriendos, int width, int height);

private:
ArriendosList& arriendos;

QMenuBar* menuBar;
SpreadSheet* spreadSheet;
AgregarArriendoDialog* agregarArriendo;
};
