#Copyright (c) 2018 InversePalindrome
#DossierTable - DossierTable.pro
#InversePalindrome.com


QT += core gui xml printsupport xlsx

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DossierTable
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    MainWindow.cpp \
    Main.cpp \
    Application.cpp \
    LoginDialog.cpp \
    Main.cpp \
    RegisterDialog.cpp \
    SimpleCrypt.cpp \
    SpreadSheet.cpp \
    SpreadSheetList.cpp

HEADERS += \
    MainWindow.hpp \
    Application.hpp \
    LoginDialog.hpp \
    RegisterDialog.hpp \
    SimpleCrypt.hpp \
    SpreadSheet.hpp \
    SpreadSheetList.hpp \
    User.hpp

FORMS += \
        MainWIndow.ui

RESOURCES += \
    resources.qrc
