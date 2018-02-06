/*
Copyright (c) 2018 InversePalindrome
DossierTable - Tree.hpp
InversePalindrome.com
*/


#pragma once

#include <QTreeWidget>


class Tree : public QTreeWidget
{
    Q_OBJECT

public:
    Tree(QWidget* parent, const QString& directory);

public slots:

private:
    QString directory;
};
