/*
Copyright (c) 2018 InversePalindrome
Inverbienes - MainWindow.cpp
InversePalindrome.com
*/


#include "MainWindow.hpp"

#include <QMenu>
#include <QBoxLayout>
#include <QGraphicsView>


MainWindow::MainWindow(ArriendosList& arriendos, int width, int height) :
    arriendos(arriendos),
    menuBar(new QMenuBar()),
    spreadSheet(new SpreadSheet(this)),
    agregarArriendo(new AgregarArriendoDialog(this, 1000, 900))
{
  setFixedSize(width, height);

  auto* archivo = new QMenu("Archivos", this);

  archivo->addAction("Guardar", [](){});
  archivo->addAction("Guardar Como", [](){});
  archivo->addAction("Imprimir", [](){});

  menuBar->addMenu(archivo);

  auto* arriendosMenu = new QMenu("Arriendos", this);

  arriendosMenu->addAction("Agregar", [this](){ agregarArriendo->open(); });

  menuBar->addMenu(arriendosMenu);

  setMenuBar(menuBar);

  auto* mainLayout = new QVBoxLayout(this);

  mainLayout->addWidget(spreadSheet);

  auto* view = new QGraphicsView(this);

  view->setLayout(mainLayout);

  setCentralWidget(view);

  QObject::connect(agregarArriendo, &AgregarArriendoDialog::agregarArriendo, &arriendos, &ArriendosList::agregarArriendo);
  QObject::connect(agregarArriendo, &AgregarArriendoDialog::agregarArriendo, spreadSheet, &SpreadSheet::agregarArriendo);
  QObject::connect(&arriendos, &ArriendosList::cambiaronTotales, spreadSheet, &SpreadSheet::cambiarTotales);
}
