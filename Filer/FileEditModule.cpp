#include "stdafx.h"
#include "FileEditModule.h"
#include "MultiTabPane.h"
#include "TabContentView.h"
#include "Settings.h"
#include "PlatformCompat.h"
#include <iostream>
#include <fstream>

//-----------------------------------------------------------------------------
// FileEditModule
//-----------------------------------------------------------------------------

FileEditModule::FileEditModule(MultiTabPane* pMultiTabPane, QLineEdit* pLineEdit)
	:_pMultiTabPane(pMultiTabPane)
	, _pLineEdit(pLineEdit)
{
	_pLineEdit->installEventFilter(this);
}

bool FileEditModule::eventFilter(QObject* obj, QEvent* event)
{
	if (!_pLineEdit)
		return  false;

	//qDebug() << "FileEditModule::eventFilter";
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* e = static_cast<QKeyEvent*>(event);
		//qDebug("Ate key press %d", e->key());
		switch (e->key())
		{
		case Qt::Key_Escape:
			//編集終了
			finishInput();
			return true;

		case Qt::Key_Up:
		case Qt::Key_Down:
		{
			const QString& text = _pLineEdit->text();
			if (text.isEmpty())
			{
				//入力済みテキストがある場合は上下キーで補完候補を選択
			}
			else
			{
				//入力済みテキストがない場合は上下キーで履歴を選択
			}
			return true;
		}

		default:
			break;
		}
	}

	return false;
}

void FileEditModule::startInput(const QString& currentDir)
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

void FileEditModule::finishInput()
{
	disconnect(_connLineEditTextChanged);
	disconnect(_connLineEditEditingFinished);
	disconnect(_connLineEditReturnPressed);

	_pLineEdit->setText("");
	_pLineEdit->setReadOnly(true);
	_pMultiTabPane->getCurrentView()->setFocus();
}

void FileEditModule::lineEditTextChanged(const QString& text)
{
	qDebug() << "FileEditModule::lineEditTextChanged: " << text;
}

void FileEditModule::lineEditEditingFinished()
{
	qDebug() << "FileEditModule::lineEditEditingFinished";
	finishInput();
}

void FileEditModule::lineEditReturnPressed()
{
	QDir dir(_currentDir);
	QString text = dir.absoluteFilePath(_pLineEdit->text().replace(QChar('\\'), QChar('/')));
	qDebug() << "FileEditModule::lineEditReturnPressed: " << text;

	auto fi = QFileInfo(text);
	if (fi.isFile())
	{
		//有効なFileパスだったら、テキストエディタで開いてやる
		qDebug() << "FileEditModule::lineEditReturnPressed exec file";
		PlatformCompat::ExecuteAssociatedApp(fi.absoluteFilePath());
	}
	else
	{
		//ファイルを作成する
		qDebug() << "FileEditModule::lineEditReturnPressed new file: " << fi.absoluteFilePath();
		QFile file(fi.absoluteFilePath());
		file.open(QIODevice::WriteOnly);
		file.close();
		//さらに開く
		PlatformCompat::ExecuteAssociatedApp(fi.absoluteFilePath());
	}

	finishInput();
}

