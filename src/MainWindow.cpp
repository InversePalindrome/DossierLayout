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
    view(new QGraphicsView(this)),
    menuBar(new QMenuBar(this)),
    toolBar(new QToolBar(this)),
    tabBar(new QTabWidget(this)),
    spreadSheets(this)
{
    setFixedSize(2048, 1536);
    setCentralWidget(view);
    setMenuBar(menuBar);
    setContextMenuPolicy(Qt::NoContextMenu);
    addToolBar(toolBar);

    centralWidget()->setLayout(new QVBoxLayout());
    centralWidget()->layout()->addWidget(tabBar);

    auto* addButton = new QToolButton();
    addButton->setText("+");

    tabBar->addTab(new QLabel(), QString());
    tabBar->setTabEnabled(0, false);
    tabBar->tabBar()->setTabButton(0, QTabBar::RightSide, addButton);

    tabBar->setTabsClosable(true);
    tabBar->setMovable(true);
    tabBar->setFont(QFont("Arial", 11, QFont::Bold));
    tabBar->setStyleSheet("QTabBar::tab { min-width: 100px; min-height : 60px; }");

    auto* File = menuBar->addMenu("File");

    File->addAction("Open", [this]()
    {
        getCurrentSpreadSheet()->loadSpreadSheet(QFileDialog::
           getOpenFileName(this, "Open", "", "Excel (*.xlsx)"));
    });
    File->addAction("Save as", [this]()
    {
        getCurrentSpreadSheet()->saveSpreadSheet(QFileDialog::
          getSaveFileName(this, "Guardar Como", "", "SpreadSheet (*.pdf .xlsx)"));
    });
    File->addAction("Print", [this]()
    {
        if(getCurrentSpreadSheet())
        {
            getCurrentSpreadSheet()->print();
        }
    });
    File->addSeparator();
    File->addAction("Exit", [this]()
    {
        emit exit();
    });

    auto* insert = menuBar->addMenu("Insert");

    insert->addAction("Column", [this]()
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(500, 200);
        insertDialog->setWindowTitle("Insert Column");
        insertDialog->setLabelText("Column Name");

        if(insertDialog->exec() == QDialog::Accepted && getCurrentSpreadSheet())
        {
            getCurrentSpreadSheet()->insertColumn(insertDialog->textValue());
        }
    });
    insert->addAction("Row", [this]()
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(500, 200);
        insertDialog->setWindowTitle("Insert Row");
        insertDialog->setLabelText("Row Name");

        if(insertDialog->exec() == QDialog::Accepted && getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->insertRow(insertDialog->textValue());
        }
    });

    auto* remove = menuBar->addMenu("Remove");

    remove->addAction("Column",
            [this]()
    {
        if(getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->removeColumn();
        }
    });

    remove->addAction("Row",
            [this]()
    {
        if(getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->removeRow();
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
    operationButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    operationButton->setPopupMode(QToolButton::InstantPopup);
    operationButton->setIcon(QIcon(":/Resources/Sigma.png"));
    operationButton->setText("Calculate  ");

    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(operationButton);
    toolBar->addSeparator();

    auto* sortMenu = new QMenu();
    auto* columnSort = sortMenu->addMenu("Column");
    columnSort->addAction("Ascending", [this]()
    {
        if(getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->sortSelectedColumn(Qt::AscendingOrder);
        }
    });
    columnSort->addAction("Descending", [this]()
    {
        if(getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->sortSelectedColumn(Qt::DescendingOrder);
        }
    });
    auto* rowSort = sortMenu->addMenu("Row");
    rowSort->addAction("Ascending", [this]()
    {
        if(getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->sortSelectedRow(Qt::AscendingOrder);
        }
    });
    rowSort->addAction("Descending", [this]()
    {
        if(getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->sortSelectedRow(Qt::DescendingOrder);
        }
    });

    auto* sortButton = new QToolButton();
    sortButton->setMenu(sortMenu);
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText("Sort  ");

    toolBar->addWidget(sortButton);
    toolBar->addSeparator();
    toolBar->addAction(QIcon(":/Resources/Merge.png"), "Merge", [this]()
    {
        if(getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->mergeSelected();
        }
    });
    toolBar->addAction(QIcon(":/Resources/Split.png"), "Split", [this]()
    {
        if(getCurrentSpreadSheet())
        {
           getCurrentSpreadSheet()->splitSelected();
        }
    });

    QObject::connect(addButton, &QToolButton::clicked, [this]()
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(600, 200);
        insertDialog->setWindowTitle("Add SpreadSheet");
        insertDialog->setLabelText("SpreadSheet Name");

        if(insertDialog->exec() == QDialog::Accepted)
        {
            const auto& name = insertDialog->textValue();

            if(name.isEmpty())
            {
                QMessageBox message(this);
                message.setWindowTitle("Error");
                message.setText("No name specified!");
                message.exec();
            }
            else
            {
                auto* spreadSheet = spreadSheets.addSpreadSheet(name);

                tabBar->addTab(spreadSheet, name);

                QDir().mkdir(user + '/' + name);
            }
        }
    });
    QObject::connect(tabBar, &QTabWidget::tabCloseRequested, [this](auto index)
    {
        const auto& name = tabBar->tabText(index);

        QMessageBox message(this);
        message.setWindowTitle("Error");
        message.setText("Are you sure about removing " + name + " ?");
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        switch(message.exec())
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
