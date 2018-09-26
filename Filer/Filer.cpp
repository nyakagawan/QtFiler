#include "stdafx.h"
#include "Filer.h"
#include "WindowsContextMenu.h"
#include "TabContentView.h"

Filer::Filer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	{
		auto tabWidget = new QTabWidget(this);
		ui.verticalLayout_left->addWidget(tabWidget);

		auto tableView = new TabContentView(this);
		tabWidget->addTab(tableView, "tab1");
	}

	{
		auto tabWidget = new QTabWidget(this);
		ui.verticalLayout_right->addWidget(tabWidget);

		auto tableView = new TabContentView(this);
		tabWidget->addTab(tableView, "tab1");
	}
}

void Filer::showEvent(QShowEvent *event)
{
	qDebug() << "showEvent";

	qDebug() << " splitter_h->sizes";
	for each (auto size in ui.splitter_h->sizes())
	{
		qDebug() << size;
	}
	qDebug() << " splitter_v->sizes";
	for each (auto size in ui.splitter_v->sizes())
	{
		qDebug() << size;
	}
}
