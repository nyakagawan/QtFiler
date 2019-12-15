#pragma once
#include <qdialog.h>
#include "ui_ListSelectDialog.h"

class ListSelectDialog : public QDialog
{
public:
	ListSelectDialog(QWidget* parent);
	virtual ~ListSelectDialog();

protected:
	Ui::ListSelectDialog _ui = {};
	QStandardItemModel* _pModel = {};
};

class DriveSelectListDialog : public ListSelectDialog
{
public:
	DriveSelectListDialog(QWidget* parent, QString currentPath);

	QString getRootPath() const;

	bool eventFilter(QObject *obj, QEvent *event);

private slots:
	void onCursorMovedByHeadChar();

private:
	class DriveSelectView* _pView = {};
};