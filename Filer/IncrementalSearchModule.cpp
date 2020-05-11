#include "stdafx.h"
#include "IncrementalSearchModule.h"
#include "MultiTabPane.h"
#include "TabContentView.h"
#include "Settings.h"

//-----------------------------------------------------------------------------
// IncrementalSearchModule
//-----------------------------------------------------------------------------
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

void IncrementalSearchModule::lineEditTextChanged(const QString& text)
{
	qDebug() << "IncrementalSearchModule::lineEditTextChanged: " << text;
	_pMultiTabPane->getCurrentView()->incrementalSearch(text, 0, 1);
}

void IncrementalSearchModule::lineEditReturnPressed()
{
	finishInput();
}


