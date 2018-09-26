#include "stdafx.h"
#include "MultiTabPane.h"
#include "TabContentView.h"

MultiTabPane::MultiTabPane(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	auto tabWidget = new QTabWidget(this);
	ui.verticalLayout_center->addWidget(tabWidget);

	auto tableView = new TabContentView(this);
	tabWidget->addTab(tableView, "tab1");
}

MultiTabPane::~MultiTabPane()
{
}
