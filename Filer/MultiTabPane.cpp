#include "stdafx.h"
#include "MultiTabPane.h"
#include "TabContentView.h"

MultiTabPane::MultiTabPane(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	auto tabWidget = new QTabWidget(this);
	ui.verticalLayout_center->addWidget(tabWidget);

	_pView = new TabContentView(
		std::bind(&MultiTabPane::eventFilter, this, std::placeholders::_1, std::placeholders::_2),
		this);
	tabWidget->addTab(_pView, "tab1");
}

MultiTabPane::~MultiTabPane()
{
}

bool MultiTabPane::eventFilter(QObject *obj, QEvent *event)
{
	//qDebug() << "MultiTabPane::eventFilter:" << event->type();

	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		//qDebug("Ate key press %d", keyEvent->key());
		switch (keyEvent->key())
		{
		default:
			break;
		}
	}

	return false;
}
