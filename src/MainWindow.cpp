/*
Copyright (c) 2018 InversePalindrome
DossierLayout - MainWindow.cpp
InversePalindrome.com
*/


#include "MainWindow.hpp"
#include "AddDataStructureDialog.hpp"

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
    setMinimumSize(2048, 1536);
    setMenuBar(menuBar);
    addToolBar(toolBar);
    setCentralWidget(view);
    setContextMenuPolicy(Qt::NoContextMenu);

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

    auto* addDialog = new AddDataStructureDialog(this);

    QObject::connect(addButton, &QToolButton::clicked, [addDialog]
    {
        addDialog->show();
    });
    QObject::connect(addDialog, &AddDataStructureDialog::addDataStructure, [this](const auto& type, const auto& name)
    {
        if(name.isEmpty())
        {
            QMessageBox message(QMessageBox::Critical, "Error", "No Data Structure name specified!", QMessageBox::NoButton, this);
            message.exec();
        }
        else if(dataStructureExists(name))
        {
            QMessageBox message(QMessageBox::Critical, "Error", "Data Structure '" + name + "' already exists!", QMessageBox::NoButton, this);
            message.exec();
        }
        else
        {
            if(type == "List")
            {
               tabBar->addTab(new List(this, user + '/' + name + '/'), QIcon(":/Resources/List.png"), name);
            }
            else if(type == "Table")
            {
               tabBar->addTab(new Table(this, user + '/' + name + '/'), QIcon(":/Resources/Table.png"), name);
            }
            else if(type == "Tree")
            {
               tabBar->addTab(new Tree(this, user + '/' + name + '/'), QIcon(":/Resources/Tree.png"), name);
            }

            QDir().mkdir(user + '/' + name);
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

        auto* list = qobject_cast<List*>(tabBar->widget(index));
        auto* table = qobject_cast<Table*>(tabBar->widget(index));
        auto* tree = qobject_cast<Tree*>(tabBar->widget(index));

        if(list)
        {
            setupListFunctions(list);
        }
        else if(table)
        {
            setupTableFunctions(table);
        }
        else if(tree)
        {
            setupTreeFunctions(tree);
        }
    });
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto dataStructuresElement = doc.createElement("DataStructures");

    for(int i = 1; i < tabBar->count(); ++i)
    {
        auto dataStructureElement = doc.createElement("DataStructure");

        const auto* list = qobject_cast<List*>(tabBar->widget(i));
        const auto* table = qobject_cast<Table*>(tabBar->widget(i));
        const auto* tree = qobject_cast<Tree*>(tabBar->widget(i));

        if(list)
        {
            dataStructureElement.setAttribute("type", "List");
        }
        else if(table)
        {
            dataStructureElement.setAttribute("type", "Table");
        }
        else if(tree)
        {
            dataStructureElement.setAttribute("type", "Tree");
        }

        dataStructureElement.setAttribute("name", tabBar->tabText(i));

        dataStructuresElement.appendChild(dataStructureElement);
    }

    doc.appendChild(dataStructuresElement);

    QFile file(user + "/DataStructures.xml");

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
    QFile file(user + "/DataStructures.xml");

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

    auto dataStructuresElement = doc.firstChildElement("DataStructures");
    auto dataStructureList = dataStructuresElement.elementsByTagName("DataStructure");

    for(int i = 0; i < dataStructureList.size(); ++i)
    {
        auto dataStructureNode = dataStructureList.at(i);

        if(dataStructureNode.isElement())
        {
            auto dataStructureElement = dataStructureNode.toElement();

            const auto& type = dataStructureElement.attribute("type");
            const auto& name = dataStructureElement.attribute("name");

            if(type == "List")
            {
                tabBar->addTab(new List(this, user + '/' + name + '/'), QIcon(":/Resources/List.png"), name);
            }
            else if(type == "Table")
            {
                tabBar->addTab(new Table(this, user + '/' + name + '/'), QIcon(":/Resources/Table.png"), name);
            }
            else if(type == "Tree")
            {
                tabBar->addTab(new Tree(this, user + '/' + name + '/'), QIcon(":/Resources/Tree.png"), name);
            }
        }
    }

    if(tabBar->count() > 1)
    {
        tabBar->setCurrentIndex(1);
    }
}

