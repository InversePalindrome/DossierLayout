/*
Copyright (c) 2018 InversePalindrome
DossierTable - AddDataStructureDialog.hpp
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

    virtual void closeEvent(QCloseEvent* event) override;

private:
    QLineEdit* nameEntry;

signals:
    void addDataStructure(const QString& type, const QString& name);
};
