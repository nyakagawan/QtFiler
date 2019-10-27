#pragma once

#include <QtWidgets/QWidget>
#include "ui_Filer.h"

class MultiTabPane;

class Filer : public QWidget
{
	Q_OBJECT

private:
	class EventFilter : public QAbstractNativeEventFilter
	{
	public:
		bool nativeEventFilter(const QByteArray& eventType, void* message, long* result);
	};

public:
	Filer(QWidget *parent = Q_NULLPTR);

	static Filer* getInstance() { return _pInstance; }
	MultiTabPane* getLeftTabPane() { return _pLeftTabPane; }
	MultiTabPane* getRightTabPane() { return _pRightTabPane; }

protected:
	void showEvent(QShowEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

private:
	void initializeGlobalShortcutEvent();

private:
	Ui::FilerClass ui;
	MultiTabPane* _pLeftTabPane;
	MultiTabPane* _pRightTabPane;
	static Filer* _pInstance;
	static EventFilter _globalShortcutEvent;
};
