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
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilerClass
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *splitter_v;
    QSplitter *splitter_h;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_left;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_right;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_bottom;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *FilerClass)
    {
        if (FilerClass->objectName().isEmpty())
            FilerClass->setObjectName(QStringLiteral("FilerClass"));
        FilerClass->resize(772, 420);
        verticalLayout = new QVBoxLayout(FilerClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        splitter_v = new QSplitter(FilerClass);
        splitter_v->setObjectName(QStringLiteral("splitter_v"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter_v->sizePolicy().hasHeightForWidth());
        splitter_v->setSizePolicy(sizePolicy);
        splitter_v->setOrientation(Qt::Vertical);
        splitter_h = new QSplitter(splitter_v);
        splitter_h->setObjectName(QStringLiteral("splitter_h"));
        splitter_h->setOrientation(Qt::Horizontal);
        verticalLayoutWidget = new QWidget(splitter_h);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayout_left = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_left->setSpacing(6);
        verticalLayout_left->setContentsMargins(11, 11, 11, 11);
        verticalLayout_left->setObjectName(QStringLiteral("verticalLayout_left"));
        verticalLayout_left->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_left->setContentsMargins(0, 0, 0, 0);
        splitter_h->addWidget(verticalLayoutWidget);
        verticalLayoutWidget_2 = new QWidget(splitter_h);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayout_right = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_right->setSpacing(6);
        verticalLayout_right->setContentsMargins(11, 11, 11, 11);
        verticalLayout_right->setObjectName(QStringLiteral("verticalLayout_right"));
        verticalLayout_right->setContentsMargins(0, 0, 0, 0);
        splitter_h->addWidget(verticalLayoutWidget_2);
        splitter_v->addWidget(splitter_h);
        verticalLayoutWidget_3 = new QWidget(splitter_v);
        verticalLayoutWidget_3->setObjectName(QStringLiteral("verticalLayoutWidget_3"));
        verticalLayout_bottom = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_bottom->setSpacing(6);
        verticalLayout_bottom->setContentsMargins(11, 11, 11, 11);
        verticalLayout_bottom->setObjectName(QStringLiteral("verticalLayout_bottom"));
        verticalLayout_bottom->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_bottom->setContentsMargins(0, 0, 0, 0);
        textBrowser = new QTextBrowser(verticalLayoutWidget_3);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout_bottom->addWidget(textBrowser);

        splitter_v->addWidget(verticalLayoutWidget_3);

        verticalLayout->addWidget(splitter_v);


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
