/*
Copyright (c) 2018 InversePalindrome
DossierLayout - MainWindow.cpp
InversePalindrome.com
*/


#include "MainWindow.hpp"

#include <QDir>
#include <QIcon>
#include <QFile>
#include <QMenu>
#include <QLabel>
#include <QCheckBox>
#include <QBoxLayout>
#include <QCompleter>
#include <QFormLayout>
#include <QPushButton>
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include <QDomDocument>
#include <QRadioButton>
#include <QButtonGroup>


MainWindow::MainWindow(const QString& user) :
    view(new QGraphicsView(this)),
    menuBar(new QMenuBar(this)),
    toolBar(new QToolBar(this)),
    tabBar(new QTabWidget(this)),
    searchBar(new QLineEdit(this)),
    user(user)
{
    setMinimumSize(2048, 1536);
    setMenuBar(menuBar);
    addToolBar(toolBar);
    setCentralWidget(view);
    setContextMenuPolicy(Qt::NoContextMenu);

    centralWidget()->setLayout(new QVBoxLayout());
    centralWidget()->layout()->addWidget(tabBar);

    menuBar->setCornerWidget(searchBar);

    searchBar->setPlaceholderText("🔍");
    searchBar->setFont(QFont("Seqoe UI Symbol"));

    auto* addButton = new QToolButton(this);
    addButton->setText("+");

    tabBar->addTab(new QLabel(this), QString());
    tabBar->setTabEnabled(0, false);
    tabBar->tabBar()->setTabButton(0, QTabBar::RightSide, addButton);
    tabBar->setTabsClosable(true);
    tabBar->setMovable(true);
    tabBar->setFont(QFont("Arial", 11, QFont::Bold));
    tabBar->setStyleSheet("QTabBar::tab { min-width: 100px; min-height : 60px; }");

    QObject::connect(addButton, &QToolButton::clicked, [this, user]
    {
        auto* addDialog = new QDialog(this, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
        addDialog->setFixedSize(600, 270);
        addDialog->setWindowTitle(tr("Add Data Structure"));

        auto* nameEntry = new QLineEdit(this);

        auto* formLayout = new QFormLayout(this);
        formLayout->addRow(tr("Name:"), nameEntry);

        auto* listButton = new QRadioButton(tr("List"), this);
        listButton->setProperty("type", "List");
        listButton->setIcon(QIcon(":/Resources/List.png"));
        listButton->setChecked(true);

        auto* tableButton = new QRadioButton(tr("Table"), this);
        tableButton->setProperty("type", "Table");
        tableButton->setIcon(QIcon(":/Resources/Table.png"));

        auto* treeButton = new QRadioButton(tr("Tree"), this);
        treeButton->setProperty("type", "Tree");
        treeButton->setIcon(QIcon(":/Resources/Tree.png"));

        auto* buttonGroup = new QButtonGroup();
        buttonGroup->addButton(listButton);
        buttonGroup->addButton(tableButton);
        buttonGroup->addButton(treeButton);

        auto* radioButtonLayout = new QHBoxLayout();
        radioButtonLayout->addWidget(listButton);
        radioButtonLayout->addWidget(tableButton);
        radioButtonLayout->addWidget(treeButton);

        auto* addButton = new QPushButton(tr("Add"), this);
        auto* cancelButton = new QPushButton(tr("Cancel"), this);

        auto* buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(cancelButton);

        auto* layout = new QVBoxLayout(addDialog);

        layout->addLayout(formLayout);
        layout->addSpacing(20);
        layout->addLayout(radioButtonLayout);
        layout->addSpacing(20);
        layout->addLayout(buttonLayout);

        addDialog->show();

        QObject::connect(addButton, &QPushButton::clicked, [this, addDialog, buttonGroup, nameEntry, user]
        {
            const auto& type = buttonGroup->checkedButton()->property("type").toString();
            const auto& name = nameEntry->text();

            if(name.isEmpty())
            {
                QMessageBox message(QMessageBox::Critical, tr("Error"), tr("No Data Structure name specified!"), QMessageBox::NoButton, this);
                message.exec();
            }
            else if(dataStructureExists(name))
            {
                QMessageBox message(QMessageBox::Critical, tr("Error"), tr("Data Structure '") + name + tr("' already exists!"), QMessageBox::NoButton, this);
                message.exec();
            }
            else
            {
                addTab(type, name);

                QDir().mkdir(user + '/' + name);

                addDialog->close();

                updateSearchBar();
            }
        });
        QObject::connect(cancelButton, &QPushButton::clicked, [addDialog] { addDialog->close(); });
    });
    QObject::connect(tabBar, &QTabWidget::tabCloseRequested, [this, user](auto index)
    {
        const auto& name = tabBar->tabText(index);

        QMessageBox message(QMessageBox::Critical, tr("Error"), tr("Are you sure about removing ") + name + " ?", QMessageBox::Yes | QMessageBox::No, this);

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
        menuBar->clear();
        toolBar->clear();

        const auto* list = qobject_cast<List*>(tabBar->widget(index));
        const auto* table = qobject_cast<Table*>(tabBar->widget(index));
        const auto* tree = qobject_cast<Tree*>(tabBar->widget(index));

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
        else
        {
            auto* file =  menuBar->addMenu(tr("File"));
            file->addAction(QIcon(":/Resources/Exit.png"), tr("   Exit"), [this] { emit exit(); }, QKeySequence("Esc"));
        }
    });
    QObject::connect(searchBar, &QLineEdit::returnPressed, [this]
    {
        emit findTab(searchBar->text());
    });

    load(user);
}

MainWindow::~MainWindow()
{
    save();
}

void MainWindow::load(const QString& user)
{
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

            addTab(type, name);
        }
    }

    if(tabBar->count() == 1)
    {
        auto* file =  menuBar->addMenu(tr("File"));
        file->addAction(QIcon(":/Resources/Exit.png"), tr("   Exit"), [this] { emit exit(); }, QKeySequence("Esc"));
    }
    else if(tabBar->count() > 1)
    {
        tabBar->setCurrentIndex(1);
    }

    updateSearchBar();
}

