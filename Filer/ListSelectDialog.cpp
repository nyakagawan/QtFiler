#include "stdafx.h"
#include "ListSelectDialog.h"
#include "DriveSelectView.h"

ListSelectDialog::ListSelectDialog(QWidget* parent)
	:QDialog(parent)
{
	_ui.setupUi(this);
}

ListSelectDialog::~ListSelectDialog()
{
}

//=============================================================================
//=============================================================================
DriveSelectListDialog::DriveSelectListDialog(QWidget* parent, QString currentPath)
	:ListSelectDialog(parent)
	,_pView(new DriveSelectView(this, currentPath))
{
	_ui.verticalLayout->addWidget(_pView);
	_pView->installEventFilter(this);
}

QString DriveSelectListDialog::getRootPath() const
{
	return _pView->getRootPath();
}

bool DriveSelectListDialog::eventFilter(QObject *obj, QEvent *event)
{
	//qDebug() << "MultiTabPane::eventFilter:" << event->type();
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *e = static_cast<QKeyEvent *>(event);
		switch (e->key())
		{
		case Qt::Key_Return:
		case Qt::Key_Enter:
			accept();
			return true;

		default:
			break;
		}

	}
	return false;
}
