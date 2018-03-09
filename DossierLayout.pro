#Copyright (c) 2018 InversePalindrome
#DossierLayout - DossierLayout.pro
#InversePalindrome.com


QT += core gui uitools xml printsupport xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DossierLayout
TEMPLATE = app
INCLUDEPATH += C:\Users\Bryan\Desktop\DossierLayout\DossierLayout\include
win32:RC_ICONS += DossierLayout.ico

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/AlignmentUtility.cpp \
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
    src/Users.cpp

HEADERS += \
    include/AlignmentUtility.hpp \
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
    include/Users.hpp

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

RESOURCES += \
    ../Resources/resources.qrc \
    ../Translations/translations.qrc \
    ../Styles/breeze.qrc

TRANSLATIONS = ../Translations/spanish.ts
