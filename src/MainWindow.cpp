/*
Copyright (c) 2018 InversePalindrome
DossierLayout - MainWindow.cpp
InversePalindrome.com
*/


#include "MainWindow.hpp"

#include <QDir>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QCompleter>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QButtonGroup>
#include <QRadioButton>


MainWindow::MainWindow(const QString& user) :
    user(user),
    menuBar(new QMenuBar(this)),
    toolBar(new QToolBar(this)),
    view(new QGraphicsView(this)),
    stackWidget(new QStackedWidget(this)),
    titleIcon(new QLabel(this)),
    titleLabel(new QLabel(this))
{
    setMinimumSize(2048, 1536);
    setMenuBar(menuBar);
    addToolBar(toolBar);
    setContextMenuPolicy(Qt::NoContextMenu);
    setAttribute(Qt::WA_DeleteOnClose);
    setCentralWidget(view);

    QPixmap icon(":/Resources/User.png");
    icon = icon.scaledToHeight(64);

    titleIcon->setPixmap(icon);
    titleLabel->setText(user);
    titleLabel->setFont(QFont("MS Shell Dlg 2", 10, QFont::Bold));

    auto* titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleIcon, 0, Qt::AlignRight);
    titleLayout->addWidget(titleLabel, 0, Qt::AlignLeft);

    auto* centralLayout = new QVBoxLayout(view);
    centralLayout->addLayout(titleLayout);
    centralLayout->addWidget(stackWidget);

    auto* hub = new Hub(user, this);

    stackWidget->addWidget(hub);
    stackWidget->setCurrentIndex(0);

    setupHubFunctions(hub);
}

void MainWindow::setupHubFunctions(Hub* hub)
{
    auto* searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("🔍");
    searchBar->setFont(QFont("Seqoe UI Symbol"));

    auto* completer = new QCompleter(hub->getDataStructureModel(), searchBar);
    searchBar->setCompleter(completer);

    auto* exitButton = new QToolButton(this);
    exitButton->setText(tr("Exit"));
    exitButton->setIcon(QIcon(":/Resources/Exit.png"));
    exitButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    menuBar->setCornerWidget(searchBar);
    menuBar->setCornerWidget(exitButton, Qt::TopLeftCorner);

    QObject::connect(hub, &Hub::openDataStructure, [this, searchBar](const auto& type, const auto& name)
    {
        auto* menuButton = new QToolButton(this);
        menuButton->setText(tr("Menu"));
        menuButton->setIcon(QIcon(":/Resources/DataStructureMenu.png"));
        menuButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        menuBar->setCornerWidget(menuButton);
        searchBar->hide();
        menuButton->show();

        QPixmap icon(":/Resources/" + type + ".png");
        icon = icon.scaledToHeight(64);

        titleIcon->setPixmap(icon);
        titleLabel->setText(name);

        if(type == "List")
        {
            auto* list = new List(this, user + '/' + name + '/');

            setupListFunctions(list);

            stackWidget->addWidget(list);
        }
        else if(type == "Table")
        {
            auto* table = new Table(this, user + '/' + name + '/');

            setupTableFunctions(table);

            stackWidget->addWidget(table);
        }
        else if(type == "Tree")
        {
            auto* tree = new Tree(this, user + '/' + name + '/');

            setupTreeFunctions(tree);

            stackWidget->addWidget(tree);
        }

        stackWidget->setCurrentIndex(1);

        QObject::connect(menuButton, &QToolButton::clicked, [this, searchBar, menuButton]
        {
            menuBar->clear();
            toolBar->clear();

            menuBar->setCornerWidget(searchBar);
            menuButton->hide();
            searchBar->show();

            QPixmap icon(":/Resources/User.png");
            icon = icon.scaledToHeight(64);

            titleIcon->setPixmap(icon);
            titleLabel->setText(user);

            stackWidget->currentWidget()->deleteLater();
            stackWidget->removeWidget(stackWidget->widget(1));
            stackWidget->setCurrentIndex(0);
        });
    });
    QObject::connect(searchBar, &QLineEdit::returnPressed, [hub, searchBar]{ hub->findDataStructure(searchBar->text()); });
    QObject::connect(completer, static_cast<void(QCompleter::*)(const QString&)>(&QCompleter::activated), [hub](const auto& text)
    {
        hub->findDataStructure(text);
    });
    QObject::connect(exitButton, &QToolButton::clicked, [this]{ emit exit(); });
}

