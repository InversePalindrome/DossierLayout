/*
Copyright (c) 2018 InversePalindrome
DossierLayout - AndroidUtility.cpp
InversePalindrome.com
*/


#include "AndroidUtility.hpp"

#include <QtAndroid>
#include <QStandardPaths>
#include <QAndroidJniObject>


QDir Utility::appDir()
{
    return QDir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
}

QString Utility::appPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + '/';
}

void Utility::setRotation(int orientation)
{
    QAndroidJniObject activity(QtAndroid::androidActivity());

    activity.callMethod<void>("setRequestedOrientation", "(I)V", orientation);
}
