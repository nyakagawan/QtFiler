#include "stdafx.h"
#include "Filer.h"
#include "WindowsContextMenu.h"
#include "MultiTabPane.h"
#include "Settings.h"

Filer* Filer::_pInstance = nullptr;

Filer::Filer(QWidget *parent)
	: QWidget(parent)
{
	_pInstance = this;

	ui.setupUi(this);

	Settings::create();

	{
		_pLeftTabPane = new MultiTabPane(this);
		ui.verticalLayout_left->addWidget(_pLeftTabPane);
	}

	{
		_pRightTabPane = new MultiTabPane(this);
		ui.verticalLayout_right->addWidget(_pRightTabPane);
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
