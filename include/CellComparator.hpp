/*
Copyright (c) 2018 InversePalindrome
DossierTable - CellComparator.hpp
InversePalindrome.com
*/


#pragma once

#include <QTableWidgetItem>


struct CellComparator
{
    bool operator()(QTableWidgetItem* first, QTableWidgetItem* second) const;
};
