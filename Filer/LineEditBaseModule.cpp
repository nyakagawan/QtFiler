#include "stdafx.h"
#include "ListEditBaseModule.h"
#include "MultiTabPane.h"
#include "TabContentView.h"

//-----------------------------------------------------------------------------
// LineEditBaseModule
//-----------------------------------------------------------------------------

LineEditBaseModule::LineEditBaseModule(MultiTabPane* pMultiTabPane, QLineEdit* pLineEdit)
	:_pMultiTabPane(pMultiTabPane)
	, _pLineEdit(pLineEdit)
{
	_pLineEdit->installEventFilter(this);
}

void LineEditBaseModule::startInput(const QString& currentDir)
{
	_currentDir = currentDir;

	_pLineEdit->setText("");
	_pLineEdit->setReadOnly(false);
	_pLineEdit->setFocus();

	_connLineEditTextChanged = connect(
		_pLineEdit,
		SIGNAL(textChanged(QString)),
		this,
		SLOT(lineEditTextChanged(QString)));

	_connLineEditEditingFinished = connect(
		_pLineEdit,
		SIGNAL(editingFinished()),
		this,
		SLOT(lineEditEditingFinished()));

	_connLineEditReturnPressed = connect(
		_pLineEdit,
		SIGNAL(returnPressed()),
		this,
		SLOT(lineEditReturnPressed()));
}

void LineEditBaseModule::finishInput()
{
	disconnect(_connLineEditTextChanged);
	disconnect(_connLineEditEditingFinished);
	disconnect(_connLineEditReturnPressed);

	_pLineEdit->setText("");
	_pLineEdit->setReadOnly(true);
	_pMultiTabPane->getCurrentView()->setFocus();
}

void LineEditBaseModule::lineEditTextChanged(const QString& text)
{
}

void LineEditBaseModule::lineEditEditingFinished()
{
	finishInput();
}

void LineEditBaseModule::lineEditReturnPressed()
{
}


