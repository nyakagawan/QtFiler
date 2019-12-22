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

	MultiTabPane* getOpponent() const { return _opponent; }
	void setOpponent(MultiTabPane* pOpp) { _opponent = pOpp; }

	void onRootPathChanged(TabContentView* tabContentView, const QString& path);

private:
	bool eventFilter(QObject *obj, QEvent *event);
	void setCurrentTabPathText(const QString& path);

private:
	Ui::MultiTabPane ui = {};
	MultiTabPane* _opponent {};
	QTabWidget *_tabWidget = {};
	class IncrementalSearchModule* _pIncrementalSearch = {};
	class PathJumpModule* _pPathJump = {};
	class FileEditModule* _pFileEdit = {};
	class MakeDirLineEditModule* _pMakeDir = {};
};

