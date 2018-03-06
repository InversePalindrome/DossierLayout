/*
Copyright (c) 2018 InversePalindrome
DossierLayout - AndroidUtility.cpp
InversePalindrome.com
*/


#include "AndroidUtility.hpp"

#include <QtAndroid>
#include <QAndroidJniObject>


void Utility::setRotation(int orientation)
{
    QAndroidJniObject activity(QtAndroid::androidActivity());

    activity.callMethod<void>("setRequestedOrientation", "(I)V", orientation);
}
