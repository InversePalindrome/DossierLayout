/*
Copyright (c) 2018 InversePalindrome
DossierTable - MainWindow.cpp
InversePalindrome.com
*/


#include "MainWindow.hpp"
#include "SpreadSheet.hpp"

#include <QDir>
#include <QIcon>
#include <QFile>
#include <QMenu>
#include <QLabel>
#include <QBoxLayout>
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include <QDomDocument>


MainWindow::MainWindow() :
    view(new QGraphicsView(this)),
    menuBar(new QMenuBar(this)),
    toolBar(new QToolBar(this)),
    tabBar(new QTabWidget(this))
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

    auto* file = menuBar->addMenu("File");
    file->addAction(QIcon(":/Resources/Open.png"), "Open", [this]()
    {
        emit loadSpreadSheet(QFileDialog::getOpenFileName(this, "Open", "", "Excel (*.xlsx)"));
    }, QKeySequence::Open);
    file->addAction(QIcon(":/Resources/Download.png"), "Save as", [this]()
    {
        emit saveSpreadSheet(QFileDialog::getSaveFileName(this, "Guardar Como", "", "SpreadSheet (*.pdf .xlsx)"));
    }, QKeySequence::Save);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Print.png"), "Print", [this]() { emit print(); }, QKeySequence::Print);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Exit.png"), "Exit", [this](){ emit exit(); }, QKeySequence("Esc"));

    auto* insert = menuBar->addMenu("Insert");
    insert->addAction(QIcon(":/Resources/AddColumn.png"),"Column", [this]()
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(500, 200);
        insertDialog->setWindowTitle("Insert Column");
        insertDialog->setLabelText("Column Name");

        if(insertDialog->exec() == QDialog::Accepted)
        {
            emit insertColumn(insertDialog->textValue());
        }
    });
    insert->addAction(QIcon(":/Resources/AddRow.png"), "Row", [this]()
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(500, 200);
        insertDialog->setWindowTitle("Insert Row");
        insertDialog->setLabelText("Row Name");

        if(insertDialog->exec() == QDialog::Accepted)
        {
           emit insertRow(insertDialog->textValue());
        }
    });

    auto* remove = menuBar->addMenu("Remove");
    remove->addAction(QIcon(":/Resources/RemoveColumn.png"), "Column", [this]() { emit removeColumn(); });
    remove->addAction(QIcon(":/Resources/RemoveRow.png"), "Row", [this]() { emit removeRow(); });

    auto* operationsMenu = new QMenu();
    operationsMenu->addAction("Sum", [this]() { emit getSum(); });
    operationsMenu->addAction("Average", [this]() { emit getAverage(); });
    operationsMenu->addAction("Min", [this]() { emit getMin(); });
    operationsMenu->addAction("Max", [this]() { emit getMax(); });
    operationsMenu->addAction("Count", [this]() { emit getCount(); });

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
    columnSort->addAction("Ascending", [this]() { emit sortColumn(Qt::AscendingOrder); });
    columnSort->addAction("Descending", [this](){ emit sortColumn(Qt::DescendingOrder); });

    auto* rowSort = sortMenu->addMenu("Row");
    rowSort->addAction("Ascending", [this]() { emit sortRow(Qt::AscendingOrder); });
    rowSort->addAction("Descending", [this]() { emit sortRow(Qt::DescendingOrder); });

    auto* sortButton = new QToolButton();
    sortButton->setMenu(sortMenu);
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText("Sort  ");

    toolBar->addWidget(sortButton);
    toolBar->addSeparator();
    toolBar->addAction(QIcon(":/Resources/Merge.png"), "Merge", [this](){ emit merge(); });
    toolBar->addAction(QIcon(":/Resources/Split.png"), "Split", [this]() { emit split(); });

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
                QMessageBox message(QMessageBox::Critical, "Error", "No name specified!", QMessageBox::NoButton, this);
                message.exec();
            }
            else if(spreadSheetExists(name))
            {
                QMessageBox message(QMessageBox::Critical, "Error", "SpreadSheet already exists!", QMessageBox::NoButton, this);
                message.exec();
            }
            else
            {
                tabBar->addTab(new SpreadSheet(this, user + '/' + name + '/'), name);

                QDir().mkdir(user + '/' + name);
            }
        }
    });
    QObject::connect(tabBar, &QTabWidget::tabCloseRequested, [this](auto index)
    {
        const auto& name = tabBar->tabText(index);

        QMessageBox message(QMessageBox::Critical, "Error", "Are you sure about removing " + name + " ?", QMessageBox::Yes | QMessageBox::No, this);

        if(message.exec() == QMessageBox::Yes)
        {
            tabBar->removeTab(index);
            QDir(user + '/' + name).removeRecursively();
        }
    });
    QObject::connect(tabBar, &QTabWidget::currentChanged, [this](auto index)
    {
        for(const auto& connection : connections)
        {
            QObject::disconnect(connection);
        }

        connections.clear();

        auto* spreadSheet = qobject_cast<SpreadSheet*>(tabBar->widget(index));

        if(!spreadSheet)
        {
            return;
        }

        connections << QObject::connect(this, &MainWindow::loadSpreadSheet, spreadSheet, &SpreadSheet::loadSpreadSheet);
        connections << QObject::connect(this, &MainWindow::saveSpreadSheet, spreadSheet, &SpreadSheet::saveSpreadSheet);
        connections << QObject::connect(this, &MainWindow::print, spreadSheet, &SpreadSheet::print);
        connections << QObject::connect(this, &MainWindow::insertColumn, spreadSheet, &SpreadSheet::insertColumn);
        connections << QObject::connect(this, &MainWindow::insertRow, spreadSheet, &SpreadSheet::insertRow);
        connections << QObject::connect(this, &MainWindow::removeColumn, spreadSheet, &SpreadSheet::removeColumn);
        connections << QObject::connect(this, &MainWindow::removeRow, spreadSheet, &SpreadSheet::removeRow);
        connections << QObject::connect(this, &MainWindow::getSum, spreadSheet, &SpreadSheet::getSum);
        connections << QObject::connect(this, &MainWindow::getAverage, spreadSheet, &SpreadSheet::getAverage);
        connections << QObject::connect(this, &MainWindow::getMin, spreadSheet, &SpreadSheet::getMin);
        connections << QObject::connect(this, &MainWindow::getMax, spreadSheet, &SpreadSheet::getMax);
        connections << QObject::connect(this, &MainWindow::getCount, spreadSheet, &SpreadSheet::getCount);
        connections << QObject::connect(this, &MainWindow::sortColumn, spreadSheet, &SpreadSheet::sortColumn);
        connections << QObject::connect(this, &MainWindow::sortRow, spreadSheet, &SpreadSheet::sortRow);
        connections << QObject::connect(this, &MainWindow::merge, spreadSheet, &SpreadSheet::merge);
        connections << QObject::connect(this, &MainWindow::split, spreadSheet, &SpreadSheet::split);
    });
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto spreadSheetsElement = doc.createElement("SpreadSheets");

    for(int i = 1; i < tabBar->count(); ++i)
    {
        auto spreadSheetElement = doc.createElement("SpreadSheet");
        spreadSheetElement.setAttribute("name", tabBar->tabText(i));

        spreadSheetsElement.appendChild(spreadSheetElement);
    }

    doc.appendChild(spreadSheetsElement);

    QFile file(user + "/SpreadSheets.xml");

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    else
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }

    for(int i = tabBar->count() - 1; i > 0; --i)
    {
        tabBar->widget(i)->deleteLater();
        tabBar->removeTab(i);
    }

    QWidget::closeEvent(event);
}

void MainWindow::load(const QString& user)
{
    this->user = user;

    QDomDocument doc;
    QFile file(user + "/SpreadSheets.xml");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    else
    {
        if(!doc.setContent(&file))
        {
            return;
        }

        file.close();
    }

    auto spreadSheetsElement = doc.firstChildElement("SpreadSheets");
    auto spreadSheetList = spreadSheetsElement.elementsByTagName("SpreadSheet");

    for(int i = 0; i < spreadSheetList.size(); ++i)
    {
        auto spreadSheetNode = spreadSheetList.at(i);

        if(spreadSheetNode.isElement())
        {
            auto spreadSheetElement = spreadSheetNode.toElement();

            const auto& name = spreadSheetElement.attribute("name");

            tabBar->addTab(new SpreadSheet(this, user + '/' + name + '/'), name);
        }
    }

    if(tabBar->count() > 1)
    {
        tabBar->setCurrentIndex(1);
    }
}

bool MainWindow::spreadSheetExists(const QString& name) const
{
    for(int i = 1; i < tabBar->count(); ++i)
    {
        if(tabBar->tabText(i).compare(name, Qt::CaseInsensitive) == 0)
        {
            return true;
        }
    }

    return false;
}
