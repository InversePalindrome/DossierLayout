/*
Copyright (c) 2018 InversePalindrome
Inverbienes - RegistrarDialog.hpp
InversePalindrome.com
*/


#pragma once

#include "Usuario.hpp"

#include <QDialog>
#include <QLineEdit>


class RegistrarDialog : public QDialog
{
    Q_OBJECT

public:
    RegistrarDialog(QWidget* parent);

private:
    QLineEdit* usuarioEntry;
    QLineEdit* passwordEntry;
    QLineEdit* rePasswordEntry;

    virtual void closeEvent(QCloseEvent*) override;

signals:
    void registrarUsuario(const Usuario& usuario);
};
