/********************************************************************************
** Form generated from reading UI file 'TabContentView.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABCONTENTVIEW_H
#define UI_TABCONTENTVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_TabContentView
{
public:

    void setupUi(QTableView *TabContentView)
    {
        if (TabContentView->objectName().isEmpty())
            TabContentView->setObjectName(QString::fromUtf8("TabContentView"));
        TabContentView->resize(400, 300);
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(10);
        TabContentView->setFont(font);
        TabContentView->setGridStyle(Qt::SolidLine);
        TabContentView->setSortingEnabled(true);
        if (TabContentView->horizontalHeader()->objectName().isEmpty())
        if (TabContentView->verticalHeader()->objectName().isEmpty())
        TabContentView->verticalHeader()->setVisible(false);

        retranslateUi(TabContentView);

        QMetaObject::connectSlotsByName(TabContentView);
    } // setupUi

    void retranslateUi(QTableView *TabContentView)
    {
        TabContentView->setWindowTitle(QCoreApplication::translate("TabContentView", "TabContentView", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TabContentView: public Ui_TabContentView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABCONTENTVIEW_H
