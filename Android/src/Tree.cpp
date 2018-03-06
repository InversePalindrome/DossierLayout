/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Tree.cpp
InversePalindrome.com
*/


#include "Tree.hpp"

#include <QFile>
#include <QMenu>
#include <QPainter>
#include <QPrinter>
#include <QLineEdit>
#include <QSettings>
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
     setSelectionMode(QAbstractItemView::ContiguousSelection);

     header()->setContextMenuPolicy(Qt::CustomContextMenu);
     header()->setSectionsClickable(true);
     header()->setSortIndicatorShown(true);
     headerItem()->setTextAlignment(0, Qt::AlignCenter);
     headerItem()->setFont(0, QFont("Arial", 10, QFont::Bold));

     QObject::connect(header(), &QHeaderView::sectionDoubleClicked, this, &Tree::editHeader);
     QObject::connect(header(), &QHeaderView::customContextMenuRequested, this, &Tree::openHeaderMenu);
     QObject::connect(header(), &QHeaderView::sectionClicked, [this](int index) { header()->setSortIndicator(index, Qt::AscendingOrder);});
     QObject::connect(this, &Tree::customContextMenuRequested, this, &Tree::openNodesMenu);

     load(directory + "Tree.xml");
}

Tree::~Tree()
{
    QSettings settings(directory + "Header.ini", QSettings::IniFormat);
    settings.setValue("Horizontal", header()->saveState());

    save(directory + "Tree.xml");
}

void Tree::load(const QString& fileName)
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

    setColumnCount(headerElement.attribute("count").toInt());

    initialiseNode(headerItem(), headerElement);

    auto rootList = treeElement.elementsByTagName("Root");

    for(int i = 0; i < rootList.count(); ++i)
    {
        auto root = rootList.at(i);

        if(root.isElement())
        {
            auto rootElement = root.toElement();

            auto* item = new QTreeWidgetItem(this);
            item->setFlags(item->flags() | Qt::ItemIsEditable);

            initialiseNode(item, rootElement);
            loadNode(item, rootElement);
        }
    }

    QSettings settings(directory + "Header.ini", QSettings::IniFormat);
    header()->restoreState(settings.value("Horizontal").toByteArray());
}

