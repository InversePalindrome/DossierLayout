/*
Copyright (c) 2018 InversePalindrome
Inverbienes - MainWindow.hpp
InversePalindrome.com
*/


#pragma once

#include "SpreadSheet.hpp"
#include "ArriendosList.hpp"
#include "AgregarDialog.hpp"

#include <QMenuBar>
#include <QToolBar>
#include <QMainWindow>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ArriendosList& arriendos);

    void setUsuario(const QString& usuario);

private:
    QMenuBar* menuBar;
    QToolBar* toolBar;
    SpreadSheet* spreadSheet;
    AgregarDialog* agregarDialog;

    ArriendosList& arriendos;
    QString usuario;

signals:
    void guardarArriendos();
    void guardarDocumento(const QString& string);
    void imprimir();
    void salir();
};
