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


class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent);
    ~LoginDialog();

    void cargarUsuarios(const QString& fileName);

public slots:
     void agregarUsuario(const Usuario& usuario);

private:
    RegistrarDialog* registrarDialog;

    SimpleCrypt crypto;

    QString fileName;
    QMap<QString, QString> usuarios;

signals:
    void ingresoAceptado(const QString& usuario);
};