void MainWindow::save()
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
}

void MainWindow::setupListFunctions(const List* list)
{
    auto* file = menuBar->addMenu(tr("File"));
    file->addAction(QIcon(":/Resources/Open.png"), tr("   Open"), [this]
    {
        emit loadDataStructure(QFileDialog::getOpenFileName(this, tr("Open"), "", "Xml (*.xml)"));
    }, QKeySequence::Open);
    file->addAction(QIcon(":/Resources/Download.png"), tr("   Save as"), [this]
    {
        emit saveDataStructure(QFileDialog::getSaveFileName(this, tr("Save as"), "", tr("List (*.pdf .xml)")));
    }, QKeySequence::Save);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Print.png"), tr("   Print"), [this] { emit print(); }, QKeySequence::Print);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Exit.png"), tr("   Exit"), [this] { emit exit(); }, QKeySequence("Esc"));

    auto* insert = menuBar->addMenu(tr("Insert"));
    insert->addAction(QIcon(":/Resources/AddRow.png"), tr("   Element"), [this]
    {
        auto* insertDialog = new QDialog(this, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
        insertDialog->setFixedSize(600, 270);
        insertDialog->setWindowTitle(tr("Insert Element"));

        auto* nameLabel = new QLabel(tr("Name"), this);
        auto* nameEntry = new QLineEdit(this);

        auto* checkButton = new QCheckBox(tr("Checkable"), this);

        auto* okButton = new QPushButton("Ok", this);
        auto* cancelButton = new QPushButton(tr("Cancel"), this);

        auto* buttonLayout = new QHBoxLayout(this);

        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);

        auto* layout = new QVBoxLayout();
        layout->addWidget(nameLabel);
        layout->addWidget(nameEntry);
        layout->addWidget(checkButton);
        layout->addLayout(buttonLayout);

        insertDialog->setLayout(layout);

        QObject::connect(okButton, &QPushButton::clicked, insertDialog, &QDialog::accept);
        QObject::connect(cancelButton, &QPushButton::clicked, insertDialog, &QDialog::reject);

        if(insertDialog->exec() == QDialog::Accepted)
        {
            if(checkButton->isChecked())
            {
                emit insertElement(nameEntry->text(), Qt::ItemIsUserCheckable);
            }
            else
            {
                emit insertElement(nameEntry->text(), Qt::NoItemFlags);
            }
        }
    });

    auto* remove = menuBar->addMenu(tr("Remove"));
    remove->addAction(QIcon(":/Resources/RemoveRow.png"), tr("   Element"), [this] { emit removeElement(); });

    auto* sortMenu = new QMenu(this);
    sortMenu->addAction(tr("Ascending"), [this] { emit sortColumn(Qt::AscendingOrder); });
    sortMenu->addAction(tr("Descending"), [this] { emit sortColumn(Qt::DescendingOrder); });

    auto* sortButton = new QToolButton(this);
    sortButton->setMenu(sortMenu);
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText(tr("Sort  "));

    toolBar->addWidget(sortButton);

    connections << QObject::connect(this, &MainWindow::loadDataStructure, list, &List::load);
    connections << QObject::connect(this, &MainWindow::saveDataStructure, list, &List::save);
    connections << QObject::connect(this, &MainWindow::print, list, &List::print);
    connections << QObject::connect(this, &MainWindow::insertElement, list, &List::insertElement);
    connections << QObject::connect(this, &MainWindow::removeElement, list, &List::removeElement);
    connections << QObject::connect(this, &MainWindow::sortColumn, list, &List::sort);
}

