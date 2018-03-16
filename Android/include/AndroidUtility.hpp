/*
Copyright (c) 2018 InversePalindrome
DossierLayout - AndroidUtility.hpp
InversePalindrome.com
*/


#pragma once

#include <QDir>
#include <QString>


namespace Utility
{
    QDir appDir();
    QString appPath();

    void setRotation(int orientation);
}