void MainWindow::setupListFunctions(List* list)
{
    menuBar->clear();
    toolBar->clear();

    auto* file = menuBar->addMenu("File");
    file->addAction(QIcon(":/Resources/Open.png"), "Open", [this]
    {
        emit loadDataStructure(QFileDialog::getOpenFileName(this, "Open", "", "Xml (*.xml)"));
    }, QKeySequence::Open);
    file->addAction(QIcon(":/Resources/Download.png"), "Save as", [this]
    {
        emit saveDataStructure(QFileDialog::getSaveFileName(this, "Save as", "", "List (*.pdf .xml)"));
    }, QKeySequence::Save);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Print.png"), "Print", [this] { emit print(); }, QKeySequence::Print);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Exit.png"), "Exit", [this] { emit exit(); }, QKeySequence("Esc"));

    auto* insert = menuBar->addMenu("Insert");
    insert->addAction(QIcon(":/Resources/AddRow.png"), "Element", [this]
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(500, 200);
        insertDialog->setWindowTitle("Insert Element");
        insertDialog->setLabelText("Element Name");

        if(insertDialog->exec() == QDialog::Accepted)
        {
            emit insertElement(insertDialog->textValue());
        }
    });

    auto* remove = menuBar->addMenu("Remove");
    remove->addAction(QIcon(":/Resources/RemoveRow.png"), "Element", [this] { emit removeElement(); });

    auto* sortMenu = new QMenu();
    sortMenu->addAction("Ascending", [this] { emit sortColumn(Qt::AscendingOrder); });
    sortMenu->addAction("Descending", [this] { emit sortColumn(Qt::DescendingOrder); });

    auto* sortButton = new QToolButton();
    sortButton->setMenu(sortMenu);
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText("Sort  ");

    toolBar->addWidget(sortButton);

    connections << QObject::connect(this, &MainWindow::loadDataStructure, list, &List::loadList);
    connections << QObject::connect(this, &MainWindow::saveDataStructure, list, &List::saveList);
    connections << QObject::connect(this, &MainWindow::print, list, &List::print);
    connections << QObject::connect(this, &MainWindow::insertElement, list, &List::insertElement);
    connections << QObject::connect(this, &MainWindow::removeElement, list, &List::removeElement);
    connections << QObject::connect(this, &MainWindow::sortColumn, list, &List::sort);
}

