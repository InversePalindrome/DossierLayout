#Copyright (c) 2018 InversePalindrome
#DossierLayout - DossierLayout.pro
#InversePalindrome.com


QT += widgets printsupport xml

TARGET = DossierLayout
TEMPLATE = app
INCLUDEPATH += $$PWD/include
include(Qtxlsx/src/xlsx/qtxlsx.pri)

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

RESOURCES += \
    ../Resources/resources.qrc \
    ../Translations/translations.qrc \
    ../Styles/breeze.qrc \
    qt_conf.qrc

TRANSLATIONS = ../Translations/spanish.ts
