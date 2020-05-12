#include "stdafx.h"
#include "FileEditModule.h"
#include "Settings.h"
#include "PlatformCompat.h"
#include "FolderModel.h"
#include <fstream>
#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#endif


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
				//todo: 入力済みテキストがある場合は上下キーで補完候補を選択
			}
			else
			{
				//todo: 入力済みテキストがない場合は上下キーで履歴を選択
			}
			return true;
		}

		default:
			break;
		}
	}

	return false;
}

void MakeDirLineEditModule::lineEditReturnPressed()
{
#ifdef __cpp_lib_filesystem
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
#endif
	finishInput();
}



//-----------------------------------------------------------------------------
// CopyItemLineEditModule
//-----------------------------------------------------------------------------
bool CopyItemLineEditModule::eventFilter(QObject* obj, QEvent* event)
{
	if (!_pLineEdit)
		return  false;

	//qDebug() << "CopyItemLineEditModule::eventFilter";
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
				//todo: 入力済みテキストがある場合は上下キーで補完候補を選択
			}
			else
			{
				//todo: 入力済みテキストがない場合は上下キーで履歴を選択
			}
			return true;
		}

		default:
			break;
		}
	}

	return false;
}

void CopyItemLineEditModule::lineEditReturnPressed()
{
#ifdef __cpp_lib_filesystem
	fs::path path(_pLineEdit->text().toStdWString());
	if (path.is_relative())
	{
		path = fs::path(_currentDir.toStdWString()) / path;
	}

	path = fs::absolute(path);

	if (!fs::is_directory(path))
	{
		if (!fs::create_directories(path))
		{
			return;
		}
	}

	PlatformCompat::CopyItems(_copyItemPaths, QString::fromStdWString(path));
#endif
	finishInput();
}


