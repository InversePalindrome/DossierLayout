/*
Copyright (c) 2018 InversePalindrome
Inverbienes - Arriendo.cpp
InversePalindrome.com
*/


#include "Arriendo.hpp"


Arriendo::Arriendo(const std::string& local, const std::string& nombre, const std::string& telefono,
          const std::string& correo, std::size_t precio, std::size_t IVA) :
    local(local),
    nombre(nombre),
    telefono(telefono),
    correo(correo),
    precio(precio),
    IVA(IVA)
{
}

std::string Arriendo::getLocal() const
{
    return local;
}

std::string Arriendo::getNombre() const
{
    return nombre;
}

std::string Arriendo::getTelefono() const
{
    return telefono;
}

std::string Arriendo::getCorreo() const
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

void Arriendo::setLocal(const std::string &local)
{
    this->local = local;
}

void Arriendo::setNombre(const std::string& nombre)
{
    this->nombre = nombre;
}

void Arriendo::setTelefono(const std::string& telefono)
{
    this->telefono = telefono;
}

void Arriendo::setCorreo(const std::string& correo)
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
