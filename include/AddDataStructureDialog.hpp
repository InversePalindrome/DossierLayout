/*
Copyright (c) 2018 InversePalindrome
DossierLayout - AddDataStructureDialog.hpp
InversePalindrome.com
*/


#pragma once

#include <QDialog>
#include <QLineEdit>


class AddDataStructureDialog : public QDialog
{
    Q_OBJECT

public:
    AddDataStructureDialog(QWidget* parent);

private:
    QLineEdit* nameEntry;

     virtual void closeEvent(QCloseEvent* event) override;

signals:
    void addDataStructure(const QString& type, const QString& name);
};
