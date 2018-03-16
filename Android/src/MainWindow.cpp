/*
Copyright (c) 2018 InversePalindrome
DossierLayout - MainWindow.cpp
InversePalindrome.com
*/


#include "MainWindow.hpp"
#include "SettingsDialog.hpp"
#include "AndroidUtility.hpp"

#include <QMenu>
#include <QDialog>
#include <QSettings>
#include <QCheckBox>
#include <QLineEdit>
#include <QCompleter>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QInputDialog>
#include <QButtonGroup>
#include <QRadioButton>


MainWindow::MainWindow(const QString& user) :
    user(user),
    toolBar(new QToolBar(this)),
    view(new QGraphicsView(this)),
    stackWidget(new QStackedWidget(this)),
    titleIcon(new QLabel(this)),
    titleLabel(new QLabel(this)),
    hub(new Hub(user, this))
{
    addToolBar(toolBar);
    setContextMenuPolicy(Qt::NoContextMenu);
    setAttribute(Qt::WA_DeleteOnClose);
    setCentralWidget(view);
    setProperty("name", "MainWindow");
    setProperty("user", user);

    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QPixmap icon(":/Resources/User.png");

    titleIcon->setPixmap(icon);
    titleLabel->setText(user);
    titleLabel->setFont(QFont("MS Shell Dlg 2", 10, QFont::Bold));

    auto* titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleIcon, 0, Qt::AlignRight);
    titleLayout->addWidget(titleLabel, 0, Qt::AlignLeft);

    auto* centralLayout = new QVBoxLayout(view);
    centralLayout->addLayout(titleLayout);
    centralLayout->addWidget(stackWidget);

    stackWidget->addWidget(hub);
    stackWidget->setCurrentIndex(0);

    setupHubFunctions();
}

void MainWindow::save()
{
    hub->save(Utility::appPath() + user.toLower() + "/DataStructures.xml");

    auto* dataStructure = stackWidget->widget(1);

    if(dataStructure)
    {
        auto* list = qobject_cast<Tree*>(dataStructure);
        auto* table = qobject_cast<Table*>(dataStructure);
        auto* tree = qobject_cast<Tree*>(dataStructure);

        const auto& directory = Utility::appPath() + user + '/' + dataStructure->property("name").toString() + '/';

        if(list)
        {
            list->save(directory + "List.xml");
        }
        else if(table)
        {
            QSettings settings(directory + "Headers.ini", QSettings::IniFormat);
            settings.setValue("Horizontal", table->horizontalHeader()->saveState());
            settings.setValue("Vertical", table->verticalHeader()->saveState());

            table->save(directory + "Table.xml");
        }
        else if(tree)
        {
            QSettings settings(directory + "Header.ini", QSettings::IniFormat);
            settings.setValue("Horizontal", tree->header()->saveState());

            tree->save(directory + "Tree.xml");
        }
    }
}

