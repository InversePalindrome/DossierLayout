/*
Copyright (c) 2018 InversePalindrome
DossierLayout - AlignmentUtility.hpp
InversePalindrome.com
*/


#pragma once

#include "xlsxformat.h"

#include <Qt>
#include <QPair>


namespace Utility
{
   QPair<QXlsx::Format::HorizontalAlignment, QXlsx::Format::VerticalAlignment> QtToExcelAlignment(int alignment);

   Qt::Alignment ExcelToQtAlignment(const QPair<QXlsx::Format::HorizontalAlignment, QXlsx::Format::VerticalAlignment>& alignment);
}

