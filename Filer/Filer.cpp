#include "stdafx.h"
#include "Filer.h"
#include "WindowsContextMenu.h"
#include "MultiTabPane.h"
#include "Settings.h"

Filer* Filer::_pInstance = nullptr;

namespace
{
	MultiTabPane* SettingUpTabs(Filer* This, QBoxLayout* layout, bool tabLeft)
	{
		auto settings = Settings::getInstance();
		auto multiTabPane = new MultiTabPane(This);
		layout->addWidget(multiTabPane);
		auto initialTabs = tabLeft ? settings->getLeftTabs() : settings->getRightTabs();
		if (initialTabs.count() > 0)
		{
			for (auto path : initialTabs)
			{
				multiTabPane->addTab(path);
			}
		}
		else
		{
			multiTabPane->addTab("C:/");
		}
		auto initialIndex = tabLeft ? settings->getLeftTabCurrentIndex() : settings->getRightTabCurrentIndex();
		multiTabPane->setCurrentTabIndex(initialIndex);
		return multiTabPane;
	}
}

Filer::Filer(QWidget *parent)
	: QWidget(parent)
{
	_pInstance = this;

	ui.setupUi(this);

	Settings::create();

	_pLeftTabPane = SettingUpTabs(this, ui.verticalLayout_left, true);
	_pRightTabPane = SettingUpTabs(this, ui.verticalLayout_right, false);

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
	{
		QStringList tabs;
		_pLeftTabPane->getTabs(tabs);
		settings->setLeftTabs(tabs);
		for (auto t : tabs)
		{
			qDebug() << "leftTab:" + t;
		}
		settings->setLeftTabCurrnetIndex(_pLeftTabPane->getCurrentTabIndex());
	}
	{
		QStringList tabs;
		_pRightTabPane->getTabs(tabs);
		settings->setRightTabs(tabs);
		for (auto t : tabs)
		{
			qDebug() << "rightTab:" + t;
		}
		settings->setRightTabCurrnetIndex(_pRightTabPane->getCurrentTabIndex());
	}
	settings->flush();

	QWidget::closeEvent(event);//super class
}
