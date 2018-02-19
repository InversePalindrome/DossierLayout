/*
Copyright (c) 2018 InversePalindrome
DossierLayout - AlignmentUtility.cpp
InversePalindrome.com
*/


#include "AlignmentUtility.hpp"


QPair<QXlsx::Format::HorizontalAlignment, QXlsx::Format::VerticalAlignment>
Utility::QtToExcelAlignment(const int alignment)
{
    if(alignment == (Qt::AlignLeft | Qt::AlignVCenter))
    {
        return qMakePair(QXlsx::Format::AlignLeft, QXlsx::Format::AlignVCenter);
    }
    else if(alignment == (Qt::AlignRight | Qt::AlignVCenter))
    {
        return qMakePair(QXlsx::Format::AlignRight, QXlsx::Format::AlignVCenter);
    }
    else if(alignment == (Qt::AlignTop | Qt::AlignHCenter))
    {
        return qMakePair(QXlsx::Format::AlignHCenter, QXlsx::Format::AlignTop);
    }
    else if(alignment == (Qt::AlignBottom | Qt::AlignHCenter))
    {
        return qMakePair(QXlsx::Format::AlignHCenter, QXlsx::Format::AlignBottom);
    }
    else if(alignment == Qt::AlignCenter)
    {
        return qMakePair(QXlsx::Format::AlignHCenter, QXlsx::Format::AlignVCenter);
    }

    return qMakePair(QXlsx::Format::AlignHCenter, QXlsx::Format::AlignVCenter);
}

Qt::Alignment Utility::ExcelToQtAlignment(const QPair<QXlsx::Format::HorizontalAlignment,
QXlsx::Format::VerticalAlignment>& alignment)
{
    if(alignment.first == QXlsx::Format::AlignLeft && alignment.second == QXlsx::Format::AlignVCenter)
    {
        return Qt::AlignLeft | Qt::AlignVCenter;
    }
    else if(alignment.first == QXlsx::Format::AlignRight && alignment.second == QXlsx::Format::AlignVCenter)
    {
        return Qt::AlignRight | Qt::AlignVCenter;
    }
    else if(alignment.first == QXlsx::Format::AlignHCenter && alignment.second == QXlsx::Format::AlignTop)
    {
        return Qt::AlignTop | Qt::AlignHCenter;
    }
    else if(alignment.first == QXlsx::Format::AlignHCenter && alignment.second == QXlsx::Format::AlignBottom)
    {
        return Qt::AlignBottom | Qt::AlignHCenter;
    }
    else if(alignment.first == QXlsx::Format::AlignHCenter && alignment.second == QXlsx::Format::AlignVCenter)
    {
        return Qt::AlignCenter;
    }

    return Qt::AlignCenter;
}
