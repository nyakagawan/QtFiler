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
			//�ҏW�I��
			finishInput();
			return true;

		case Qt::Key_Up:
		case Qt::Key_Down:
		{
			const QString& text = _pLineEdit->text();
			if (text.isEmpty())
			{
				//���͍ς݃e�L�X�g������ꍇ�͏㉺�L�[�ŕ⊮����I��
			}
			else
			{
				//���͍ς݃e�L�X�g���Ȃ��ꍇ�͏㉺�L�[�ŗ�����I��
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
		//�L����File�p�X��������A�e�L�X�g�G�f�B�^�ŊJ���Ă��
		qDebug() << "FileEditModule::lineEditReturnPressed exec file";
		PlatformCompat::ExecuteAssociatedApp(fi.absoluteFilePath());
	}
	else
	{
		//�t�@�C�����쐬����
		qDebug() << "FileEditModule::lineEditReturnPressed new file: " << fi.absoluteFilePath();
		QFile file(fi.absoluteFilePath());
		file.open(QIODevice::WriteOnly);
		file.close();
		//����ɊJ��
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
			//�ҏW�I��
			finishInput();
			return true;

		case Qt::Key_Up:
		case Qt::Key_Down:
		{
			const QString& text = _pLineEdit->text();
			if (text.isEmpty())
			{
				//���͍ς݃e�L�X�g������ꍇ�͏㉺�L�[�ŕ⊮����I��
			}
			else
			{
				//���͍ς݃e�L�X�g���Ȃ��ꍇ�͏㉺�L�[�ŗ�����I��
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
	//	//�f�B���N�g�����쐬����
	//	qDebug() << "MakeDirLineEditModule::lineEditReturnPressed new file: " << fi.absoluteDir();
	//	QDir dir(fi.absoluteDir());
	//	
	//}

	finishInput();
}


