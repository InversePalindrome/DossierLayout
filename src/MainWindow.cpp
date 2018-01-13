/*
Copyright (c) 2018 InversePalindrome
Inverbienes - MainWindow.cpp
InversePalindrome.com
*/


#include "MainWindow.hpp"

#include <QMenu>
#include <QBoxLayout>
#include <QApplication>
#include <QGraphicsView>
#include <QFileDialog>


MainWindow::MainWindow(ArriendosList& arriendos) :
    menuBar(new QMenuBar(this)),
    toolBar(new QToolBar(this)),
    spreadSheet(new SpreadSheet(this)),
    agregarDialog(new AgregarDialog(this)),
    arriendos(arriendos)
{
    setFixedSize(2048, 1536);

    auto* archivo = new QMenu("Archivos", this);

    archivo->addAction("Guardar", [this]()
    {
        emit guardarArriendos();
        emit guardarDocumento("Arriendos.pdf");
    });
    archivo->addAction("Guardar Como", [this]()
    {
        emit guardarArriendos();
        emit guardarDocumento(QFileDialog::getSaveFileName
             (this, "Agregue o cree un documento.", "", "Documents (*.pdf)"));
    });

    archivo->addAction("Imprimir", [this](){ emit imprimir(); });

    menuBar->addMenu(archivo);
    setMenuBar(menuBar);

    toolBar->addAction(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogNewFolder)), "",
                     [this](){ agregarDialog->open(); });

    addToolBar(toolBar);

    auto* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(spreadSheet);

    auto* view = new QGraphicsView(this);
    view->setLayout(mainLayout);

    setCentralWidget(view);

    QObject::connect(this, &MainWindow::guardarDocumento, spreadSheet, &SpreadSheet::guardarDocumento);
    QObject::connect(this, &MainWindow::imprimir, spreadSheet, &SpreadSheet::imprimir);
    QObject::connect(this, &MainWindow::guardarArriendos, spreadSheet, &SpreadSheet::guardarArriendos);
    QObject::connect(agregarDialog, &AgregarDialog::agregarArriendo, &arriendos, &ArriendosList::agregarArriendo);
    QObject::connect(agregarDialog, &AgregarDialog::agregarArriendo, spreadSheet, &SpreadSheet::agregarArriendo);
    QObject::connect(&arriendos, &ArriendosList::enviarArriendos, spreadSheet, &SpreadSheet::cargarArriendos);
    QObject::connect(&arriendos, &ArriendosList::cambiaronTotales, spreadSheet, &SpreadSheet::cambiarTotales);
    QObject::connect(spreadSheet, &SpreadSheet::arriendoRemovido, &arriendos, &ArriendosList::removerArriendo);
    QObject::connect(spreadSheet, &SpreadSheet::setArriendos, &arriendos, &ArriendosList::setArriendos);

    arriendos.cargarArriendos("Arriendos.xml");
}
