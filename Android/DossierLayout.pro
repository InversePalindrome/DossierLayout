#Copyright (c) 2018 InversePalindrome
#DossierLayout - DossierLayout.pro
#InversePalindrome.com


QT += core gui uitools xml printsupport androidextras
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DossierLayout
TEMPLATE = app
INCLUDEPATH += C:\Users\Bryan\Desktop\DossierLayout\include

win32:RC_ICONS += DossierLayout.ico

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ../src/Application.cpp \
    ../src/List.cpp \
    ../src/Main.cpp \
    ../src/MainWindow.cpp \
    ../src/SimpleCrypt.cpp \
    ../src/Table.cpp \
    ../src/Tree.cpp \
    ../src/Users.cpp \
    ../src/RegisterDialog.cpp \
    ../src/LoginDialog.cpp \
    ../src/SettingsDialog.cpp \
    ../src/Hub.cpp \
    ../src/AndroidUtility.cpp

HEADERS += \
    ../include/Application.hpp \
    ../include/List.hpp \
    ../include/MainWindow.hpp \
    ../include/SimpleCrypt.hpp \
    ../include/Table.hpp \
    ../include/Tree.hpp \
    ../include/Users.hpp \
    ../include/RegisterDialog.hpp \
    ../include/LoginDialog.hpp \
    ../include/SettingsDialog.hpp \
    ../include/Hub.hpp \
    ../include/AndroidUtility.hpp

SUBDIRS += \
    DossierLayout.pro

DISTFILES += \
    ../Resources/InversePalindromeLogo.jpg \
    ../Resources/AddRow.png \
    ../Resources/Download.png \
    ../Resources/Exit.png \
    ../Resources/List.png \
    ../Resources/LoginTitle.png \
    ../Resources/Merge.png \
    ../Resources/Open.png \
    ../Resources/Print.png \
    ../Resources/RemoveColumn.png \
    ../Resources/RemoveNode.png \
    ../Resources/RemoveRow.png \
    ../Resources/Sigma.png \
    ../Resources/Sort.png \
    ../Resources/Split.png \
    ../Resources/Table.png \
    ../Resources/Tree.png \
    ../src/LoginDialog

RESOURCES += \
    ../Resources/resources.qrc \
    ../Translations/translations.qrc \
    ../Styles/breeze.qrc

TRANSLATIONS = ../Translations/spanish.ts
