#pragma once

#include <QTableView>
#include "ui_TabContentView.h"
#include <functional>

class FolderModel;

class TabContentView : public QTableView
{
	Q_OBJECT

public:
	typedef std::function<bool(QObject*, QEvent*)> EventFilterHandler;
public:
	TabContentView(EventFilterHandler eventFilter, QWidget *parent = Q_NULLPTR);
	~TabContentView();

	QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex &index, const QEvent *e = Q_NULLPTR) const Q_DECL_OVERRIDE;
	void setModel(FolderModel *folderModel);
	void selectCurrent(QItemSelectionModel::SelectionFlag selectionFlag = QItemSelectionModel::Toggle);
	void refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight);
private:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void setCursor(const QModelIndex& index);
	void listCursorUp();
	void listCursorDown();
	void enterDirectory();
	void goUpDirectory();
	void setPath(const QString &path);

private slots:
	void on_TabContentView_clicked(const QModelIndex &index);
	void on_TabContentView_doubleClicked(const QModelIndex &index);
	void directoryLoaded(const QString &path);
	void currentChanged(const QModelIndex & current, const QModelIndex & previous);
	void customContextMenuRequested(const QPoint &pos);

private:
	Ui::TabContentView _ui;
	FolderModel *_folderModel;
	EventFilterHandler _eventFilter;
	//各RootPathのカーソル位置
	QMap<QString, int> _rootPathToCursorRow;
};