void MainWindow::setupTableFunctions(Table* table)
{
    menuBar->clear();
    toolBar->clear();

    auto* file = menuBar->addMenu("File");
    file->addAction(QIcon(":/Resources/Open.png"), "Open", [this]
    {
        emit loadDataStructure(QFileDialog::getOpenFileName(this, "Open", "", "Excel (*.xlsx)"));
    }, QKeySequence::Open);
    file->addAction(QIcon(":/Resources/Download.png"), "Save as", [this]
    {
        emit saveDataStructure(QFileDialog::getSaveFileName(this, "Save as", "", "Table (*.pdf .xlsx)"));
    }, QKeySequence::Save);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Print.png"), "Print", [this] { emit print(); }, QKeySequence::Print);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Exit.png"), "Exit", [this] { emit exit(); }, QKeySequence("Esc"));

    auto* insert = menuBar->addMenu("Insert");
    insert->addAction(QIcon(":/Resources/AddColumn.png"), "Column", [this]
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
    insert->addAction(QIcon(":/Resources/AddRow.png"), "Row", [this]
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
    remove->addAction(QIcon(":/Resources/RemoveColumn.png"), "Column", [this] { emit removeColumn(); });
    remove->addAction(QIcon(":/Resources/RemoveRow.png"), "Row", [this] { emit removeRow(); });

    auto* operationsMenu = new QMenu();
    operationsMenu->addAction("Sum", [this] { emit getSum(); });
    operationsMenu->addAction("Average", [this] { emit getAverage(); });
    operationsMenu->addAction("Min", [this] { emit getMin(); });
    operationsMenu->addAction("Max", [this] { emit getMax(); });
    operationsMenu->addAction("Count", [this] { emit getCount(); });

    auto* operationButton = new QToolButton();
    operationButton->setMenu(operationsMenu);
    operationButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    operationButton->setPopupMode(QToolButton::InstantPopup);
    operationButton->setIcon(QIcon(":/Resources/Sigma.png"));
    operationButton->setText("Calculate  ");

    auto* sortMenu = new QMenu();
    auto* columnSort = sortMenu->addMenu("Column");
    columnSort->addAction("Ascending", [this] { emit sortColumn(Qt::AscendingOrder); });
    columnSort->addAction("Descending", [this] { emit sortColumn(Qt::DescendingOrder); });

    auto* rowSort = sortMenu->addMenu("Row");
    rowSort->addAction("Ascending", [this] { emit sortRow(Qt::AscendingOrder); });
    rowSort->addAction("Descending", [this] { emit sortRow(Qt::DescendingOrder); });

    auto* sortButton = new QToolButton();
    sortButton->setMenu(sortMenu);
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText("Sort  ");

    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(operationButton);
    toolBar->addSeparator();
    toolBar->addWidget(sortButton);
    toolBar->addSeparator();
    toolBar->addAction(QIcon(":/Resources/Merge.png"), "Merge", [this] { emit merge(); });
    toolBar->addAction(QIcon(":/Resources/Split.png"), "Split", [this] { emit split(); });

    connections << QObject::connect(this, &MainWindow::loadDataStructure, table, &Table::loadTable);
    connections << QObject::connect(this, &MainWindow::saveDataStructure, table, &Table::saveTable);
    connections << QObject::connect(this, &MainWindow::print, table, &Table::print);
    connections << QObject::connect(this, &MainWindow::insertColumn, table, &Table::insertColumn);
    connections << QObject::connect(this, &MainWindow::insertRow, table, &Table::insertRow);
    connections << QObject::connect(this, &MainWindow::removeColumn, table, &Table::removeColumn);
    connections << QObject::connect(this, &MainWindow::removeRow, table, &Table::removeRow);
    connections << QObject::connect(this, &MainWindow::getSum, table, &Table::getSum);
    connections << QObject::connect(this, &MainWindow::getAverage, table, &Table::getAverage);
    connections << QObject::connect(this, &MainWindow::getMin, table, &Table::getMin);
    connections << QObject::connect(this, &MainWindow::getMax, table, &Table::getMax);
    connections << QObject::connect(this, &MainWindow::getCount, table, &Table::getCount);
    connections << QObject::connect(this, &MainWindow::sortColumn, table, &Table::sortColumn);
    connections << QObject::connect(this, &MainWindow::sortRow, table, &Table::sortRow);
    connections << QObject::connect(this, &MainWindow::merge, table, &Table::merge);
    connections << QObject::connect(this, &MainWindow::split, table, &Table::split);
}

void MainWindow::setupTreeFunctions(Tree* tree)
{
    menuBar->clear();
    toolBar->clear();

    auto* file = menuBar->addMenu("File");

    file->addAction(QIcon(":/Resources/Open.png"), "Open", [this]
    {
        emit loadDataStructure(QFileDialog::getOpenFileName(this, "Open", "", "Xml (*.xml)"));
    }, QKeySequence::Open);
    file->addAction(QIcon(":/Resources/Download.png"), "Save as", [this]
    {
       emit saveDataStructure(QFileDialog::getSaveFileName(this, "Save as", "", "Tree (*.pdf .xml)"));
    }, QKeySequence::Save);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Print.png"), "Print", [this] { emit print(); }, QKeySequence::Print);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Exit.png"), "Exit", [this] { emit exit(); }, QKeySequence("Esc"));

    auto* insert = menuBar->addMenu("Insert");
    insert->addAction(QIcon(":/Resources/AddColumn.png"), "Column", [this]
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
    insert->addAction(QIcon(":/Resources/AddNode.png"), "Node", [this]
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(500, 200);
        insertDialog->setWindowTitle("Insert Node");
        insertDialog->setLabelText("Node Name");

        if(insertDialog->exec() == QDialog::Accepted)
        {
           emit insertNode(insertDialog->textValue());
        }
    });

    auto* remove = menuBar->addMenu("Remove");
    remove->addAction(QIcon(":/Resources/RemoveNode.png"), "Node", [this]
    {
        emit removeNode();
    });

    auto* sortMenu = new QMenu();
    sortMenu->addAction("Ascending", [this] { emit sortColumn(Qt::AscendingOrder); });
    sortMenu->addAction("Descending", [this] { emit sortColumn(Qt::DescendingOrder); });

    auto* sortButton = new QToolButton();
    sortButton->setMenu(sortMenu);
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText("Sort  ");

    toolBar->addWidget(sortButton);

    connections << QObject::connect(this, &MainWindow::loadDataStructure, tree, &Tree::loadTree);
    connections << QObject::connect(this, &MainWindow::saveDataStructure, tree, &Tree::saveTree);
    connections << QObject::connect(this, &MainWindow::print, tree, &Tree::print);
    connections << QObject::connect(this, &MainWindow::insertColumn, tree, &Tree::insertColumn);
    connections << QObject::connect(this, &MainWindow::insertNode, tree, &Tree::insertNode);
    connections << QObject::connect(this, &MainWindow::removeNode, tree, &Tree::removeNode);
    connections << QObject::connect(this, &MainWindow::sortColumn, tree, &Tree::sortColumn);
}

bool MainWindow::dataStructureExists(const QString& name) const
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
