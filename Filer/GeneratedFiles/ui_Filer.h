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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilerClass
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *FilerClass)
    {
        if (FilerClass->objectName().isEmpty())
            FilerClass->setObjectName(QStringLiteral("FilerClass"));
        FilerClass->resize(567, 367);
        verticalLayoutWidget = new QWidget(FilerClass);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(9, 9, 551, 351));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(0, 0, 0, 0);

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
