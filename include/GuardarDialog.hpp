/*
Copyright (c) 2018 InversePalindrome
Inverbienes - GuardarDialog.hpp
InversePalindrome.com
*/


#pragma once

#include <QFileDialog>


class GuardarDialog : public QFileDialog
{
    Q_OBJECT

public:
    GuardarDialog(QWidget* parent);
};