void MainWindow::setupListFunctions(List* list)
{
    auto* file = menuBar->addMenu(tr("File"));
    file->addAction(QIcon(":/Resources/Open.png"), "   " + tr("Open"), [this, list]
    {
        list->load(QFileDialog::getOpenFileName(this, tr("Open"), "", "Xml (*.xml)"));
    }, QKeySequence::Open);
    file->addAction(QIcon(":/Resources/Download.png"), "   " + tr("Save as"), [this, list]
    {
        list->save(QFileDialog::getSaveFileName(this, tr("Save as"), "", tr("List") + " (*.pdf .xml)"));
    }, QKeySequence::Save);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Print.png"), "   " + tr("Print"), [list] { list->print(); }, QKeySequence::Print);
    file->addSeparator();

    auto* insert = menuBar->addMenu(tr("Insert"));
    insert->addAction(QIcon(":/Resources/AddRow.png"), "   " + tr("Element"), [this, list]
    {
        auto* insertDialog = new QDialog(this, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
        insertDialog->setMinimumSize(600, 270);
        insertDialog->setWindowTitle(tr("Insert Element"));

        auto* nameLabel = new QLabel(tr("Name"), this);
        auto* nameEntry = new QLineEdit(this);

        auto* checkButton = new QCheckBox(tr("Checkable"), this);

        auto* okButton = new QPushButton(tr("Ok"), this);
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
                list->insertElement(nameEntry->text(), Qt::ItemIsUserCheckable);
            }
            else
            {
                list->insertElement(nameEntry->text(), Qt::NoItemFlags);
            }
        }
    });

    auto* remove = menuBar->addMenu(tr("Remove"));
    remove->addAction(QIcon(":/Resources/RemoveRow.png"), "   " + tr("Element"), [list] { list->removeElement(); });

    auto* sortButton = new QToolButton(this);
    sortButton->setMenu(new QMenu(this));
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText(tr("Sort") + "  ");
    sortButton->menu()->addAction(tr("Ascending"), [list] { list->sort(Qt::AscendingOrder); });
    sortButton->menu()->addAction(tr("Descending"), [list] { list->sort(Qt::DescendingOrder); });

    toolBar->addWidget(sortButton);
}

