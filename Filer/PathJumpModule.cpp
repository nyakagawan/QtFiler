#include "stdafx.h"
#include "PathJumpModule.h"
#include "MultiTabPane.h"
#include "TabContentView.h"
#include "Settings.h"

//-----------------------------------------------------------------------------
// PathJumpModule
//-----------------------------------------------------------------------------
QList<QString> PathJumpModule::_history = {};

PathJumpModule::PathJumpModule(MultiTabPane* pMultiTabPane, QLineEdit* pLineEdit)
	:_pMultiTabPane(pMultiTabPane)
	, _pLineEdit(pLineEdit)
{
	_pLineEdit->installEventFilter(this);
}

bool PathJumpModule::eventFilter(QObject* obj, QEvent* event)
{
	if (!_pLineEdit)
		return  false;

	//qDebug() << "PathJumpModule::eventFilter";
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

void PathJumpModule::startInput()
{
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

void PathJumpModule::finishInput()
{
	disconnect(_connLineEditTextChanged);
	disconnect(_connLineEditEditingFinished);
	disconnect(_connLineEditReturnPressed);

	_pLineEdit->setText("");
	_pLineEdit->setReadOnly(true);
	_pMultiTabPane->getCurrentView()->setFocus();
}

void PathJumpModule::lineEditTextChanged(const QString& text)
{
	qDebug() << "PathJumpModule::lineEditTextChanged: " << text;
}

void PathJumpModule::lineEditEditingFinished()
{
	qDebug() << "PathJumpModule::lineEditEditingFinished";
	finishInput();
}

void PathJumpModule::lineEditReturnPressed()
{
	QString text = _pLineEdit->text().replace(QChar('\\'), QChar('/'));
	qDebug() << "PathJumpModule::lineEditReturnPressed: " << text;

	auto fi = QFileInfo(text);
	QString jumpPath{};
	if (fi.isFile())
	{
		//�L����File�p�X��������A�f�B���N�g�����J���ăJ�[�\�����t�@�C���ɍ��킹�Ă��
		qDebug() << "PathJumpModule::lineEditReturnPressed isFile";
		jumpPath = text;
	}
	else
	{
		QDir dir(text);
		if (dir.exists())
		{
			//�L����Directory�p�X��������A�f�B���N�g�����J���ăJ�[�\���͏����ʒu
			qDebug() << "PathJumpModule::lineEditReturnPressed isDir";
			jumpPath = text;
		}
		else
		{
			auto dir = fi.dir();
			if (dir.exists())
			{
				qDebug() << "PathJumpModule::lineEditReturnPressed invalid but Dir";
				jumpPath = dir.absolutePath();
			}
		}
	}

	TabContentView* pView = _pMultiTabPane->getCurrentView();
	if (jumpPath.size() > 0 && pView->setPath(jumpPath))
	{
		finishInput();
	}
}