void MainWindow::setupTableFunctions(const Table* table)
{
    auto* file = menuBar->addMenu(tr("File"));
    file->addAction(QIcon(":/Resources/Open.png"), tr("   Open"), [this]
    {
        emit loadDataStructure(QFileDialog::getOpenFileName(this, tr("Open"), "", "Excel (*.xlsx)"));
    }, QKeySequence::Open);
    file->addAction(QIcon(":/Resources/Download.png"), tr("   Save as"), [this]
    {
        emit saveDataStructure(QFileDialog::getSaveFileName(this, tr("Save as"), "", tr("Table (*.pdf .xlsx)")));
    }, QKeySequence::Save);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Print.png"), tr("   Print"), [this] { emit print(); }, QKeySequence::Print);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Exit.png"), tr("   Exit"), [this] { emit exit(); }, QKeySequence("Esc"));

    auto* insert = menuBar->addMenu(tr("Insert"));
    insert->addAction(QIcon(":/Resources/AddColumn.png"), tr("   Column"), [this]
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(500, 200);
        insertDialog->setWindowTitle(tr("Insert Column"));
        insertDialog->setLabelText(tr("Column Name"));

        if(insertDialog->exec() == QDialog::Accepted)
        {
            emit insertColumn(insertDialog->textValue());
        }
    });
    insert->addAction(QIcon(":/Resources/AddRow.png"), tr("   Row"), [this]
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(500, 200);
        insertDialog->setWindowTitle(tr("Insert Row"));
        insertDialog->setLabelText(tr("Row Name"));

        if(insertDialog->exec() == QDialog::Accepted)
        {
           emit insertRow(insertDialog->textValue());
        }
    });

    auto* remove = menuBar->addMenu(tr("Remove"));
    remove->addAction(QIcon(":/Resources/RemoveColumn.png"), tr("   Column"), [this] { emit removeColumn(); });
    remove->addAction(QIcon(":/Resources/RemoveRow.png"), tr("   Row"), [this] { emit removeRow(); });

    auto* operationsMenu = new QMenu(this);
    operationsMenu->addAction(tr("Sum"), [this] { emit getSum(); });
    operationsMenu->addAction(tr("Average"), [this] { emit getAverage(); });
    operationsMenu->addAction(tr("Min"), [this] { emit getMin(); });
    operationsMenu->addAction(tr("Max"), [this] { emit getMax(); });
    operationsMenu->addAction(tr("Count"), [this] { emit getCount(); });

    auto* operationButton = new QToolButton(this);
    operationButton->setMenu(operationsMenu);
    operationButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    operationButton->setPopupMode(QToolButton::InstantPopup);
    operationButton->setIcon(QIcon(":/Resources/Sigma.png"));
    operationButton->setText(tr("Calculate  "));

    auto* sortMenu = new QMenu(this);
    auto* columnSort = sortMenu->addMenu(tr("Column"));
    columnSort->addAction(tr("Ascending"), [this] { emit sortColumn(Qt::AscendingOrder); });
    columnSort->addAction(tr("Descending"), [this] { emit sortColumn(Qt::DescendingOrder); });

    auto* rowSort = sortMenu->addMenu("Row");
    rowSort->addAction(tr("Ascending"), [this] { emit sortRow(Qt::AscendingOrder); });
    rowSort->addAction(tr("Descending"), [this] { emit sortRow(Qt::DescendingOrder); });

    auto* sortButton = new QToolButton(this);
    sortButton->setMenu(sortMenu);
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText(tr("Sort  "));

    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(operationButton);
    toolBar->addSeparator();
    toolBar->addWidget(sortButton);
    toolBar->addSeparator();
    toolBar->addAction(QIcon(":/Resources/Merge.png"), tr("   Merge"), [this] { emit merge(); });
    toolBar->addAction(QIcon(":/Resources/Split.png"), tr("   Split"), [this] { emit split(); });

    connections << QObject::connect(this, &MainWindow::loadDataStructure, table, &Table::load);
    connections << QObject::connect(this, &MainWindow::saveDataStructure, table, &Table::save);
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

