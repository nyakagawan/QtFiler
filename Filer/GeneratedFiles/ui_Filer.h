/********************************************************************************
** Form generated from reading UI file 'Filer.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILER_H
#define UI_FILER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilerClass
{
public:
    QListView *listView;

    void setupUi(QWidget *FilerClass)
    {
        if (FilerClass->objectName().isEmpty())
            FilerClass->setObjectName(QStringLiteral("FilerClass"));
        FilerClass->resize(717, 394);
        listView = new QListView(FilerClass);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(20, 40, 341, 231));
        listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setAlternatingRowColors(true);

        retranslateUi(FilerClass);

        QMetaObject::connectSlotsByName(FilerClass);
    } // setupUi

    void retranslateUi(QWidget *FilerClass)
    {
        FilerClass->setWindowTitle(QApplication::translate("FilerClass", "Filer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FilerClass: public Ui_FilerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILER_H
