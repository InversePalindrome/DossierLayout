/*
Copyright (c) 2018 InversePalindrome
DossierLayout - WidgetHeader.hpp
InversePalindrome.com
*/


#pragma once

#include <QHeaderView>


class WidgetHeader : public QHeaderView
{
public:
    WidgetHeader(Qt::Orientation orientation, QWidget* parent);

private:
     virtual bool event(QEvent* event) override;

signals:
    void openHeaderMenu(const QPoint& postion);
};
