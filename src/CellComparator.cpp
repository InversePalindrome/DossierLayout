/*
Copyright (c) 2018 InversePalindrome
DossierTable - CellComparator.cpp
InversePalindrome.com
*/


#include "CellComparator.hpp"


bool CellComparator::operator()(QTableWidgetItem* first, QTableWidgetItem* second)
{
    if(!first || !second)
    {
        return false;
    }

    bool isFirstOk, isSecondOk;

    auto firstNumber = first->text().toDouble(&isFirstOk);
    auto secondNumber = second->text().toDouble(&isSecondOk);

    if(isFirstOk && isSecondOk)
    {
        return firstNumber < secondNumber;
    }

    return first->text() < second->text();
}
