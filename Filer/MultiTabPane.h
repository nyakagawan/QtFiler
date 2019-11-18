#pragma once

#include <QWidget>
#include "ui_MultiTabPane.h"

class TabContentView;
class Settings;

class IncrementalSearchModule : public QObject
{
	Q_OBJECT
public:
	IncrementalSearchModule(class MultiTabPane* pMultiTabPane, QLineEdit* pLineEdit);

	bool eventFilter(QObject* obj, QEvent* event);

	void startIncrementalSearch();

	void finishIncrementalSearch();

private Q_SLOTS:
	void lineEditBottomTextChanged(const QString& text);

	void lineEditBottomEditingFinished();

	void lineEditBottomReturnPressed();

private:
	class MultiTabPane* _pMultiTabPane = {};
	QLineEdit* _pLineEdit = {};

	QMetaObject::Connection _connLineEditBottomTextChanged = {};
	QMetaObject::Connection _connLineEditBottomEditingFinished = {};
	QMetaObject::Connection _connLineEditBottomReturnPressed = {};

	QList<QModelIndex> _matchingIndexList = {};
};


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
	IncrementalSearchModule* _pIncrementalSearch = {};
};

