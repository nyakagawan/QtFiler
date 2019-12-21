#include "stdafx.h"
#include "FileEditModule.h"
#include "MultiTabPane.h"
#include "TabContentView.h"
#include "Settings.h"
#include "PlatformCompat.h"
#include <iostream>
#include <fstream>

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



//-----------------------------------------------------------------------------
// FileEditModule
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// MakeDirLineEditModule
//-----------------------------------------------------------------------------
#include "FolderModel.h"

bool MakeDirLineEditModule::eventFilter(QObject* obj, QEvent* event)
{
	if (!_pLineEdit)
		return  false;

	//qDebug() << "MakeDirLineEditModule::eventFilter";
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

void MakeDirLineEditModule::lineEditEditingFinished()
{
	qDebug() << "MakeDirLineEditModule::lineEditEditingFinished";
	finishInput();
}

#include <filesystem>
namespace fs = std::filesystem;

void MakeDirLineEditModule::lineEditReturnPressed()
{
	fs::path path(_pLineEdit->text().toStdWString());
	if (path.is_relative())
	{
		path = fs::path(_currentDir.toStdWString()) / path;
	}

	path = fs::absolute(path);

	if (!fs::is_directory(path))
	{
		fs::create_directories(path);
	}

	//QDir dir(_pLineEdit->text().replace(QChar('\\'), QChar('/')));
	//if (dir.isRelative)
	//{
	//	dir = QDir(_currentDir)
	//}
	//QString text = dir.absoluteFilePath();
	//qDebug() << "MakeDirLineEditModule::lineEditReturnPressed: " << text;

	//_pMultiTabPane->getCurrentView()->getModel()->mkdir()

	//auto fi = QFileInfo(text);
	//if (!fi.isDir())
	//{
	//	//ディレクトリを作成する
	//	qDebug() << "MakeDirLineEditModule::lineEditReturnPressed new file: " << fi.absoluteDir();
	//	QDir dir(fi.absoluteDir());
	//	
	//}

	finishInput();
}


