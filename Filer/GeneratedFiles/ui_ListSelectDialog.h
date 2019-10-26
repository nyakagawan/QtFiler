/********************************************************************************
** Form generated from reading UI file 'ListSelectDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTSELECTDIALOG_H
#define UI_LISTSELECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ListSelectDialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;

    void setupUi(QDialog *ListSelectDialog)
    {
        if (ListSelectDialog->objectName().isEmpty())
            ListSelectDialog->setObjectName(QStringLiteral("ListSelectDialog"));
        ListSelectDialog->setWindowModality(Qt::WindowModal);
        ListSelectDialog->resize(367, 300);
        ListSelectDialog->setModal(true);
        gridLayout = new QGridLayout(ListSelectDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(ListSelectDialog);

        QMetaObject::connectSlotsByName(ListSelectDialog);
    } // setupUi

    void retranslateUi(QDialog *ListSelectDialog)
    {
        ListSelectDialog->setWindowTitle(QApplication::translate("ListSelectDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListSelectDialog: public Ui_ListSelectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTSELECTDIALOG_H
