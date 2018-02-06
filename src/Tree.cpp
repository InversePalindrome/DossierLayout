/*
Copyright (c) 2018 InversePalindrome
DossierTable - Tree.cpp
InversePalindrome.com
*/


#include "Tree.hpp"


Tree::Tree(QWidget* parent, const QString& directory) :
    QTreeWidget(parent),
    directory(directory)
{
}
