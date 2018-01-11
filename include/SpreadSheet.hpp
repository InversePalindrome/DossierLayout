/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheet.hpp
InversePalindrome.com
*/


#pragma once

#include "Arriendo.hpp"
#include "ArriendosList.hpp"

#include <QTableWidget>


class SpreadSheet : public QTableWidget
{
    Q_OBJECT

public:
    SpreadSheet(QWidget* parent);

    void agregarArriendo(const Arriendo& arriendo);

    void guardarArriendos();

public slots:
    void cargarArriendos(const Arriendos& arriendos);
    void cambiarTotales(std::size_t precioTotal, std::size_t IVATotal);

    void emprimir();

private slots:
    void removerArriendo();

signals:
    void setArriendos(const Arriendos& arriendos);
    void arriendoRemovido(const std::string& local);
};
