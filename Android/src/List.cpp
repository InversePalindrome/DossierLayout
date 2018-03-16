/*
Copyright (c) 2018 InversePalindrome
DossierLayout - List.cpp
InversePalindrome.com
*/


#include "List.hpp"
#include "AndroidUtility.hpp"

#include <QFile>
#include <QMenu>
#include <QPrinter>
#include <QPainter>
#include <QFontDialog>
#include <QDataStream>
#include <QTextStream>
#include <QDomDocument>
#include <QColorDialog>
#include <QPrintDialog>
#include <QTapAndHoldGesture>


List::List(QWidget* parent, const QString& user, const QString& name) :
    QListWidget(parent),
    directory(Utility::appPath() + user + '/' + name + '/')
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::ContiguousSelection);
    setProperty("name", name);

    grabGesture(Qt::TapAndHoldGesture);

    load(directory + "List.xml");

    QObject::connect(this, &List::customContextMenuRequested, this, &List::openElementMenu);
}

List::~List()
{
    save(directory + "List.xml");
}

void List::load(const QString& fileName)
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

    auto listElement = doc.firstChildElement("List");
    auto elementList = listElement.elementsByTagName("Element");

    for(int i = 0; i < elementList.count(); ++i)
    {
        auto elementIndex = elementList.at(i);

        if(elementIndex.isElement())
        {
            auto elementNode = elementIndex.toElement();

            auto* element = new QListWidgetItem(elementNode.text(), this);

            if(elementNode.attribute("type") == "checkable")
            {
                element->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);

                if(elementNode.attribute("isChecked") == "true")
                {
                   element->setCheckState(Qt::Checked);
                }
                else
                {
                   element->setCheckState(Qt::Unchecked);
                }
            }
            else
            {
                element->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            }

            QDataStream fontStream(QByteArray::fromHex(elementNode.attribute("font").toLocal8Bit()));
            QFont font;
            fontStream >> font;
            element->setFont(font);

            QDataStream backgroundColorStream(QByteArray::fromHex(elementNode.attribute("backgroundColor").toLocal8Bit()));
            QColor backgroundColor;
            backgroundColorStream >> backgroundColor;
            element->setBackgroundColor(backgroundColor);

            QDataStream textColorStream(QByteArray::fromHex(elementNode.attribute("textColor").toLocal8Bit()));
            QColor textColor;
            textColorStream >> textColor;
            element->setTextColor(textColor);

            element->setTextAlignment(elementNode.attribute("alignment").toInt());
        }
    }
}

void List::save(const QString& fileName)
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

void List::print()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);

    if(printDialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        render(&painter);
    }
}

void List::insertElement(const QString& name, Qt::ItemFlags flags)
{
    auto* element = new QListWidgetItem(name, this);
    element->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | flags);
    element->setBackgroundColor(Qt::white);
    element->setTextColor(Qt::black);

    if(flags.testFlag(Qt::ItemIsUserCheckable))
    {
        element->setCheckState(Qt::Unchecked);
    }
}

void List::removeElement()
{
    auto selectedElements = selectedItems();

    for(const auto& element : selectedElements)
    {
        delete element;
    }
}

void List::sort(Qt::SortOrder order)
{
    sortItems(order);
}

bool List::event(QEvent* event)
{
    if(event->type() == QEvent::Gesture)
    {
        auto* gestureEvent = static_cast<QGestureEvent*>(event);

        if(auto* gesture = gestureEvent->gesture(Qt::TapAndHoldGesture))
        {
            const auto& position = mapFromGlobal(static_cast<QTapAndHoldGesture*>(gesture)->position().toPoint());

            if(itemAt(position))
            {
                emit customContextMenuRequested(position);
            }
        }
    }

    return QListWidget::event(event);
}

void List::saveToPdf(const QString& fileName)
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

void List::saveToXml(const QString& fileName)
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto listElement = doc.createElement("List");

    for(int row = 0; row < count(); ++row)
    {
        auto elementNode = doc.createElement("Element");
        elementNode.appendChild(doc.createTextNode(item(row)->text()));

        if(item(row)->flags().testFlag(Qt::ItemIsUserCheckable))
        {
            elementNode.setAttribute("type", "checkable");

            if(item(row)->checkState() == Qt::Checked)
            {
               elementNode.setAttribute("isChecked", "true");
            }
            else
            {
               elementNode.setAttribute("isChecked", "false");
            }
        }
        else
        {
            elementNode.setAttribute("type", "nonCheckable");;
        }

        QByteArray fontData;
        QDataStream fontStream(&fontData, QIODevice::ReadWrite);
        fontStream << item(row)->font();
        elementNode.setAttribute("font", QString(fontData.toHex()));

        QByteArray backgroundColorData;
        QDataStream backgroundColorStream(&backgroundColorData, QIODevice::ReadWrite);
        backgroundColorStream << item(row)->backgroundColor();
        elementNode.setAttribute("backgroundColor", QString(backgroundColorData.toHex()));

        QByteArray textColorData;
        QDataStream textColorStream(&textColorData, QIODevice::ReadWrite);
        textColorStream << item(row)->textColor();
        elementNode.setAttribute("textColor", QString(textColorData.toHex()));

        elementNode.setAttribute("alignment", item(row)->textAlignment());

        listElement.appendChild(elementNode);
    }

    doc.appendChild(listElement);

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

void List::openElementMenu(const QPoint& position)
{
    const auto& elements = selectedItems();

    auto* menu = new QMenu(this);

    menu->addAction("Font", [this, elements]
    {
        const auto& font = QFontDialog::getFont(nullptr, QFont("MS Shell Dlg 2", 10), this);

        for(const auto& element : elements)
        {
            element->setFont(font);
        }
    });

    auto* color = menu->addMenu(tr("Color"));
    color->addAction(tr("Background"), [this, elements]
    {
        const auto& color = QColorDialog::getColor(Qt::white, this, tr("Background Color"));

        for(const auto& element : elements)
        {
           element->setBackgroundColor(color);
        }
    });
    color->addAction(tr("Text"), [this, elements]
    {
        const auto& color = QColorDialog::getColor(Qt::black, this, tr("Text Color"));

        for(const auto& element : elements)
        {
           element->setTextColor(color);
        }
    });

    auto* alignment = menu->addMenu(tr("Alignment"));
    alignment->addAction(tr("Left"), [elements]
    {
        for(const auto& element : elements)
        {
           element->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        }
    });
    alignment->addAction(tr("Right"), [elements]
    {
        for(const auto& element : elements)
        {
           element->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        }
    });
    alignment->addAction(tr("Center"), [elements]
    {
        for(const auto& element : elements)
        {
           element->setTextAlignment(Qt::AlignCenter);
        }
    });

    menu->exec(mapToGlobal(position));
}
