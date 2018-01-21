/*
Copyright (c) 2018 InversePalindrome
Inverbienes - MainWindow.cpp
InversePalindrome.com
*/


#include "MainWindow.hpp"

#include <QDir>
#include <QMenu>
#include <QBoxLayout>
#include <QGraphicsView>
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>


MainWindow::MainWindow() :
    menuBar(new QMenuBar(this)),
    toolBar(new QToolBar(this)),
    tabBar(new QTabWidget(this)),
    spreadSheets(this)
{
    setFixedSize(2048, 1536);
    setMenuBar(menuBar);
    addToolBar(toolBar);

    tabBar->setTabsClosable(true);
    tabBar->setFont(QFont("Arial", 11, QFont::Bold));
    tabBar->setStyleSheet("QTabBar::tab { min-width: 200px; min-height : 60px; }");

    auto* view = new QGraphicsView(this);

    setCentralWidget(view);
    centralWidget()->setLayout(new QVBoxLayout());
    centralWidget()->layout()->addWidget(tabBar);

    auto* archivo = new QMenu("Archivo", this);

    archivo->addAction("Guardar", [this]()
    {

    });
    archivo->addAction("Guardar Como", [this]()
    {

    });
    archivo->addAction("Guardar Todo", [this]()
    {

    });
    archivo->addAction("Imprimir", [this](){  });
    archivo->addAction("Salir", [this]()
    {
        emit salir();
    });

    menuBar->addMenu(archivo);

    auto* insertar = new QMenu("Insertar", this);

    insertar->addAction("Categoria", [this]()
    {
        bool ok;
        const auto& categoria = QInputDialog::getText(this, "Insertar Categoria", "Categoria", QLineEdit::Normal, "", &ok);

        if(ok)
        {
           auto* spreadSheet = spreadSheets[tabBar->tabText(tabBar->currentIndex())];

           if(spreadSheet)
           {
               spreadSheet->insertarCategoria(categoria);
           }
        }
    });
    insertar->addAction("Item", [this]()
    {
        bool ok;
        const auto& item = QInputDialog::getText(this, "Insertar Item", "Item", QLineEdit::Normal, "", &ok);

        if(ok)
        {
            auto* spreadSheet = spreadSheets[tabBar->tabText(tabBar->currentIndex())];

            if(spreadSheet)
            {
                spreadSheet->insertarItem(item);
            }
        }
    });

    menuBar->addMenu(insertar);

    auto* remover = new QMenu("Remover", this);

    remover->addAction("Categoria",
            [this]()
    {
        spreadSheets[tabBar->tabText(tabBar->currentIndex())]->removerCategoriaSeleccionada();
    });

    remover->addAction("Item",
            [this]()
    {
        spreadSheets[tabBar->tabText(tabBar->currentIndex())]->removerItemSeleccionado();
    });

    menuBar->addMenu(remover);

    toolBar->addAction(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogNewFolder)), "",
            [this]()
    {
        bool ok;
        const auto& nombre = QInputDialog::getText(this, "Agregar SpreadSheet", "Nombre", QLineEdit::Normal, "", &ok);

        if(ok)
        {
            auto* spreadSheet = new SpreadSheet(this);

            tabBar->addTab(spreadSheet, nombre);
            spreadSheets.agregarSpreadSheet(nombre, spreadSheet);

            QDir().mkdir(usuario + '/' + nombre);
        }
    });

    QObject::connect(tabBar, &QTabWidget::tabCloseRequested,
           [this](auto index)
    {
        const auto& nombre = tabBar->tabText(index);

        QMessageBox message(this);
        message.setText("Esta seguro de Remover " + nombre + " ?");
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        auto button = message.exec();

        switch(button)
        {
            case QMessageBox::Yes:
            tabBar->removeTab(index);
            spreadSheets.removerSpreadSheet(nombre);
            QDir(usuario + '/' + nombre).removeRecursively();
            break;
        }
    });
}

void MainWindow::cargarUsuario(const QString& usuario)
{
    this->usuario = usuario;

    spreadSheets.cargarSpreadSheets(usuario);

    for(auto spreadSheetItr = spreadSheets.cbegin(); spreadSheetItr != spreadSheets.cend(); ++spreadSheetItr)
    {
        tabBar->addTab(spreadSheetItr.value(), spreadSheetItr.key());
    }
}
