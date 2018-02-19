/*
Copyright (c) 2018 InversePalindrome
DossierLayout - AlignmentUtility.hpp
InversePalindrome.com
*/


#pragma once

#include <Qt>
#include <QPair>

#include <QtXlsx/xlsxformat.h>


namespace Utility
{
    QPair<QXlsx::Format::HorizontalAlignment, QXlsx::Format::VerticalAlignment> QtToExcelAlignment(int alignment);

    Qt::Alignment ExcelToQtAlignment(const QPair<QXlsx::Format::HorizontalAlignment, QXlsx::Format::VerticalAlignment>& alignment);
}
