#include "stdafx.h"
#include "IncrementalSearchModule.h"
#include "MultiTabPane.h"
#include "TabContentView.h"
#include "Settings.h"

//-----------------------------------------------------------------------------
// IncrementalSearchModule
//-----------------------------------------------------------------------------
IncrementalSearchModule::IncrementalSearchModule(MultiTabPane* pMultiTabPane, QLineEdit* pLineEdit)
	:_pMultiTabPane(pMultiTabPane)
	, _pLineEdit(pLineEdit)
{
	_pLineEdit->installEventFilter(this);
}

bool IncrementalSearchModule::eventFilter(QObject* obj, QEvent* event)
{
	if (!_pLineEdit)
		return  false;

	//qDebug() << "IncrementalSearchModule::eventFilter";
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* e = static_cast<QKeyEvent*>(event);
		//qDebug("Ate key press %d", e->key());
		switch (e->key())
		{
		case Qt::Key_Escape:
			//•ÒWI—¹
			finishInput();
			return true;

		case Qt::Key_Up:
		case Qt::Key_Down:
		{
			const QString& searchText = _pLineEdit->text();
			if (!searchText.isEmpty())
			{
				int searchDir = e->key() == Qt::Key_Up ? -1 : 1;
				_pMultiTabPane->getCurrentView()->incrementalSearch(searchText, searchDir, searchDir);
			}
			return true;
		}

		default:
			break;
		}
	}

	return false;
}

void IncrementalSearchModule::startInput()
{
	_pLineEdit->setText("");
	_pLineEdit->setReadOnly(false);
	_pLineEdit->setFocus();

	_connLineEditBottomTextChanged = connect(
		_pLineEdit,
		SIGNAL(textChanged(QString)),
		this,
		SLOT(lineEditBottomTextChanged(QString)));

	_connLineEditBottomEditingFinished = connect(
		_pLineEdit,
		SIGNAL(editingFinished()),
		this,
		SLOT(lineEditBottomEditingFinished()));

	_connLineEditBottomReturnPressed = connect(
		_pLineEdit,
		SIGNAL(returnPressed()),
		this,
		SLOT(lineEditBottomReturnPressed()));
}

void IncrementalSearchModule::finishInput()
{
	disconnect(_connLineEditBottomTextChanged);
	disconnect(_connLineEditBottomEditingFinished);
	disconnect(_connLineEditBottomReturnPressed);

	QString text = _pLineEdit->text();

	_pLineEdit->setText("");
	_pLineEdit->setReadOnly(true);
	_pMultiTabPane->getCurrentView()->setFocus();
}

void IncrementalSearchModule::lineEditBottomTextChanged(const QString& text)
{
	qDebug() << "lineEditBottomTextChanged: " << text;
	_pMultiTabPane->getCurrentView()->incrementalSearch(text, 0, 1);
}

void IncrementalSearchModule::lineEditBottomEditingFinished()
{
	qDebug() << "lineEditBottomEditingFinished";
	finishInput();
}

void IncrementalSearchModule::lineEditBottomReturnPressed()
{
	qDebug() << "lineEditBottomReturnPressed";
	finishInput();
}

