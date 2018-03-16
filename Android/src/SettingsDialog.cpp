/*
Copyright (c) 2018 InversePalindrome
DossierLayout - SettingsDialog.cpp
InversePalindrome.com
*/


#include "SettingsDialog.hpp"
#include "AndroidUtility.hpp"

#include <QFile>
#include <QBoxLayout>
#include <QTextStream>
#include <QDomDocument>

#include <string.h>


SettingsDialog::SettingsDialog(QWidget* parent) :
    QDialog(parent, Qt::Window),
    formLayout(new QFormLayout()),
    styleLabel(new QLabel(tr("Style:"), this)),
    languageLabel(new QLabel(tr("Language:"), this)),
    styleChoices(new QComboBox(this)),
    languageChoices(new QComboBox(this)),
    doneButton(new QPushButton(tr("Done"), this))
{
    setAttribute(Qt::WA_DeleteOnClose);

    auto* settingsLabel = new QLabel(this);
    settingsLabel->setPixmap(QPixmap(":/Resources/Settings.png"));

    styleChoices->setProperty("0", "Regular");
    styleChoices->setProperty("1", "Light");
    styleChoices->setProperty("2", "Dark");

    styleChoices->addItem(tr("Regular"));
    styleChoices->addItem(tr("Light"));
    styleChoices->addItem(tr("Dark"));

    languageChoices->setProperty("0", "English");
    languageChoices->setProperty("1", "Spanish");

    languageChoices->addItem("English");
    languageChoices->setItemIcon(0, QIcon(":/Resources/English.png"));
    languageChoices->addItem("Español");
    languageChoices->setItemIcon(1, QIcon(":/Resources/Spanish.png"));

    auto* layout = new QVBoxLayout(this);

    formLayout->addRow(styleLabel, styleChoices);
    formLayout->addRow(languageLabel, languageChoices);

    layout->addWidget(settingsLabel, 0, Qt::AlignCenter);
    layout->addLayout(formLayout);
    layout->addWidget(doneButton);

    QObject::connect(styleChoices, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index)
    {
        emit changeStyle(styleChoices->property(QString::number(index).toStdString().c_str()).toString());
    });
    QObject::connect(languageChoices, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index)
    {
        emit changeLanguage(languageChoices->property(QString::number(index).toStdString().c_str()).toString());
    });
    QObject::connect(doneButton, &QPushButton::clicked, [this] { emit closeSettings(); });

    load(Utility::appPath() + "Settings.xml");
}

SettingsDialog::~SettingsDialog()
{
    save(Utility::appPath() + "Settings.xml");
}

void SettingsDialog::load(const QString& fileName)
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

    auto settingsElement = doc.firstChildElement("Settings");

    styleChoices->setCurrentIndex(settingsElement.firstChildElement("Style").attribute("index").toInt());
    languageChoices->setCurrentIndex(settingsElement.firstChildElement("Language").attribute("index").toInt());
}

void SettingsDialog::save(const QString& fileName)
{
    QDomDocument doc;

    auto dec = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(dec);

    auto settingsElement = doc.createElement("Settings");

    auto styleElement = doc.createElement("Style");
    styleElement.setAttribute("index", styleChoices->currentIndex());
    styleElement.appendChild(doc.createTextNode(styleChoices->property(QString::number(styleChoices->currentIndex()).toLatin1().data()).toString()));

    auto languageElement = doc.createElement("Language");
    languageElement.setAttribute("index", languageChoices->currentIndex());
    languageElement.appendChild(doc.createTextNode(languageChoices->property(QString::number(languageChoices->currentIndex()).toLatin1().data()).toString()));

    settingsElement.appendChild(styleElement);
    settingsElement.appendChild(languageElement);

    doc.appendChild(settingsElement);

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

void SettingsDialog::changeEvent(QEvent* event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        retranslateUi();
    }

    QDialog::changeEvent(event);
}

void SettingsDialog::retranslateUi()
{
    doneButton->setText(tr("Done"));

    styleChoices->setItemText(0, tr("Regular"));
    styleChoices->setItemText(1, tr("Light"));
    styleChoices->setItemText(2, tr("Dark"));

    styleLabel->setText(tr("Style:"));
    languageLabel->setText(tr("Language:"));
}