void MainWindow::setupHubFunctions()
{
    auto* searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("🔍");
    searchBar->setFont(QFont("Seqoe UI Symbol"));

    auto* completer = new QCompleter(hub->getDataStructureModel(), searchBar);
    searchBar->setCompleter(completer);

    auto* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto* exitButton = new QToolButton(this);
    exitButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    exitButton->setText(tr("Exit"));
    exitButton->setIcon(QIcon(":/Resources/Exit.png"));

    auto* settingsButton = new QToolButton(this);
    settingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    settingsButton->setText(tr("Settings"));
    settingsButton->setIcon(QIcon(":/Resources/Settings.png"));

    auto* exitAction = toolBar->addWidget(exitButton);
    auto* settingsAction = toolBar->addWidget(settingsButton);
    auto* spacerAction = toolBar->addWidget(spacer);
    auto* searchAction = toolBar->addWidget(searchBar);

    QObject::connect(hub, &Hub::openDataStructure, [this, exitAction, settingsAction, spacerAction, searchAction](const QString& type, const QString& name)
    {
        Utility::setRotation(0);

        toolBar->clear();

        QPixmap icon(":/Resources/" + type + ".png");
        icon = icon.scaledToHeight(64);

        titleIcon->setPixmap(icon);
        titleLabel->setText(name);

        auto* menuButton = new QToolButton(this);
        menuButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        menuButton->setText(tr("Menu"));
        menuButton->setIcon(QIcon(":/Resources/DataStructureMenu.png"));

        toolBar->addWidget(menuButton);

        if(type == "List")
        {
            auto* list = new List(this, user, name);

            setupListFunctions(list);

            stackWidget->addWidget(list);
        }
        else if(type == "Table")
        {
            auto* table = new Table(this, user, name);

            setupTableFunctions(table);

            stackWidget->addWidget(table);
        }
        else if(type == "Tree")
        {
            auto* tree = new Tree(this, user, name);

            setupTreeFunctions(tree);

            stackWidget->addWidget(tree);
        }

        stackWidget->setCurrentIndex(1);

        QObject::connect(menuButton, &QToolButton::clicked, [this, searchAction, settingsAction, spacerAction, exitAction]
        {
            Utility::setRotation(1);

            toolBar->clear();
            toolBar->addAction(exitAction);
            toolBar->addAction(settingsAction);
            toolBar->addAction(spacerAction);
            toolBar->addAction(searchAction);

            titleIcon->setPixmap(QPixmap(":/Resources/User.png"));
            titleLabel->setText(user);

            stackWidget->currentWidget()->deleteLater();
            stackWidget->removeWidget(stackWidget->widget(1));
            stackWidget->setCurrentIndex(0);
        });
    });
    QObject::connect(searchBar, &QLineEdit::returnPressed, [this, searchBar]{ hub->findDataStructure(searchBar->text()); });
    QObject::connect(completer, static_cast<void(QCompleter::*)(const QString&)>(&QCompleter::activated), [this](const QString& text)
    {
        hub->findDataStructure(text);
    });
    QObject::connect(exitButton, &QToolButton::clicked, [this] { emit exit(); });
    QObject::connect(settingsButton, &QToolButton::clicked, [this]{ emit openSettings(); });
}

void MainWindow::setupListFunctions(List* list)
{
    auto* fileButton = new QToolButton(this);
    fileButton->setMenu(new QMenu(this));
    fileButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fileButton->setPopupMode(QToolButton::InstantPopup);
    fileButton->setText(tr("File") + "  ");
    fileButton->setIcon(QIcon(":/Resources/File.png"));
    fileButton->menu()->addAction(QIcon(":/Resources/Open.png"), "   " + tr("Open"), [this, list]
    {
        list->load(QFileDialog::getOpenFileName(this, tr("Open"), "", "Xml (*.xml)"));
    }, QKeySequence::Open);
    fileButton->menu()->addAction(QIcon(":/Resources/Download.png"), "   " + tr("Save as"), [this, list]
    {
        list->save(QFileDialog::getSaveFileName(this, tr("Save as"), "", tr("List") + " (*.pdf .xml)"));
    }, QKeySequence::Save);
    fileButton->menu()->addSeparator();
    fileButton->menu()->addAction(QIcon(":/Resources/Print.png"), "   " + tr("Print"), [list] { list->print(); }, QKeySequence::Print);
    fileButton->menu()->addSeparator();

    auto* insertButton = new QToolButton(this);
    insertButton->setMenu(new QMenu(this));
    insertButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    insertButton->setPopupMode(QToolButton::InstantPopup);
    insertButton->setText(tr("Insert") + "  ");
    insertButton->setIcon(QIcon(":/Resources/Insert.png"));
    insertButton->menu()->addAction(QIcon(":/Resources/AddRow.png"), "   " + tr("Element"), [this, list]
    {
        auto* insertDialog = new QDialog(this);

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
                list->insertElement(nameEntry->text(), Qt::ItemIsUserCheckable);
            }
            else
            {
                list->insertElement(nameEntry->text(), Qt::NoItemFlags);
            }
        }
    });

    auto* removeButton = new QToolButton(this);
    removeButton->setMenu(new QMenu(this));
    removeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    removeButton->setPopupMode(QToolButton::InstantPopup);
    removeButton->setText(tr("Remove") + "  ");
    removeButton->setIcon(QIcon(":/Resources/Remove.png"));
    removeButton->menu()->addAction(QIcon(":/Resources/RemoveRow.png"), "   " + tr("Element"), [list] { list->removeElement(); });

    auto* sortButton = new QToolButton(this);
    sortButton->setMenu(new QMenu(this));
    sortButton->menu()->addAction(tr("Ascending"), [list] { list->sort(Qt::AscendingOrder); });
    sortButton->menu()->addAction(tr("Descending"), [list] { list->sort(Qt::DescendingOrder); });
    sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    sortButton->setPopupMode(QToolButton::InstantPopup);
    sortButton->setIcon(QIcon(":/Resources/Sort.png"));
    sortButton->setText(tr("Sort") + "  ");

    toolBar->addWidget(fileButton);
    toolBar->addSeparator();
    toolBar->addWidget(insertButton);
    toolBar->addWidget(removeButton);
    toolBar->addSeparator();
    toolBar->addWidget(sortButton);
}

