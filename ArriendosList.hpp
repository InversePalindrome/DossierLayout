/*
Copyright (c) 2018 InversePalindrome
Inverbienes - ArriendosList.hpp
InversePalindrome.com
*/


#pragma once

#include "Arriendo.hpp"

#include <QObject>
#include <QString>

#include <set>
#include <string>


struct CompareArriendo
{
    using is_transparent = void;

    bool operator()(const Arriendo& arriendo1, const Arriendo& arriendo2) const
    {
         return arriendo1.getLocal()< arriendo2.getLocal();
    }
    bool operator()(const QString& local, const Arriendo& arriendo) const
    {
         return local < arriendo.getLocal();
    }
    bool operator()(const Arriendo& arriendo, const QString& local) const
    {
         return arriendo.getLocal() < local;
    }
};

using Arriendos = std::set<Arriendo, CompareArriendo>;
Q_DECLARE_METATYPE(Arriendos)

class ArriendosList : public QObject
{
    Q_OBJECT

public:
    ArriendosList();
    ArriendosList(const std::string& fileName);
    ~ArriendosList();

    void cargarArriendos(const std::string& fileName);

public slots:
    void agregarArriendo(const Arriendo& arriendo);
    void removerArriendo(const QString& local);

    void setArriendos(const Arriendos& arriendos);

private:
    Arriendos arriendos;
    std::string fileName;
    std::size_t precioTotal;
    std::size_t IVATotal;

signals:
    void enviarArriendos(const Arriendos& arriendos);
    void cambiaronTotales(std::size_t precioTotal, std::size_t IVATotal);
};
