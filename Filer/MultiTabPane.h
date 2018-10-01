#pragma once

#include <QWidget>
#include "ui_MultiTabPane.h"

class TabContentView;

class MultiTabPane : public QWidget
{
	Q_OBJECT

public:
	MultiTabPane(QWidget *parent = Q_NULLPTR);
	~MultiTabPane();

	TabContentView* getView() { return _pView; }

private:
	bool eventFilter(QObject *obj, QEvent *event);

private:
	Ui::MultiTabPane ui;
	TabContentView* _pView;
};
