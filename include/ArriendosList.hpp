/*
Copyright (c) 2018 InversePalindrome
Inverbienes - ArriendosList.hpp
InversePalindrome.com
*/


#pragma once

#include "Arriendo.hpp"

#include <QObject>

#include <set>
#include <string>


struct CompareArriendo
{
    using is_transparent = void;

    bool operator()(const Arriendo& arriendo1, const Arriendo& arriendo2) const
    {
         return arriendo1.getLocal()< arriendo2.getLocal();
    }
    bool operator()(const std::string& local, const Arriendo& arriendo) const
    {
         return local < arriendo.getLocal();
    }
    bool operator()(const Arriendo& arriendo, const std::string& local) const
    {
         return arriendo.getLocal() < local;
    }
};

class ArriendosList : public QObject
{
    Q_OBJECT

public:
    ArriendosList(const std::string& fileName);
    ~ArriendosList();

public slots:
    void agregarArriendo(const Arriendo& arriendo);
    void removerArriendo(const std::string& local);

private:
    std::set<Arriendo, CompareArriendo> arriendos;
    std::string fileName;
    std::size_t precioTotal;
    std::size_t IVATotal;

signals:
    void cargarArriendos(const std::set<Arriendo, std::less<>>& arriendos);
    void cambiaronTotales(std::size_t precioTotal, std::size_t IVATotal);
};
