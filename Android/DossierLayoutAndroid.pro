#Copyright (c) 2018 InversePalindrome
#DossierLayout - DossierLayoutAndroid.pro
#InversePalindrome.com


QT += core gui uitools xml printsupport androidextras
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DossierLayout
TEMPLATE = app

INCLUDEPATH += C:\Users\Bryan\Desktop\DossierLayout\DossierLayoutAndroid\include

win32:RC_ICONS += DossierLayout.ico


DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
    src/AndroidUtility.cpp \
    src/Application.cpp \
    src/Hub.cpp \
    src/List.cpp \
    src/LoginDialog.cpp \
    src/Main.cpp \
    src/MainWindow.cpp \
    src/RegisterDialog.cpp \
    src/SettingsDialog.cpp \
    src/SimpleCrypt.cpp \
    src/Table.cpp \
    src/Tree.cpp \
    src/Users.cpp \
    src/WidgetHeader.cpp

HEADERS += \
    include/AndroidUtility.hpp \
    include/Application.hpp \
    include/Hub.hpp \
    include/List.hpp \
    include/LoginDialog.hpp \
    include/MainWindow.hpp \
    include/RegisterDialog.hpp \
    include/SettingsDialog.hpp \
    include/SimpleCrypt.hpp \
    include/Table.hpp \
    include/Tree.hpp \
    include/Users.hpp \
    include/WidgetHeader.hpp

RESOURCES += \
    ../Resources/resources.qrc \
    ../Translations/translations.qrc \
    ../Styles/breeze.qrc

TRANSLATIONS = ../Translations/spanish.ts
