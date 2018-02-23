/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Hub.hpp
InversePalindrome.com
*/


#pragma once

#include <QMap>
#include <QFrame>
#include <QGroupBox>
#include <QScrollArea>
#include <QButtonGroup>
#include <QStringListModel>


class Hub : public QGroupBox
{
    Q_OBJECT

public:
    Hub(const QString& user, QWidget* parent = nullptr);
    ~Hub();

    void load(const QString& fileName);
    void save(const QString& fileName);

    QStringListModel* getDataStructureModel();

    void findDataStructure(const QString& name);

private:
    QString user;
    QButtonGroup* dataButtons;
    QStringListModel* dataStructureModel;

    QMap<QString, QPair<QScrollArea*, QFrame*>> sections;

    QGroupBox* createDataStructureSelector(const QString& translatedType, const QString& type);
    void addDataStructure(const QString& type, const QString& name);

    bool hasDataStructure(const QString& name) const;

signals:
    void openDataStructure(const QString& type, const QString& name);
};