void Tree::save(const QString& fileName)
{
    if(fileName.endsWith(".pdf"))
    {
        saveToPdf(fileName);
    }
    else if(fileName.endsWith(".xml"))
    {
        saveToXml(fileName);
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
    headerItem()->setFont(columnCount() - 1, QFont("MS Shell Dlg 2", 8, QFont::Bold));

    selectAll();

    for(auto node : selectedItems())
    {
        node->setBackgroundColor(columnCount() - 1, Qt::white);
        node->setTextColor(columnCount() - 1, Qt::black);
    }

    clearSelection();
}

void Tree::insertNode(const QString& name)
{
    const auto& nodes = selectedItems();

    if(nodes.isEmpty())
    {
        auto* item = new QTreeWidgetItem(this, QStringList(name));
        item->setFlags(item->flags() | Qt::ItemIsEditable);

        for(int column = 0; column < columnCount(); ++column)
        {
            item->setBackgroundColor(column, Qt::white);
            item->setTextColor(column, Qt::black);
        }
    }
    else
    {
        for(const auto& node : nodes)
        {
           auto* item = new QTreeWidgetItem(node, QStringList(name));
           item->setFlags(item->flags() | Qt::ItemIsEditable);

           for(int column = 0; column < columnCount(); ++column)
           {
               item->setBackgroundColor(column, Qt::white);
               item->setTextColor(column, Qt::black);
           }
        }
    }
}

void Tree::removeNode()
{
    const auto& nodes = selectedItems();

    for(const auto& node : nodes)
    {
        if(node->parent() || indexOfTopLevelItem(node) >= 0)
        {
            delete node;
        }
    }
}

void Tree::sortColumn(Qt::SortOrder order)
{
   sortByColumn(header()->sortIndicatorSection(), order);
}

void Tree::mousePressEvent(QMouseEvent* event)
{
    if(event->button() != Qt::LeftButton)
    {
        return;
    }

    auto* item = itemAt(event->pos());
    bool isSelected = false;

    if(item)
    {
        isSelected = item->isSelected();
    }
    else
    {
        clearSelection();
    }

    QTreeWidget::mousePressEvent(event);

    if(isSelected)
    {
        item->setSelected(false);
    }
}

void Tree::loadNode(QTreeWidgetItem* item, QDomElement& element)
{
    auto nodeElement = element.firstChildElement("Node");

    while(!nodeElement.isNull())
    {
        auto* child = new QTreeWidgetItem(item);
        child->setFlags(child->flags() | Qt::ItemIsEditable);

        initialiseNode(child, nodeElement);
        loadNode(child, nodeElement);

        nodeElement = nodeElement.nextSiblingElement("Node");
    }
}

void Tree::saveNode(QTreeWidgetItem* item, QDomDocument& doc, QDomElement& element)
{
    for(int i = 0; i < item->childCount(); ++i)
    {
        auto* child = item->child(i);
        auto nodeElement = doc.createElement("Node");

        element.appendChild(nodeElement);

        initialiseElement(child, nodeElement);
        saveNode(child, doc, nodeElement);
    }
}

void Tree::initialiseElement(QTreeWidgetItem* item, QDomElement& element)
{
    for(int column = 0; column < columnCount(); ++column)
    {
        element.setAttribute("col" + QString::number(column), item->text(column));

        QByteArray fontData;
        QDataStream fontStream(&fontData, QIODevice::ReadWrite);
        fontStream << item->font(column);
        element.setAttribute("font" + QString::number(column), QString(fontData.toHex()));

        QByteArray backgroundColorData;
        QDataStream backgroundColorStream(&backgroundColorData, QIODevice::ReadWrite);
        backgroundColorStream << item->backgroundColor(column);
        element.setAttribute("backgroundColor" + QString::number(column), QString(backgroundColorData.toHex()));

        QByteArray textColorData;
        QDataStream textColorStream(&textColorData, QIODevice::ReadWrite);
        textColorStream << item->textColor(column);
        element.setAttribute("textColor" + QString::number(column), QString(textColorData.toHex()));

        element.setAttribute("alignment" + QString::number(column), QString::number(item->textAlignment(column)));
    }
}

void Tree::initialiseNode(QTreeWidgetItem* item, QDomElement& element)
{
    for(int column = 0; column < columnCount(); ++column)
    {
       item->setText(column, element.attribute("col" + QString::number(column)));

       QDataStream fontStream(QByteArray::fromHex(element.attribute("font" + QString::number(column)).toLocal8Bit()));
       QFont font;
       fontStream >> font;
       item->setFont(column, font);

       QDataStream backgroundColorStream(QByteArray::fromHex(element.attribute("backgroundColor" + QString::number(column)).toLocal8Bit()));
       QColor backgroundColor;
       backgroundColorStream >> backgroundColor;
       item->setBackgroundColor(column, backgroundColor);

       QDataStream textColorStream(QByteArray::fromHex(element.attribute("textColor" + QString::number(column)).toLocal8Bit()));
       QColor textColor;
       textColorStream >> textColor;
       item->setTextColor(column, textColor);

       item->setTextAlignment(column, element.attribute("alignment" + QString::number(column)).toInt());
    }
}

void Tree::saveToPdf(const QString &fileName)
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

void Tree::saveToXml(const QString& fileName)
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto headerElement = doc.createElement("Header");
    headerElement.setAttribute("count", QString::number(columnCount()));
    initialiseElement(headerItem(), headerElement);

    auto treeElement = doc.createElement("Tree");

    treeElement.appendChild(headerElement);

    for(int i = 0; i < topLevelItemCount(); ++i)
    {
        auto* item = topLevelItem(i);
        auto rootElement = doc.createElement("Root");

        treeElement.appendChild(rootElement);

        initialiseElement(item, rootElement);
        saveNode(item, doc, rootElement);
    }

    doc.appendChild(treeElement);

    QFile file(fileName);

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

void Tree::openHeaderMenu(const QPoint& position)
{
    auto column = columnAt(position.x());

    auto* menu = new QMenu(this);

    menu->addAction("Font", [this, column]
    {
        const auto& font = QFontDialog::getFont(nullptr, QFont("Arial", 10), this);

        headerItem()->setFont(column, font);
    });
    menu->addAction(tr("Text Color"), [this, column]
    {
        const auto& color = QColorDialog::getColor(Qt::white, this, tr("Text Color"));

        headerItem()->setTextColor(column, color);
    });

    auto* alignment = menu->addMenu(tr("Alignment"));
    alignment->addAction(tr("Left"), [this, column]
    {
        headerItem()->setTextAlignment(column, Qt::AlignLeft | Qt::AlignVCenter);
    });
    alignment->addAction(tr("Right"), [this, column]
    {
        headerItem()->setTextAlignment(column, Qt::AlignRight | Qt::AlignVCenter);
    });
    alignment->addAction(tr("Center"), [this, column]
    {
        headerItem()->setTextAlignment(column, Qt::AlignCenter);
    });

    menu->exec(mapToGlobal(position));
}

void Tree::openNodesMenu(const QPoint& position)
{
    const auto& selectedNodes = selectedItems();
    auto column = columnAt(position.x());

    auto* menu = new QMenu(this);

    menu->addAction("Font", [this, selectedNodes, column]
    {
        const auto& font = QFontDialog::getFont(nullptr, QFont("Arial", 10), this);

        for(const auto& node : selectedNodes)
        {
           node->setFont(column, font);
        }
    });

    auto* color = menu->addMenu(tr("Color"));
    color->addAction("Background", [this, selectedNodes, column]
    {
        const auto& color = QColorDialog::getColor(Qt::white, this, tr("Background Color"));

        for(const auto& node : selectedNodes)
        {
           node->setBackgroundColor(column, color);
        }
    });
    color->addAction(tr("Text"), [this, selectedNodes, column]
    {
        const auto& color = QColorDialog::getColor(Qt::white, this, tr("Text Color"));

        for(const auto& node : selectedNodes)
        {
           node->setTextColor(column, color);
        }
    });

    auto* alignment = menu->addMenu(tr("Alignment"));
    alignment->addAction(tr("Left"), [selectedNodes, column]
    {
        for(const auto& node : selectedNodes)
        {
           node->setTextAlignment(column, Qt::AlignLeft | Qt::AlignVCenter);
        }
    });
    alignment->addAction(tr("Right"), [selectedNodes, column]
    {
        for(const auto& node : selectedNodes)
        {
           node->setTextAlignment(column, Qt::AlignRight | Qt::AlignVCenter);
        }
    });
    alignment->addAction(tr("Center"), [selectedNodes, column]
    {
        for(const auto& node : selectedNodes)
        {
           node->setTextAlignment(column, Qt::AlignCenter);
        }
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
    headerEditor->setText(headerItem()->text(logicalIndex));
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