void MainWindow::setupTableFunctions(Table* table)
{
    auto* file = menuBar->addMenu(tr("File"));
    file->addAction(QIcon(":/Resources/Open.png"), "   " + tr("Open"), [this, table]
    {
        table->load(QFileDialog::getOpenFileName(this, tr("Open"), "", "Xml (*.xml)"));
    }, QKeySequence::Open);
    file->addAction(QIcon(":/Resources/Download.png"), "   " + tr("Save as"), [this, table]
    {
        table->save(QFileDialog::getSaveFileName(this, tr("Save as"), "", tr("Table") + " (*.pdf .xlsx .xml)"));
    }, QKeySequence::Save);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Print.png"), "   " + tr("Print"), [table] { table->print(); }, QKeySequence::Print);
    file->addSeparator();

    auto* insert = menuBar->addMenu(tr("Insert"));
    insert->addAction(QIcon(":/Resources/AddColumn.png"), "   " + tr("Column"), [this, table]
    {
       auto* insertDialog = new QInputDialog(this, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
       insertDialog->setFixedSize(520, 200);
       insertDialog->setWindowTitle(tr("Insert Column"));
       insertDialog->setLabelText(tr("Column Name"));

       if(insertDialog->exec() == QDialog::Accepted)
       {
           emit table->insertColumn(insertDialog->textValue());
       }
   });
   insert->addAction(QIcon(":/Resources/AddRow.png"), "   " + tr("Row"), [this, table]
   {
       auto* insertDialog = new QInputDialog(this, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
       insertDialog->setFixedSize(520, 200);
       insertDialog->setWindowTitle(tr("Insert Row"));
       insertDialog->setLabelText(tr("Row Name"));

       if(insertDialog->exec() == QDialog::Accepted)
       {
           table->insertRow(insertDialog->textValue());
       }
   });

   auto* remove = menuBar->addMenu(tr("Remove"));
   remove->addAction(QIcon(":/Resources/RemoveColumn.png"), "   " + tr("Column"), [table] { table->removeColumn(); });
   remove->addAction(QIcon(":/Resources/RemoveRow.png"), "   " + tr("Row"), [table]{ table->removeRow(); });

   auto* operationButton = new QToolButton(this);
   operationButton->setMenu(new QMenu(this));
   operationButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   operationButton->setPopupMode(QToolButton::InstantPopup);
   operationButton->setIcon(QIcon(":/Resources/Sigma.png"));
   operationButton->setText(tr("Calculate") + "  ");
   operationButton->menu()->addAction(tr("Sum"), [table] { table->getSum(); });
   operationButton->menu()->addAction(tr("Average"), [table] { table->getAverage(); });
   operationButton->menu()->addAction(tr("Min"), [table] { table->getMin(); });
   operationButton->menu()->addAction(tr("Max"), [table] { table->getMax(); });
   operationButton->menu()->addAction(tr("Count"), [table] { table->getCount(); });

   auto* sortButton = new QToolButton(this);
   sortButton->setMenu(new QMenu(this));
   sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   sortButton->setPopupMode(QToolButton::InstantPopup);
   sortButton->setIcon(QIcon(":/Resources/Sort.png"));
   sortButton->setText(tr("Sort") + "  ");
   auto* columnSort = sortButton->menu()->addMenu(tr("Column"));
   columnSort->addAction(tr("Ascending"), [table] { table->sortColumn(Qt::AscendingOrder); });
   columnSort->addAction(tr("Descending"), [table] { table->sortColumn(Qt::DescendingOrder); });
   auto* rowSort = sortButton->menu()->addMenu("Row");
   rowSort->addAction(tr("Ascending"), [table] { table->sortRow(Qt::AscendingOrder); });
   rowSort->addAction(tr("Descending"), [table] { table->sortRow(Qt::DescendingOrder); });

   toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   toolBar->addWidget(operationButton);
   toolBar->addSeparator();
   toolBar->addWidget(sortButton);
   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Resources/Merge.png"), "   " + tr("Merge"), [table] { table->merge(); });
   toolBar->addAction(QIcon(":/Resources/Split.png"), "   " + tr("Split"), [table] { table->split(); });
}

void MainWindow::setupTreeFunctions(Tree* tree)
{
    auto* file = menuBar->addMenu(tr("File"));

    file->addAction(QIcon(":/Resources/Open.png"), "   " + tr("Open"), [this, tree]
    {
        tree->load(QFileDialog::getOpenFileName(this, tr("Open"), "", "Xml (*.xml)"));
    }, QKeySequence::Open);
    file->addAction(QIcon(":/Resources/Download.png"), "   " + tr("Save as"), [this, tree]
    {
        tree->save(QFileDialog::getSaveFileName(this, tr("Save as"), "", tr("Tree") + " (*.pdf .xml)"));
    }, QKeySequence::Save);
    file->addSeparator();
    file->addAction(QIcon(":/Resources/Print.png"), "   " + tr("Print"), [tree] { tree->print(); }, QKeySequence::Print);
    file->addSeparator();

    auto* insert = menuBar->addMenu(tr("Insert"));
    insert->addAction(QIcon(":/Resources/AddColumn.png"), "   " + tr("Column"), [this, tree]
    {
        auto* insertDialog = new QInputDialog(this, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
        insertDialog->setFixedSize(520, 200);
        insertDialog->setWindowTitle(tr("Insert Column"));
        insertDialog->setLabelText(tr("Column Name"));

        if(insertDialog->exec() == QDialog::Accepted)
        {
           tree->insertColumn(insertDialog->textValue());
        }
    });
    insert->addAction(QIcon(":/Resources/AddNode.png"), "   " + tr("Node"), [this, tree]
    {
        auto* insertDialog = new QInputDialog(this, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
        insertDialog->setFixedSize(520, 200);
        insertDialog->setWindowTitle(tr("Insert Node"));
        insertDialog->setLabelText(tr("Node Name"));

        if(insertDialog->exec() == QDialog::Accepted)
        {
            tree->insertNode(insertDialog->textValue());
        }
    });

    auto* remove = menuBar->addMenu(tr("Remove"));
    remove->addAction(QIcon(":/Resources/RemoveNode.png"), "   " + tr("Node"), [tree] { tree->removeNode(); });

    auto* sortButton = new QToolButton(this);
    sortButton->setMenu(new QMenu(this));
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText(tr("Sort") + "  ");
    sortButton->menu()->addAction(tr("Ascending"), [tree] { tree->sortColumn(Qt::AscendingOrder); });
    sortButton->menu()->addAction(tr("Descending"), [tree] { tree->sortColumn(Qt::DescendingOrder); });

    toolBar->addWidget(sortButton);
}
