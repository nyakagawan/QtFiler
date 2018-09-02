#pragma once

#include <QtWidgets/QWidget>
#include "ui_Filer.h"

class Filer : public QWidget
{
	Q_OBJECT

public:
	Filer(QWidget *parent = Q_NULLPTR);

private:
	bool eventFilter(QObject *obj, QEvent *event);
	void listCursorUp();
	void listCursorDown();
	void enterDirectory();
	void goUpDirectory();
	void setPath(const QString &path);

private:
	Ui::FilerClass ui;
	QFileSystemModel *fileSystemModel;

private slots:
	void on_listView_clicked(const QModelIndex &index);
	void directoryLoaded(const QString &path);
	void rootPathChanged(const QString &newPath);
	void currentChanged(const QModelIndex & current, const QModelIndex & previous);
	void customContextMenuRequested(const QPoint &pos);
};
