/*
Copyright (c) 2018 InversePalindrome
DossierTable - AlignmentUtility.cpp
InversePalindrome.com
*/


#include "AlignmentUtility.hpp"


QPair<QXlsx::Format::HorizontalAlignment, QXlsx::Format::VerticalAlignment>
Utility::QtToExcelAlignment(int alignment)
{
    switch(alignment)
    {
    case Qt::AlignLeft | Qt::AlignVCenter:
        return qMakePair(QXlsx::Format::AlignLeft, QXlsx::Format::AlignVCenter);
    case Qt::AlignRight | Qt::AlignVCenter:
        return qMakePair(QXlsx::Format::AlignRight, QXlsx::Format::AlignVCenter);
    case Qt::AlignTop | Qt::AlignHCenter:
        return qMakePair(QXlsx::Format::AlignHCenter, QXlsx::Format::AlignTop);
    case Qt::AlignBottom | Qt::AlignHCenter:
        return qMakePair(QXlsx::Format::AlignHCenter, QXlsx::Format::AlignBottom);
    case Qt::AlignCenter:
        return qMakePair(QXlsx::Format::AlignHCenter, QXlsx::Format::AlignVCenter);
    default:
        return qMakePair(QXlsx::Format::AlignHCenter, QXlsx::Format::AlignVCenter);
    }
}

Qt::Alignment Utility::ExcelToQtAlignment(QPair<QXlsx::Format::HorizontalAlignment,
QXlsx::Format::VerticalAlignment> alignment)
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
