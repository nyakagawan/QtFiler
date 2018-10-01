#pragma once

#include <QTableView>
#include "ui_TabContentView.h"
#include <functional>

class TabContentView : public QTableView
{
	Q_OBJECT

public:
	typedef std::function<bool(QObject*, QEvent*)> EventFilterHandler;
public:
	TabContentView(EventFilterHandler eventFilter, QWidget *parent = Q_NULLPTR);
	~TabContentView();

private:
	bool eventFilter(QObject *obj, QEvent *event);
	void listCursorUp();
	void listCursorDown();
	void enterDirectory();
	void goUpDirectory();
	void setPath(const QString &path);

private slots:
	void on_TabContentView_clicked(const QModelIndex &index);
	void on_TabContentView_doubleClicked(const QModelIndex &index);
	void directoryLoaded(const QString &path);
	void rootPathChanged(const QString &newPath);
	void currentChanged(const QModelIndex & current, const QModelIndex & previous);
	void customContextMenuRequested(const QPoint &pos);

private:
	Ui::TabContentView _ui;
	QFileSystemModel *_fsModel;
	EventFilterHandler _eventFilter;

};
