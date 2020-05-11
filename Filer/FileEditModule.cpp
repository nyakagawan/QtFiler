#include "stdafx.h"
#include "FileEditModule.h"
#include "Settings.h"
#include "PlatformCompat.h"
#include <fstream>


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
				//todo: ���͍ς݃e�L�X�g������ꍇ�͏㉺�L�[�ŕ⊮����I��
			}
			else
			{
				//todo: ���͍ς݃e�L�X�g���Ȃ��ꍇ�͏㉺�L�[�ŗ�����I��
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

	finishInput();
}


