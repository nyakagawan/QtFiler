#include "stdafx.h"
#include "Filer.h"
#include "WindowsContextMenu.h"
#include "MultiTabPane.h"
#include "Settings.h"

static const int HOTKEY_EVENT_ID = 100;
Filer* Filer::_pInstance = nullptr;
Filer::EventFilter Filer::_globalShortcutEvent;

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

	ui.splitter_v->setStretchFactor(0, 1);
	ui.splitter_v->setStretchFactor(1, 0);//windowsƒŠƒTƒCƒYŽž‚É‰º•”•ª‚ÍL‚Î‚³‚È‚¢

	Settings::create();

	_pLeftTabPane = SettingUpTabs(this, ui.verticalLayout_left, true);
	_pRightTabPane = SettingUpTabs(this, ui.verticalLayout_right, false);
	_pLeftTabPane->setOpponent(_pRightTabPane);
	_pRightTabPane->setOpponent(_pLeftTabPane);

	auto settings = Settings::getInstance();
	qDebug() << "getWindowSize: " << settings->getWindowSize();
	resize(settings->getWindowSize());

	qDebug() << "getBottomPaneSize: " << settings->getBottomPaneSize();
	ui.splitter_v->setSizes(QList<int> { 1, settings->getBottomPaneSize().height() });

	initializeGlobalShortcutEvent();
}

void Filer::initializeGlobalShortcutEvent()
{
	qApp->installNativeEventFilter(&_globalShortcutEvent);
	HWND hwnd = (HWND)winId();
	//qDebug() << hwnd;
	auto result = ::RegisterHotKey(
		hwnd,
		HOTKEY_EVENT_ID,
		MOD_CONTROL/* | MOD_ALT | MOD_SHIFT*/,
		//https://docs.microsoft.com/ja-jp/windows/win32/inputdev/virtual-key-codes?redirectedfrom=MSDN
		0x31/*1*/);
	if (!result)
	{
		QMessageBox::warning(this, "Warning", QString("Failed to register hot key %1").arg(GetLastError()));
	}
}

void Filer::showEvent(QShowEvent *event)
{
	qDebug() << "showEvent";

	qDebug() << " splitter_h->sizes";
	for(auto size : ui.splitter_h->sizes())
	{
		qDebug() << size;
	}
	qDebug() << " splitter_v->sizes";
	for(auto size : ui.splitter_v->sizes())
	{
		qDebug() << size;
	}
}

void Filer::closeEvent(QCloseEvent * event)
{
	qDebug() << "closeEvent";

	auto settings = Settings::getInstance();
	settings->setWindowSize(size());
	settings->setBottomPaneSize(ui.verticalLayoutWidget_3->size());

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

	::UnregisterHotKey(HWND(winId()), HOTKEY_EVENT_ID);

	QWidget::closeEvent(event);//super class
}

bool Filer::EventFilter::nativeEventFilter(const QByteArray& eventType, void* message, long* result)
{
	Q_UNUSED(eventType)
	Q_UNUSED(result)
	// Transform the message pointer to the MSG WinAPI
	MSG* msg = reinterpret_cast<MSG*>(message);

	//qDebug() << msg->message;

	// If the message is a HotKey, then ...
	if (msg->message == WM_HOTKEY)
	{
		// ... check HotKey
		if (msg->wParam == HOTKEY_EVENT_ID)
		{
			// We inform about this to the console
			//qDebug() << "HotKey worked";
			Filer::getInstance()->activateWindow();
			return true;
		}
	}
	return false;
}