void MainWindow::setupTableFunctions(Table* table)
{
    auto* fileButton = new QToolButton(this);
    fileButton->setMenu(new QMenu(this));
    fileButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fileButton->setPopupMode(QToolButton::InstantPopup);
    fileButton->setText(tr("File") + "  ");
    fileButton->setIcon(QIcon(":/Resources/File.png"));
    fileButton->menu()->addAction(QIcon(":/Resources/Open.png"), "   " + tr("Open"), [this, table]
    {
       table->load(QFileDialog::getOpenFileName(this, tr("Open"), "", "Xml (*.xml)"));
    }, QKeySequence::Open);
    fileButton->menu()->addAction(QIcon(":/Resources/Download.png"), "   " + tr("Save as"), [this, table]
    {
       table->save(QFileDialog::getSaveFileName(this, tr("Save as"), "", tr("Table") + " (*.pdf .xlsx .xml)"));
    }, QKeySequence::Save);
    fileButton->menu()->addSeparator();
    fileButton->menu()->addAction(QIcon(":/Resources/Print.png"), "   " + tr("Print"), [table] { table->print(); }, QKeySequence::Print);
    fileButton->menu()->addSeparator();

    auto* insertButton = new QToolButton(this);
    insertButton->setMenu(new QMenu(this));
    insertButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    insertButton->setPopupMode(QToolButton::InstantPopup);
    insertButton->setText(tr("Insert") + "  ");
    insertButton->setIcon(QIcon(":/Resources/Insert.png"));
    insertButton->menu()->addAction(QIcon(":/Resources/AddColumn.png"), "   " + tr("Column"), [this, table]
    {
       auto* insertDialog = new QInputDialog(this);
       insertDialog->setFixedSize(500, 200);
       insertDialog->setWindowTitle(tr("Insert Column"));
       insertDialog->setLabelText(tr("Column Name"));

      if(insertDialog->exec() == QDialog::Accepted)
      {
          emit table->insertColumn(insertDialog->textValue());
      }
   });
   insertButton->menu()->addAction(QIcon(":/Resources/AddRow.png"), "   " + tr("Row"), [this, table]
   {
       auto* insertDialog = new QInputDialog(this);
       insertDialog->setFixedSize(500, 200);
       insertDialog->setWindowTitle(tr("Insert Row"));
       insertDialog->setLabelText(tr("Row Name"));

       if(insertDialog->exec() == QDialog::Accepted)
       {
          table->insertRow(insertDialog->textValue());
       }
   });

   auto* removeButton = new QToolButton(this);
   removeButton->setMenu(new QMenu(this));
   removeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   removeButton->setPopupMode(QToolButton::InstantPopup);
   removeButton->setText(tr("Remove") + "  ");
   removeButton->setIcon(QIcon(":/Resources/Remove.png"));
   removeButton->menu()->addAction(QIcon(":/Resources/RemoveColumn.png"), "   " + tr("Column"), [table] { table->removeColumn(); });
   removeButton->menu()->addAction(QIcon(":/Resources/RemoveRow.png"), "   " + tr("Row"), [table]{ table->removeRow(); });

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
   auto* columnSort = sortButton->menu()->addMenu(tr("Column"));
   columnSort->addAction(tr("Ascending"), [table] { table->sortColumn(Qt::AscendingOrder); });
   columnSort->addAction(tr("Descending"), [table] { table->sortColumn(Qt::DescendingOrder); });
   auto* rowSort = sortButton->menu()->addMenu("Row");
   rowSort->addAction(tr("Ascending"), [table] { table->sortRow(Qt::AscendingOrder); });
   rowSort->addAction(tr("Descending"), [table] { table->sortRow(Qt::DescendingOrder); });
   sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   sortButton->setPopupMode(QToolButton::InstantPopup);
   sortButton->setIcon(QIcon(":/Resources/Sort.png"));
   sortButton->setText(tr("Sort") + "  ");

   toolBar->addWidget(fileButton);
   toolBar->addSeparator();
   toolBar->addWidget(insertButton);
   toolBar->addWidget(removeButton);
   toolBar->addSeparator();
   toolBar->addWidget(operationButton);
   toolBar->addSeparator();
   toolBar->addWidget(sortButton);
   toolBar->addSeparator();
   toolBar->addAction(QIcon(":/Resources/Merge.png"), "   " + tr("Merge"), [table] { table->merge(); });
   toolBar->addAction(QIcon(":/Resources/Split.png"), "   " + tr("Split"), [table] { table->split(); });
}

