#include "stdafx.h"
#include "Filer.h"
#include "WindowsContextMenu.h"
#include "TabContentView.h"

Filer::Filer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	auto tabWidget = new QTabWidget(this);
	ui.verticalLayout->addWidget(tabWidget);

	auto tableView = new TabContentView(this);
	tabWidget->addTab(tableView, "tab1");
}
