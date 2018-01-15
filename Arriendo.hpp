/*
Copyright (c) 2018 InversePalindrome
Inverbienes - Arriendo.hpp
InversePalindrome.com
*/


#pragma once

#include <QString>

#include <cstddef>


class Arriendo
{
public:
    Arriendo();
    Arriendo(const QString& local, const QString& nombre, const QString& telefono, const QString& correo,
             std::size_t precio, std::size_t IVA);

    QString getLocal() const;
    QString getNombre() const;
    QString getTelefono() const;
    QString getCorreo() const;
    std::size_t getPrecio() const;
    std::size_t getIVA() const;

    void setLocal(const QString& local);
    void setNombre(const QString& nombre);
    void setTelefono(const QString& telefono);
    void setCorreo(const QString& correo);
    void setPrecio(std::size_t precio);
    void setIVA(std::size_t precio);

private:
    QString local;
    QString nombre;
    QString telefono;
    QString correo;
    std::size_t precio;
    std::size_t IVA;
};
