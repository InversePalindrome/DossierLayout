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
    tabBar(new QTabWidget(this))
{
    setFixedSize(2048, 1536);
    setMenuBar(menuBar);
    addToolBar(toolBar);

    tabBar->setTabsClosable(true);
    tabBar->setFont(QFont("Arial", 11, QFont::Bold));
    tabBar->setStyleSheet("QTabBar::tab { min-width: 200px; min-height : 60px; }");

    auto* archivo = new QMenu("Archivos", this);

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
        auto categoria = QInputDialog::getText(this, "Insertar Categoria", "Categoria", QLineEdit::Normal, "", &ok);

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
        auto item = QInputDialog::getText(this, "Insertar Item", "Item", QLineEdit::Normal, "", &ok);

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

    toolBar->addAction(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogNewFolder)), "",
            [this]()
    {
        bool ok;
        auto tabName = QInputDialog::getText(this, "Agregar SpreadSheet", "Nombre", QLineEdit::Normal, "", &ok);

        if(ok)
        {
            auto* spreadSheet = new SpreadSheet(this);

            tabBar->addTab(spreadSheet, tabName);
            spreadSheets.insert(tabName, spreadSheet);

            QDir().mkdir(usuario + '/' + tabName);
        }
    });

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabBar);

    auto* view = new QGraphicsView(this);
    view->setLayout(mainLayout);
    setCentralWidget(view);

    QObject::connect(tabBar, &QTabWidget::tabCloseRequested,
           [this](auto index)
    {
        const auto& spreadSheetName = tabBar->tabText(index);

        QMessageBox message(this);
        message.setText("Esta seguro de Remover " + spreadSheetName + " ?");
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        auto button = message.exec();

        switch(button)
        {
            case QMessageBox::Yes:
            tabBar->removeTab(index);
            QDir(usuario + '/' + spreadSheetName).removeRecursively();
            break;
        }
    });
}

void MainWindow::setUsuario(const QString& usuario)
{
    this->usuario = usuario;
}
