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

    archivo->addAction("Abrir", [this]()
    {
        getCurrentSpreadSheet()->loadSpreadSheet(QFileDialog::
           getOpenFileName(this, "Abrir", "", "Excel (*.xlsx)"));
    });
    archivo->addAction("Guardar Como", [this]()
    {
        getCurrentSpreadSheet()->saveSpreadSheet(QFileDialog::
          getSaveFileName(this, "Guardar Como", "", "SpreadSheet (*.pdf .xlsx)"));
    });
    archivo->addAction("Imprimir", [this]()
    {
        if(getCurrentSpreadSheet())
        {
            getCurrentSpreadSheet()->print();
        }
    });
    archivo->addAction("Salir", [this]()
    {
        emit exit();
    });

    menuBar->addMenu(archivo);

    auto* insertar = new QMenu("Insertar", this);

    insertar->addAction("Categoria", [this]()
    {
        bool ok;
        const auto& category = QInputDialog::getText(this, "Insertar Categoria", "Categoria", QLineEdit::Normal, "", &ok);

        if(ok && getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->insertCategory(category);
        }
    });
    insertar->addAction("Item", [this]()
    {
        bool ok;
        const auto& item = QInputDialog::getText(this, "Insertar Item", "Item", QLineEdit::Normal, "", &ok);

        if(ok && getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->insertItem(item);
        }
    });

    menuBar->addMenu(insertar);

    auto* remove = new QMenu("Remover", this);

    remove->addAction("Categoria",
            [this]()
    {
        if(getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->removeSelectedCategory();
        }
    });

    remove->addAction("Item",
            [this]()
    {
        if(getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->removeSelectedItem();
        }
    });

    menuBar->addMenu(remove);

    toolBar->addAction(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogNewFolder)), "",
            [this]()
    {
        bool ok;
        const auto& name = QInputDialog::getText(this, "Agregar SpreadSheet", "Nombre", QLineEdit::Normal, "", &ok);

        if(ok)
        {
            auto* spreadSheet = new SpreadSheet(this);

            tabBar->addTab(spreadSheet, name);
            spreadSheets.addSpreadSheet(name, spreadSheet);

            QDir().mkdir(user + '/' + name);
        }
    });

    QObject::connect(tabBar, &QTabWidget::tabCloseRequested,
           [this](auto index)
    {
        const auto& name = tabBar->tabText(index);

        QMessageBox message(this);
        message.setText("Esta seguro de Remover " + name + " ?");
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        auto button = message.exec();

        switch(button)
        {
            case QMessageBox::Yes:
            tabBar->removeTab(index);
            spreadSheets.removeSpreadSheet(name);
            QDir(user + '/' + name).removeRecursively();
            break;
        }
    });
}

void MainWindow::loadUser(const QString& user)
{
    this->user = user;

    spreadSheets.loadSpreadSheets(user);

    for(auto spreadSheetItr = spreadSheets.cbegin(); spreadSheetItr != spreadSheets.cend(); ++spreadSheetItr)
    {
        tabBar->addTab(spreadSheetItr.value(), spreadSheetItr.key());
    }
}

SpreadSheet* MainWindow::getCurrentSpreadSheet()
{
    return spreadSheets[tabBar->tabText(tabBar->currentIndex())];
}
