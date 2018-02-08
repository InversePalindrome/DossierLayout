/*
Copyright (c) 2018 InversePalindrome
DossierTable - Tree.hpp
InversePalindrome.com
*/


#pragma once

#include <QTreeWidget>
#include <QDomDocument>


class Tree : public QTreeWidget
{
    Q_OBJECT

public:
    Tree(QWidget* parent, const QString& directory);
    ~Tree();

public slots:
    void loadTree(const QString& fileName);
    void saveTree(const QString& fileName);

    void print();

    void insertColumn(const QString& name);
    void insertRoot(const QString& name);
    void insertChild(const QString& name);
    void insertElement(const QString& name);

    void removeNode();

private:
    QString directory;

    void loadNode(QTreeWidgetItem* item, QDomElement& element);
    void saveNode(QTreeWidgetItem* item, QDomDocument& doc, QDomElement& element);

private slots:
    void openHeaderMenu(const QPoint& position);
    void openNodesMenu(const QPoint& position);
    void editHeader(int logicalIndex);
};