void MainWindow::setupTreeFunctions(const Tree* tree)
{
    auto* file = menuBar->addMenu(tr("File"));

    file->addAction(QIcon(":/Resources/Open.png"), tr("   Open"), [this]
    {
        emit loadDataStructure(QFileDialog::getOpenFileName(this, tr("Open"), "", "Xml (*.xml)"));
    }, QKeySequence::Open);
    file->addAction(QIcon(":/Resources/Download.png"), tr("   Save as"), [this]
    {
       emit saveDataStructure(QFileDialog::getSaveFileName(this, tr("Save as"), "", tr("Tree (*.pdf .xml)")));
    }, QKeySequence::Save);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Print.png"), tr("   Print"), [this] { emit print(); }, QKeySequence::Print);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Exit.png"), tr("   Exit"), [this] { emit exit(); }, QKeySequence("Esc"));

    auto* insert = menuBar->addMenu(tr("Insert"));
    insert->addAction(QIcon(":/Resources/AddColumn.png"), tr("   Column"), [this]
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(500, 200);
        insertDialog->setWindowTitle(tr("Insert Column"));
        insertDialog->setLabelText(tr("Column Name"));

        if(insertDialog->exec() == QDialog::Accepted)
        {
           emit insertColumn(insertDialog->textValue());
        }
    });
    insert->addAction(QIcon(":/Resources/AddNode.png"), tr("   Node"), [this]
    {
        auto* insertDialog = new QInputDialog(this);
        insertDialog->setFixedSize(500, 200);
        insertDialog->setWindowTitle(tr("Insert Node"));
        insertDialog->setLabelText(tr("Node Name"));

        if(insertDialog->exec() == QDialog::Accepted)
        {
           emit insertNode(insertDialog->textValue());
        }
    });

    auto* remove = menuBar->addMenu(tr("Remove"));
    remove->addAction(QIcon(":/Resources/RemoveNode.png"), tr("   Node"), [this]
    {
        emit removeNode();
    });

    auto* sortMenu = new QMenu(this);
    sortMenu->addAction(tr("Ascending"), [this] { emit sortColumn(Qt::AscendingOrder); });
    sortMenu->addAction(tr("Descending"), [this] { emit sortColumn(Qt::DescendingOrder); });

    auto* sortButton = new QToolButton(this);
    sortButton->setMenu(sortMenu);
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText(tr("Sort  "));

    toolBar->addWidget(sortButton);

    connections << QObject::connect(this, &MainWindow::loadDataStructure, tree, &Tree::load);
    connections << QObject::connect(this, &MainWindow::saveDataStructure, tree, &Tree::save);
    connections << QObject::connect(this, &MainWindow::print, tree, &Tree::print);
    connections << QObject::connect(this, &MainWindow::insertColumn, tree, &Tree::insertColumn);
    connections << QObject::connect(this, &MainWindow::insertNode, tree, &Tree::insertNode);
    connections << QObject::connect(this, &MainWindow::removeNode, tree, &Tree::removeNode);
    connections << QObject::connect(this, &MainWindow::sortColumn, tree, &Tree::sortColumn);
}

void MainWindow::updateSearchBar()
{
    QStringList tabNames;

    for(int i = 0; i < tabBar->count(); ++i)
    {
        tabNames << tabBar->tabText(i);
    }

    auto* searchOptions = new QCompleter(tabNames, this);
    searchBar->setCompleter(searchOptions);

    QObject::connect(searchOptions, static_cast<void(QCompleter::*)(const QString&)>(&QCompleter::activated), this, &MainWindow::findTab);
}

void MainWindow::addTab(const QString& type, const QString& name)
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

void MainWindow::findTab(const QString& tabName)
{
    for(int i = 0; i < tabBar->count(); ++i)
    {
        if(tabBar->tabText(i) == tabName)
        {
            tabBar->setCurrentIndex(i);
        }
    }

    searchBar->clear();
}
