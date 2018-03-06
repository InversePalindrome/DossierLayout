/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Hub.cpp
InversePalindrome.com
*/


#include "Hub.hpp"

#include <QDir>
#include <QHash>
#include <QFile>
#include <QAction>
#include <QScrollBar>
#include <QBoxLayout>
#include <QToolButton>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include <QDomDocument>
#include <QStringListIterator>


Hub::Hub(const QString& user, QWidget* parent) :
    QGroupBox(tr("Data Structures"), parent),
    user(user),
    dataButtons(new QButtonGroup(this)),
    dataStructureModel(new QStringListModel(this))
{
    setFont(QFont("MS Shell Dlg 2", 10, QFont::Bold));

    auto* layout = new QVBoxLayout(this);

    layout->addWidget(createDataStructureSelector(tr("List"), "List"));
    layout->addWidget(createDataStructureSelector(tr("Table"), "Table"));
    layout->addWidget(createDataStructureSelector(tr("Tree"), "Tree"));

    load(user + "/DataStructures.xml");
}

Hub::~Hub()
{
    save(user + "/DataStructures.xml");
}

void Hub::load(const QString& fileName)
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

           addDataStructure(type, name);
       }
    }
}

void Hub::save(const QString& fileName)
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto dataStructuresElement = doc.createElement("DataStructures");

    for(const auto& button : dataButtons->buttons())
    {
        auto dataStructureElement = doc.createElement("DataStructure");

        dataStructureElement.setAttribute("type", button->property("type").toString());
        dataStructureElement.setAttribute("name", button->text());

        dataStructuresElement.appendChild(dataStructureElement);
    }

    doc.appendChild(dataStructuresElement);

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

QStringListModel* Hub::getDataStructureModel()
{
    return dataStructureModel;
}

void Hub::findDataStructure(const QString& name)
{
    auto* dataButton = qobject_cast<QToolButton*>(dataButtons->button(qHash(name)));

    if(dataButton)
    {
        sections[dataButton->property("type").toString()].first->ensureWidgetVisible(dataButton);

        dataButton->animateClick();
    }
}

QGroupBox* Hub::createDataStructureSelector(const QString& translatedType, const QString& type)
{
    auto* addButton = new QToolButton(this);
    addButton->setIcon(QIcon(":/Resources/AddIcon.png"));
    addButton->setIconSize(QSize(100, 100));
    addButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    auto* frame = new QFrame(scrollArea);
    frame->setLayout(new QHBoxLayout());

    sections[type] = qMakePair(scrollArea, frame);

    scrollArea->setWidget(sections[type].second);

    auto* selector = new QGroupBox(translatedType, this);

    auto* mainLayout = new QHBoxLayout(selector);
    mainLayout->addWidget(addButton);
    mainLayout->addSpacing(50);
    mainLayout->addWidget(scrollArea);

    QObject::connect(addButton, &QToolButton::clicked, [this, translatedType, type]
    {
        bool ok;
        const auto& name = QInputDialog::getText(this, tr("Add ") + translatedType, tr("Name:"), QLineEdit::EchoMode(), "", &ok);

        if(ok)
        {
            if(name.isEmpty())
            {
               QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Name can't be empty!"), QMessageBox::NoButton, this);
               errorMessage.exec();
            }
            else if(hasDataStructure(name))
            {
               QMessageBox errorMessage(QMessageBox::Critical, tr("Error"), tr("Data structure already exists!"), QMessageBox::NoButton, this);
               errorMessage.exec();
            }
            else
            {
               QDir().mkdir(user + '/' + name);

               addDataStructure(type, name);
            }
        }
    });

    return selector;
}

void Hub::addDataStructure(const QString& type, const QString& name)
{
    auto* dataButton = new QToolButton(this);
    dataButton->setMinimumSize(200, 200);
    dataButton->setIcon(QIcon(":/Resources/" + type + ".png"));
    dataButton->setIconSize(QSize(150, 150));
    dataButton->setText(name);
    dataButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    dataButton->setPopupMode(QToolButton::InstantPopup);
    dataButton->setProperty("type", type);

    dataButtons->addButton(dataButton, qHash(name.toLower()));

    dataStructureModel->setStringList(dataStructureModel->stringList() << name);

    auto* openAction = new QAction(QIcon(":/Resources/Open.png"), tr("   Open"), this);
    auto* deleteAction = new QAction(QIcon(":/Resources/Delete.png"), tr("   Delete"), this);

    dataButton->addAction(openAction);
    dataButton->addAction(deleteAction);

    sections[type].second->layout()->addWidget(dataButton);

    QObject::connect(openAction, &QAction::triggered, [this, type, name]
    {
        emit openDataStructure(type, name);
    });
    QObject::connect(deleteAction, &QAction::triggered, [this, dataButton, name]
    {
        QMessageBox deleteMessage(QMessageBox::Question, tr("Delete"), tr("Do you want to remove ") + '"' + name + "\"?", QMessageBox::Yes | QMessageBox::No, this);
        deleteMessage.setButtonText(QMessageBox::Yes, tr("Yes"));
        deleteMessage.setButtonText(QMessageBox::No, tr("No"));

        if(deleteMessage.exec() == QMessageBox::Yes)
        {
           dataButton->deleteLater();

           auto names = dataStructureModel->stringList();
           QMutableStringListIterator itr(names);

           while(itr.hasNext())
           {
              if(itr.next() == name)
              {
                 itr.remove();
              }
           }

           dataStructureModel->setStringList(names);

           QDir(user + '/' + name).removeRecursively();
        }
    });
}

bool Hub::hasDataStructure(const QString& name) const
{
    return dataButtons->button(qHash(name.toLower()));
}
