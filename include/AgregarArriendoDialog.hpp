/*
Copyright (c) 2018 InversePalindrome
Inverbienes - AgregarArriendoDialog.hpp
InversePalindrome.com
*/


#pragma once

#include "Arriendo.hpp"

#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>


class AgregarArriendoDialog : public QDialog
{
    Q_OBJECT

public:
    AgregarArriendoDialog(QWidget* parent, int width, int height);

private:
    QGridLayout* layout;
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
