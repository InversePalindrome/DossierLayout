/*
Copyright (c) 2018 InversePalindrome
DossierTable - MainWindow.cpp
InversePalindrome.com
*/


#include "MainWindow.hpp"

#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QLabel>
#include <QBoxLayout>
#include <QClipboard>
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QGraphicsView>


MainWindow::MainWindow() :
    menuBar(new QMenuBar(this)),
    toolBar(new QToolBar(this)),
    tabBar(new QTabWidget(this)),
    spreadSheets(this)
{
    setFixedSize(2048, 1536);
    setMenuBar(menuBar);
    setContextMenuPolicy(Qt::NoContextMenu);
    addToolBar(toolBar);

    auto* addButton = new QToolButton();
    addButton->setText("+");

    tabBar->addTab(new QLabel(), QString());
    tabBar->setTabEnabled(0, false);
    tabBar->tabBar()->setTabButton(0, QTabBar::RightSide, addButton);

    tabBar->setTabsClosable(true);
    tabBar->setFont(QFont("Arial", 11, QFont::Bold));
    tabBar->setStyleSheet("QTabBar::tab { min-width: 100px; min-height : 60px; }");

    auto* view = new QGraphicsView(this);

    setCentralWidget(view);
    centralWidget()->setLayout(new QVBoxLayout());
    centralWidget()->layout()->addWidget(tabBar);

    auto* archivo = menuBar->addMenu("Archivo");

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
    archivo->addSeparator();
    archivo->addAction("Salir", [this]()
    {
        emit exit();
    });

    auto* insertar = menuBar->addMenu("Insertar");

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

    auto* remove = menuBar->addMenu("Remover");

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

    auto* clipboard = QApplication::clipboard();

    auto* operationsMenu = new QMenu();
    operationsMenu->addAction("Sum", [this, clipboard]()
    {
        clipboard->setText(getCurrentSpreadSheet()->getSelectedSum());
    });
    operationsMenu->addAction("Average", [this, clipboard]()
    {
        clipboard->setText(getCurrentSpreadSheet()->getSelectedAverage());
    });
    operationsMenu->addAction("Min", [this, clipboard]()
    {
        clipboard->setText(getCurrentSpreadSheet()->getSelectedMin());
    });
    operationsMenu->addAction("Max", [this, clipboard]()
    {
        clipboard->setText(getCurrentSpreadSheet()->getSelectedMax());
    });
    operationsMenu->addAction("Count", [this, clipboard]
    {
        clipboard->setText(getCurrentSpreadSheet()->getSelectedCount());
    });

    auto* operationButton = new QToolButton();
    operationButton->setMenu(operationsMenu);
    operationButton->setPopupMode(QToolButton::InstantPopup);
    operationButton->setIcon(QIcon(":/Resources/Sigma.png"));
    toolBar->addWidget(operationButton);

    QObject::connect(addButton, &QToolButton::clicked, [this]()
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
    QObject::connect(tabBar, &QTabWidget::tabCloseRequested, [this](auto index)
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
