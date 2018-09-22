#pragma once

#include <QTableView>
#include "ui_TabContentView.h"

class TabContentView : public QTableView
{
	Q_OBJECT

public:
	TabContentView(QWidget *parent = Q_NULLPTR);
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
	Ui::TabContentView ui;
	QFileSystemModel *fileSystemModel;

};
