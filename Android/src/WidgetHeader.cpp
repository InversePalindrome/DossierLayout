/*
Copyright (c) 2018 InversePalindrome
DossierLayout - WidgetHeader.cpp
InversePalindrome.com
*/


#include "WidgetHeader.hpp"

#include <QTapAndHoldGesture>


WidgetHeader::WidgetHeader(Qt::Orientation orientation, QWidget* parent) :
    QHeaderView(orientation, parent)
{
    grabGesture(Qt::TapAndHoldGesture);
}

bool WidgetHeader::event(QEvent* event)
{
    if(event->type() == QEvent::Gesture)
    {
        auto* gestureEvent = static_cast<QGestureEvent*>(event);

        if(auto* gesture = gestureEvent->gesture(Qt::TapAndHoldGesture))
        {
            const auto& position = mapFromGlobal(static_cast<QTapAndHoldGesture*>(gesture)->position().toPoint());

            if(logicalIndexAt(position))
            {
                emit customContextMenuRequested(position);
            }
        }
    }

    return QHeaderView::event(event);
}
