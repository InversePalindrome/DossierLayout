/*
Copyright (c) 2018 InversePalindrome
DossierTable - AlignmentUtility.hpp
InversePalindrome.com
*/


#pragma once

#include <Qt>
#include <QPair>

#include <QtXlsx/xlsxformat.h>


namespace Utility
{
    QPair<QXlsx::Format::HorizontalAlignment, QXlsx::Format::VerticalAlignment> QtToExcelAlignment(int alignment);

    Qt::Alignment ExcelToQtAlignment(QPair<QXlsx::Format::HorizontalAlignment, QXlsx::Format::VerticalAlignment> alignment);
}
