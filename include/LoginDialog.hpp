/*
Copyright (c) 2018 InversePalindrome
Inverbienes - LoginDialog.hpp
InversePalindrome.com
*/


#pragma once

#include "Usuario.hpp"
#include "SimpleCrypt.hpp"
#include "RegistrarDialog.hpp"

#include <QMap>
#include <QDialog>
#include <QString>

#include <string>


class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget* parent);
    ~LoginDialog();

public slots:
     void agregarUsuario(const Usuario& usuario);

private:
    RegistrarDialog* registrarDialog;

    SimpleCrypt crypto;

    std::string fileName;
    QMap<QString, QString> usuarios;

    void cargarUsuarios(const std::string& fileName);

signals:
    void ingresoAceptado(const std::string& usuario);
};
