/*
Copyright (c) 2018 InversePalindrome
Inverbienes - SpreadSheet.hpp
InversePalindrome.com
*/


#pragma once

#include "Arriendo.hpp"

#include <QTableWidget>


class SpreadSheet : public QTableWidget
{
public:
    SpreadSheet(QWidget* parent);

    void agregarArriendo(const Arriendo& arriendo);

public slots:
    void cambiarTotales(std::size_t precioTotal, std::size_t IVATotal);

private slots:
    void removerArriendo();

signals:
    void arriendoRemovido(const std::string& local);
};