void MainWindow::setupTreeFunctions(Tree* tree)
{
   auto* fileButton = new QToolButton(this);
   fileButton->setMenu(new QMenu(this));
   fileButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   fileButton->setPopupMode(QToolButton::InstantPopup);
   fileButton->setText(tr("File") + "  ");
   fileButton->setIcon(QIcon(":/Resources/File.png"));
   fileButton->menu()->addAction(QIcon(":/Resources/Open.png"), "   " + tr("Open"), [this, tree]
   {
       tree->load(QFileDialog::getOpenFileName(this, tr("Open"), "", "Xml (*.xml)"));
   }, QKeySequence::Open);
   fileButton->menu()->addAction(QIcon(":/Resources/Download.png"), "   " + tr("Save as"), [this, tree]
   {
       tree->save(QFileDialog::getSaveFileName(this, tr("Save as"), "", tr("Tree") + " (*.pdf .xml)"));
   }, QKeySequence::Save);
   fileButton->menu()->addSeparator();
   fileButton->menu()->addAction(QIcon(":/Resources/Print.png"), "   " + tr("Print"), [tree] { tree->print(); }, QKeySequence::Print);
   fileButton->menu()->addSeparator();

   auto* insertButton = new QToolButton(this);
   insertButton->setMenu(new QMenu(this));
   insertButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   insertButton->setPopupMode(QToolButton::InstantPopup);
   insertButton->setText(tr("Insert") + "  ");
   insertButton->setIcon(QIcon(":/Resources/Insert.png"));
   insertButton->menu()->addAction(QIcon(":/Resources/AddColumn.png"), "   " + tr("Column"), [this, tree]
   {
      auto* insertDialog = new QInputDialog(this);
      insertDialog->setFixedSize(500, 200);
      insertDialog->setWindowTitle(tr("Insert Column"));
      insertDialog->setLabelText(tr("Column Name"));

      if(insertDialog->exec() == QDialog::Accepted)
      {
         tree->insertColumn(insertDialog->textValue());
      }
   });
   insertButton->menu()->addAction(QIcon(":/Resources/AddNode.png"), "   " + tr("Node"), [this, tree]
   {
      auto* insertDialog = new QInputDialog(this);
      insertDialog->setFixedSize(500, 200);
      insertDialog->setWindowTitle(tr("Insert Node"));
      insertDialog->setLabelText(tr("Node Name"));

      if(insertDialog->exec() == QDialog::Accepted)
      {
          tree->insertNode(insertDialog->textValue());
      }
   });

   auto* removeButton = new QToolButton(this);
   removeButton->setMenu(new QMenu(this));
   removeButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   removeButton->setPopupMode(QToolButton::InstantPopup);
   removeButton->setText(tr("Remove") + "  ");
   removeButton->setIcon(QIcon(":/Resources/Remove.png"));
   removeButton->menu()->addAction(QIcon(":/Resources/RemoveNode.png"), "   " + tr("Node"), [tree] { tree->removeNode(); });

   auto* sortButton = new QToolButton(this);
   sortButton->setMenu(new QMenu(this));
   sortButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
   sortButton->setPopupMode(QToolButton::InstantPopup);
   sortButton->setIcon(QIcon(":/Resources/Sort.png"));
   sortButton->setText(tr("Sort") + "  ");
   sortButton->menu()->addAction(tr("Ascending"), [tree] { tree->sortColumn(Qt::AscendingOrder); });
   sortButton->menu()->addAction(tr("Descending"), [tree] { tree->sortColumn(Qt::DescendingOrder); });

   toolBar->addWidget(fileButton);
   toolBar->addSeparator();
   toolBar->addWidget(insertButton);
   toolBar->addWidget(removeButton);
   toolBar->addSeparator();
   toolBar->addWidget(sortButton);
}
