/*
Copyright (c) 2018 InversePalindrome
DossierLayout - List.hpp
InversePalindrome.com
*/


#pragma once

#include <QEvent>
#include <QListWidget>


class List : public QListWidget
{
    Q_OBJECT

public:
    List(QWidget* parent, const QString& directory);
    ~List();

    void load(const QString& fileName);
    void save(const QString& fileName);

    void print();

    void insertElement(const QString& name, Qt::ItemFlags flags);
    void removeElement();

    void sort(Qt::SortOrder order);

private:
    QString directory;

    virtual bool event(QEvent* event) override;

    void saveToPdf(const QString& fileName);
    void saveToXml(const QString& fileName);

private slots:
    void openElementMenu(const QPoint& position);
};
