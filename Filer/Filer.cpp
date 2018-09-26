#include "stdafx.h"
#include "Filer.h"
#include "WindowsContextMenu.h"
#include "MultiTabPane.h"
#include "Settings.h"

Filer::Filer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	Settings::create();

	{
		auto pane = new MultiTabPane(this);
		ui.verticalLayout_left->addWidget(pane);
	}

	{
		auto pane = new MultiTabPane(this);
		ui.verticalLayout_right->addWidget(pane);
	}

	auto settings = Settings::getInstance();
	qDebug() << settings->getWindowSize();
	resize(settings->getWindowSize());
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

void Filer::closeEvent(QCloseEvent * event)
{
	qDebug() << "closeEvent";

	auto settings = Settings::getInstance();
	settings->setWindowSize(size());
	settings->flush();

	QWidget::closeEvent(event);//super class
}
