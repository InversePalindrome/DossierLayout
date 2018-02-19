/*
Copyright (c) 2018 InversePalindrome
DossierLayout - Tree.hpp
InversePalindrome.com
*/


#pragma once

#include <QTreeWidget>
#include <QMouseEvent>
#include <QDomDocument>


class Tree : public QTreeWidget
{
    Q_OBJECT

public:
    Tree(QWidget* parent, const QString& directory);
    ~Tree();

public slots:
    void load(const QString& fileName);
    void save(const QString& fileName);

    void print();

    void insertColumn(const QString& name);
    void insertNode(const QString& name);

    void removeNode();

    void sortColumn(Qt::SortOrder order);

private:
    QString directory;

    virtual void mousePressEvent(QMouseEvent* event) override;

    void loadNode(QTreeWidgetItem* item, QDomElement& element);
    void saveNode(QTreeWidgetItem* item, QDomDocument& doc, QDomElement& element);

    void initialiseElement(QTreeWidgetItem* item, QDomElement& element);
    void initialiseNode(QTreeWidgetItem* item, QDomElement& element);

    void saveToPdf(const QString& fileName);
    void saveToXml(const QString& fileName);

private slots:
    void openHeaderMenu(const QPoint& position);
    void openNodesMenu(const QPoint& position);
    void editHeader(int logicalIndex);
};
