/*
Copyright (c) 2018 InversePalindrome
DossierTable - Tree.cpp
InversePalindrome.com
*/


#include "Tree.hpp"

#include <QMenu>
#include <QPainter>
#include <QPrinter>
#include <QLineEdit>
#include <QHeaderView>
#include <QFontDialog>
#include <QColorDialog>
#include <QPrintDialog>


Tree::Tree(QWidget* parent, const QString& directory) :
    QTreeWidget(parent),
    directory(directory)
{
     setContextMenuPolicy(Qt::CustomContextMenu);

     header()->setContextMenuPolicy(Qt::CustomContextMenu);

     QFont headerFont("Arial", 10, QFont::Bold);

     header()->setFont(headerFont);
     header()->setDefaultAlignment(Qt::AlignCenter);

     QObject::connect(header(), &QHeaderView::sectionDoubleClicked, this, &Tree::editHeader);
     QObject::connect(header(), &QHeaderView::customContextMenuRequested, this, &Tree::openHeaderMenu);
     QObject::connect(this, &Tree::customContextMenuRequested, this, &Tree::openNodesMenu);
}

Tree::~Tree()
{

}

void Tree::loadTree(const QString& fileName)
{

}

void Tree::saveTree(const QString& fileName)
{

}

void Tree::print()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);

    if(printDialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        render(&painter);
    }
}

void Tree::insertColumn(const QString& name)
{
    setColumnCount(columnCount() + 1);

    headerItem()->setText(columnCount() - 1, name);
}

void Tree::insertRoot(const QString& name)
{
    auto* item = new QTreeWidgetItem(this, QStringList(name));
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    addTopLevelItem(item);
}

void Tree::insertChild(const QString& name)
{
    auto items = selectedItems();

    if(!items.isEmpty())
    {
        auto* item = new QTreeWidgetItem(items.back(), QStringList(name));
        item->setFlags(item->flags() | Qt::ItemIsEditable);

        items.back()->addChild(item);
    }
}

void Tree::insertElement(const QString &name)
{
    auto items = selectedItems();

    if(!items.isEmpty())
    {
        items.back()->setText(columnCount() - 1, name);
    }
}

void Tree::openHeaderMenu(const QPoint& position)
{
    auto column = columnAt(position.x());

    auto* menu = new QMenu(this);

    menu->addAction("Font", [this, column]
    {
        auto font = QFontDialog::getFont(nullptr, QFont("Arial", 10), this);

        headerItem()->setFont(column, font);
    });

    menu->addAction("Text Color", [this, column]
    {
        auto color = QColorDialog::getColor(Qt::white, this, "Text Color");

        headerItem()->setTextColor(column, color);
    });

    auto* alignment = menu->addMenu("Alignment");
    alignment->addAction("Left", [this, column]
    {
        headerItem()->setTextAlignment(column, Qt::AlignLeft | Qt::AlignVCenter);
    });
    alignment->addAction("Right", [this, column]
    {
        headerItem()->setTextAlignment(column, Qt::AlignRight | Qt::AlignVCenter);
    });
    alignment->addAction("Center", [this, column]
    {
        headerItem()->setTextAlignment(column, Qt::AlignCenter);
    });

    menu->exec(mapToGlobal(position));
}

void Tree::openNodesMenu(const QPoint& position)
{
    auto* node = itemAt(position);
    auto column = columnAt(position.x());

    auto* menu = new QMenu(this);

    menu->addAction("Font", [this, node, column]
    {
        auto font = QFontDialog::getFont(nullptr, QFont("Arial", 10), this);

        node->setFont(column, font);
    });

    auto* color = menu->addMenu("Color");
    color->addAction("Background", [this, node, column]
    {
        auto color = QColorDialog::getColor(Qt::white, this, "Background Color");

        node->setBackgroundColor(column, color);
    });
    color->addAction("Text", [this, node, column]
    {
        auto color = QColorDialog::getColor(Qt::white, this, "Text Color");

        node->setTextColor(column, color);
    });

    auto* alignment = menu->addMenu("Alignment");
    alignment->addAction("Left", [this, node, column]
    {
        node->setTextAlignment(column, Qt::AlignLeft | Qt::AlignVCenter);
    });
    alignment->addAction("Right", [this, node, column]
    {
        node->setTextAlignment(column, Qt::AlignRight | Qt::AlignVCenter);
    });
    alignment->addAction("Center", [this, node, column]
    {
        node->setTextAlignment(column, Qt::AlignCenter);
    });

    menu->exec(mapToGlobal(position));
}

void Tree::editHeader(int logicalIndex)
{
    QRect rect;

    rect.setLeft(header()->sectionPosition(logicalIndex));
    rect.setWidth(header()->sectionSize(logicalIndex));
    rect.setTop(0);
    rect.setHeight(header()->height());

    rect.adjust(1, 1, -1, -1);

    auto* headerEditor = new QLineEdit(header()->viewport());
    headerEditor->move(rect.topLeft());
    headerEditor->resize(rect.size());
    headerEditor->setFrame(false);
    headerEditor->setText(header()->model()->headerData(logicalIndex, header()->orientation()).toString());
    headerEditor->setFocus();
    headerEditor->show();

    auto setData = [this, logicalIndex, headerEditor]
    {
       headerItem()->setText(logicalIndex, headerEditor->text());
       headerEditor->deleteLater();
    };

    QObject::connect(headerEditor, &QLineEdit::returnPressed, setData);
    QObject::connect(headerEditor, &QLineEdit::editingFinished, setData);
}
