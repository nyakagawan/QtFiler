#pragma once

#include <QWidget>
#include "ui_MultiTabPane.h"

class TabContentView;
class Settings;

class MultiTabPane : public QWidget
{
	Q_OBJECT

public:
	MultiTabPane(QWidget *parent = Q_NULLPTR);
	~MultiTabPane();

	TabContentView* getCurrentView();

	void addTab(const QString& path);
	void getTabs(QStringList &list) const;

	int getCurrentTabIndex()const { return _tabWidget->currentIndex(); }
	void setCurrentTabIndex(int index);

	void onRootPathChanged(TabContentView* tabContentView, const QString& path);

private:
	bool eventFilter(QObject *obj, QEvent *event);
	void setCurrentTabPathText(const QString& path);

private:
	Ui::MultiTabPane ui = {};
	QTabWidget *_tabWidget = {};
	class IncrementalSearchModule* _pIncrementalSearch = {};
};

