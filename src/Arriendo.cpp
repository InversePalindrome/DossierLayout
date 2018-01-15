/*
Copyright (c) 2018 InversePalindrome
Inverbienes - Arriendo.cpp
InversePalindrome.com
*/


#include "Arriendo.hpp"


Arriendo::Arriendo() :
    precio(0u),
    IVA(0u)
{
}

Arriendo::Arriendo(const QString& local, const QString& nombre, const QString& telefono,
        const QString& correo, std::size_t precio, std::size_t IVA) :
    local(local),
    nombre(nombre),
    telefono(telefono),
    correo(correo),
    precio(precio),
    IVA(IVA)
{
}

QString Arriendo::getLocal() const
{
    return local;
}

QString Arriendo::getNombre() const
{
    return nombre;
}

QString Arriendo::getTelefono() const
{
    return telefono;
}

QString Arriendo::getCorreo() const
{
    return correo;
}

std::size_t Arriendo::getPrecio() const
{
    return precio;
}

std::size_t Arriendo::getIVA() const
{
    return IVA;
}

void Arriendo::setLocal(const QString &local)
{
    this->local = local;
}

void Arriendo::setNombre(const QString& nombre)
{
    this->nombre = nombre;
}

void Arriendo::setTelefono(const QString& telefono)
{
    this->telefono = telefono;
}

void Arriendo::setCorreo(const QString& correo)
{
    this->correo = correo;
}

void Arriendo::setPrecio(std::size_t precio)
{
    this->precio = precio;
}

void Arriendo::setIVA(std::size_t IVA)
{
    this->IVA = IVA;
}
