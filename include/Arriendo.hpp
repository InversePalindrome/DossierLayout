/*
Copyright (c) 2018 InversePalindrome
Inverbienes - Arriendo.hpp
InversePalindrome.com
*/


#pragma once

#include <string>
#include <cstddef>


class Arriendo
{
public:
    Arriendo();
    Arriendo(const std::string& local, const std::string& nombre, const std::string& telefono, const std::string& correo,
             std::size_t precio, std::size_t IVA);

    std::string getLocal() const;
    std::string getNombre() const;
    std::string getTelefono() const;
    std::string getCorreo() const;
    std::size_t getPrecio() const;
    std::size_t getIVA() const;

    void setLocal(const std::string& local);
    void setNombre(const std::string& nombre);
    void setTelefono(const std::string& telefono);
    void setCorreo(const std::string& correo);
    void setPrecio(std::size_t precio);
    void setIVA(std::size_t precio);

private:
    std::string local;
    std::string nombre;
    std::string telefono;
    std::string correo;
    std::size_t precio;
    std::size_t IVA;
};
