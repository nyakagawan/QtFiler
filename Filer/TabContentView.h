#pragma once

#include <QTableView>
#include "ui_TabContentView.h"
#include <functional>

class FolderModel;
class MultiTabPane;

class TabContentView : public QTableView
{
	Q_OBJECT

public:
	TabContentView(QWidget *parent = Q_NULLPTR);
	~TabContentView();

	QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex &index, const QEvent *e = Q_NULLPTR) const Q_DECL_OVERRIDE;
	void setModel(FolderModel *folderModel);
	class FolderModel* getModel() { return _folderModel; }
	void selectCurrent(QItemSelectionModel::SelectionFlag selectionFlag = QItemSelectionModel::Toggle);
	void refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight);
	bool setPath(const QString &path);
	QString getPath() const;
	void incrementalSearch(const QString& searchFilename, int startOffset, int searchDir);

private:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void setCursor(const QModelIndex& index);
	void listCursorUp();
	void listCursorDown();
	void enterDirectory();
	void goUpDirectory();
	void removeSelectedItems();
	bool moveSelectedItems();

private slots:
	void on_TabContentView_clicked(const QModelIndex &index);
	void on_TabContentView_doubleClicked(const QModelIndex &index);
	void directoryLoaded(const QString &path);
	void currentChanged(const QModelIndex & current, const QModelIndex & previous);
	void customContextMenuRequested(const QPoint &pos);

private:
	Ui::TabContentView _ui = {};
	MultiTabPane* _multiTabPane = {};
	FolderModel* _folderModel = {};
	//各RootPathのカーソル位置
	QMap<QString, int> _rootPathToCursorRow = {};
	bool _isIncreamentalSearching = false;
};
