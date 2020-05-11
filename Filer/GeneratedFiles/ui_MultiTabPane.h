/********************************************************************************
** Form generated from reading UI file 'MultiTabPane.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTITABPANE_H
#define UI_MULTITABPANE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MultiTabPane
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit_top;
    QVBoxLayout *verticalLayout_center;
    QLineEdit *lineEdit_bottom;

    void setupUi(QWidget *MultiTabPane)
    {
        if (MultiTabPane->objectName().isEmpty())
            MultiTabPane->setObjectName(QString::fromUtf8("MultiTabPane"));
        MultiTabPane->resize(387, 348);
        verticalLayout = new QVBoxLayout(MultiTabPane);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit_top = new QLineEdit(MultiTabPane);
        lineEdit_top->setObjectName(QString::fromUtf8("lineEdit_top"));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(10);
        lineEdit_top->setFont(font);
        lineEdit_top->setFocusPolicy(Qt::NoFocus);
        lineEdit_top->setAcceptDrops(false);
        lineEdit_top->setAutoFillBackground(false);
        lineEdit_top->setFrame(true);
        lineEdit_top->setReadOnly(true);

        verticalLayout->addWidget(lineEdit_top);

        verticalLayout_center = new QVBoxLayout();
        verticalLayout_center->setSpacing(6);
        verticalLayout_center->setObjectName(QString::fromUtf8("verticalLayout_center"));

        verticalLayout->addLayout(verticalLayout_center);

        lineEdit_bottom = new QLineEdit(MultiTabPane);
        lineEdit_bottom->setObjectName(QString::fromUtf8("lineEdit_bottom"));
        lineEdit_bottom->setFont(font);
        lineEdit_bottom->setReadOnly(true);

        verticalLayout->addWidget(lineEdit_bottom);


        retranslateUi(MultiTabPane);

        QMetaObject::connectSlotsByName(MultiTabPane);
    } // setupUi

    void retranslateUi(QWidget *MultiTabPane)
    {
        MultiTabPane->setWindowTitle(QCoreApplication::translate("MultiTabPane", "MultiTabPane", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MultiTabPane: public Ui_MultiTabPane {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTITABPANE_H
