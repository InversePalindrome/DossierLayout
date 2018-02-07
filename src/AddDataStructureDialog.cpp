/*
Copyright (c) 2018 InversePalindrome
DossierTable - AddDataStructureDialog.cpp
InversePalindrome.com
*/


#include "AddDataStructureDialog.hpp"

#include <QIcon>
#include <QFont>
#include <QLabel>
#include <QBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>


AddDataStructureDialog::AddDataStructureDialog(QWidget* parent) :
    QDialog(parent),
    nameEntry(new QLineEdit(this))
{
    setFixedSize(600, 350);
    setWindowTitle("Add Data Structure");

    QFont labelFont("Arial", 11, QFont::Bold);
    QFont entryFont("Arial", 11);

    auto* nameLabel = new QLabel("Name");
    nameLabel->setFont(labelFont);

    nameEntry->setFont(entryFont);

    auto* tableButton = new QRadioButton("Table");
    tableButton->setIcon(QIcon(":/Resources/Table.png"));
    tableButton->setFont(labelFont);
    tableButton->setChecked(true);

    auto* treeButton = new QRadioButton("Tree");
    treeButton->setIcon(QIcon(":/Resources/Tree.png"));
    treeButton->setFont(labelFont);

    auto* buttonGroup = new QButtonGroup();
    buttonGroup->addButton(tableButton);
    buttonGroup->addButton(treeButton);

    auto* addButton = new QPushButton("Add");

    auto* layout = new QVBoxLayout(this);

    layout->addWidget(nameLabel);
    layout->addWidget(nameEntry);
    layout->addWidget(tableButton);
    layout->addWidget(treeButton);
    layout->addWidget(addButton);

    setLayout(layout);

    QObject::connect(addButton, &QPushButton::clicked, [this, buttonGroup]
    {
        emit addDataStructure(buttonGroup->checkedButton()->text(), nameEntry->text());

        close();
    });
}

void AddDataStructureDialog::closeEvent(QCloseEvent* event)
{
    nameEntry->clear();

    QWidget::closeEvent(event);
}
