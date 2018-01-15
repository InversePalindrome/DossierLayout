/*
Copyright (c) 2018 InversePalindrome
Inverbienes - AgregarDialog.hpp
InversePalindrome.com
*/


#pragma once

#include "Arriendo.hpp"

#include <QDialog>
#include <QLineEdit>


class AgregarDialog : public QDialog
{
    Q_OBJECT

public:
    AgregarDialog(QWidget* parent);

private:
    QLineEdit* localEntry;
    QLineEdit* nombreEntry;
    QLineEdit* telefonoEntry;
    QLineEdit* correoEntry;
    QLineEdit* precioEntry;
    QLineEdit* IVAEntry;

    void agregoArriendo();

signals:
    void agregarArriendo(const Arriendo& arriendo);
};
