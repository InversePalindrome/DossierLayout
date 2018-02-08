/*
Copyright (c) 2018 InversePalindrome
DossierTable - Tree.cpp
InversePalindrome.com
*/


#include "Tree.hpp"

#include <QFile>
#include <QMenu>
#include <QPainter>
#include <QPrinter>
#include <QLineEdit>
#include <QByteArray>
#include <QHeaderView>
#include <QFontDialog>
#include <QTextStream>
#include <QDataStream>
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

     loadTree(directory + "Tree.xml");
}

Tree::~Tree()
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto headerElement = doc.createElement("Header");

    for(int column = 0; column < columnCount(); ++column)
    {
        headerElement.setAttribute("col" + QString::number(column), headerItem()->text(column));
    }

    auto treeElement = doc.createElement("Tree");

    treeElement.appendChild(headerElement);

    for(int i = 0; i < topLevelItemCount(); ++i)
    {
        auto* item = topLevelItem(i);
        auto rootElement = doc.createElement("Root");

        for(int column = 0; column < columnCount(); ++column)
        {
            rootElement.setAttribute("col" + QString::number(column), item->text(column));

            QByteArray fontData;
            QDataStream stream(&fontData, QIODevice::ReadWrite);
            stream << item->font(column);

            auto fontElement = doc.createElement("Font" + QString::number(column));

            rootElement.appendChild(fontElement);
            fontElement.appendChild(doc.createTextNode(QString(fontData.toHex())));
        }

        treeElement.appendChild(rootElement);

        saveNode(item, doc, rootElement);
    }

    doc.appendChild(treeElement);

    QFile file(directory + "Tree.xml");

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

void Tree::loadTree(const QString& fileName)
{
    QDomDocument doc;
    QFile file(fileName);

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

    auto treeElement = doc.firstChildElement("Tree");
    auto headerElement = treeElement.firstChildElement("Header");

    setColumnCount(headerElement.attributes().count());

    for(int column = 0; column < columnCount(); ++column)
    {
        headerItem()->setText(column, headerElement.attribute("col" + QString::number(column)));
    }

    auto rootList = treeElement.elementsByTagName("Root");

    for(int i = 0; i < rootList.count(); ++i)
    {
        auto root = rootList.at(i);

        if(root.isElement())
        {
            auto rootElement = root.toElement();

            auto* item = new QTreeWidgetItem(this);
            item->setFlags(item->flags() | Qt::ItemIsEditable);

            for(int j = 0; j < rootElement.attributes().count(); ++j)
            {
               item->setText(j, rootElement.attribute("col" + QString::number(j)));

               auto fontElement = rootElement.firstChildElement("Font" + QString::number(j));

               QDataStream iStream(QByteArray::fromHex(fontElement.text().toLocal8Bit()));
               auto font = item->font(j);
               iStream >> font;

               item->setFont(j, font);
            }

            loadNode(item, rootElement);
        }
    }
}

void Tree::saveTree(const QString& fileName)
{
    if(fileName.endsWith(".pdf"))
    {
        QPrinter printer;

        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(fileName);

        QPainter painter(&printer);

        double xScale = printer.pageRect().width() / static_cast<double>(width());
        double yScale = printer.pageRect().height() / static_cast<double>(height());
        double scale = qMin(xScale, yScale);
        painter.scale(scale, scale);

        render(&painter);
    }
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
}

void Tree::insertChild(const QString& name)
{
    auto items = selectedItems();

    if(!items.isEmpty())
    {
        auto* item = new QTreeWidgetItem(items.back(), QStringList(name));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
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

void Tree::removeNode()
{
    auto items = selectedItems();

    if(!items.isEmpty())
    {
        delete items.back();
    }
}

void Tree::loadNode(QTreeWidgetItem* item, QDomElement& element)
{
    auto nodeList = element.elementsByTagName("Node");

    for(int i = 0; i < nodeList.count(); ++i)
    {
         auto node = nodeList.at(i);

         if(node.isElement())
         {
             auto nodeElement = node.toElement();
             auto* child = new QTreeWidgetItem(item);
             child->setFlags(child->flags() | Qt::ItemIsEditable);

             for(int j = 0; j < nodeElement.attributes().count(); ++j)
             {
                 child->setText(j, nodeElement.attribute("col" + QString::number(j)));
             }

             loadNode(child, nodeElement);
         }
    }
}

void Tree::saveNode(QTreeWidgetItem* item, QDomDocument& doc, QDomElement& element)
{
    for(int i = 0; i < item->childCount(); ++i)
    {
        auto* child = item->child(i);

        auto nodeElement = doc.createElement("Node");

        for(int column = 0; column < child->columnCount(); ++column)
        {
            nodeElement.setAttribute("col" + QString::number(column), child->text(column));
        }

        element.appendChild(nodeElement);

        saveNode(child, doc, nodeElement);
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
