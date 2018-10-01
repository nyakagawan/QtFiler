#include "stdafx.h"
#include "Filer.h"
#include "MultiTabPane.h"
#include "TabContentView.h"
#include "WindowsContextMenu.h"

TabContentView::TabContentView(EventFilterHandler eventFilter, QWidget *parent)
	: QTableView(parent)
	, _fsModel(nullptr)
	, _eventFilter(eventFilter)
{
	_ui.setupUi(this);

	_fsModel = new QFileSystemModel();
	_fsModel->setRootPath("C:/");
	_fsModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
	this->setModel(_fsModel);

	this->installEventFilter(this);
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

	connect(
		this,
		SIGNAL(customContextMenuRequested(QPoint)),
		this,
		SLOT(customContextMenuRequested(QPoint)));

	connect(
		_fsModel,
		SIGNAL(directoryLoaded(QString)),
		this,
		SLOT(directoryLoaded(QString)));

	connect(
		_fsModel,
		SIGNAL(rootPathChanged(QString)),
		this,
		SLOT(rootPathChanged(QString)));

	connect(
		_fsModel,
		SIGNAL(currentChanged(QFileInfo, QFileInfo)),
		this,
		SLOT(currentChanged(QFileInfo, QFileInfo)));
}

TabContentView::~TabContentView()
{
}

void TabContentView::customContextMenuRequested(const QPoint &pos)
{
	auto index = this->indexAt(pos);
	if (index.isValid())
	{
		auto path = _fsModel->filePath(index);
		showWindowsContext(path, nullptr);
	}
	else
	{
		//開いているフォルダのContextMenuを開きたい
	}
}

bool TabContentView::eventFilter(QObject *obj, QEvent *event)
{
	if (_eventFilter(obj, event))
		return true;

	//qDebug() << "TabContentView::eventFilter:" << event->type();

	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		qDebug("Ate key press %d", keyEvent->key());
		switch (keyEvent->key())
		{
		case Qt::Key_K:
			listCursorUp();
			return true;
		case Qt::Key_J:
			listCursorDown();
			return true;
		case Qt::Key_G://Enter directory
			enterDirectory();
			return true;
		case Qt::Key_U://Up the directory hierarchy
			goUpDirectory();
			return true;
		case Qt::Key_H:
			Filer::getInstance()->getLeftTabPane()->getView()->setFocus();
			return true;
		case Qt::Key_L:
			Filer::getInstance()->getRightTabPane()->getView()->setFocus();
			return true;
		case Qt::Key_T://ContextMenu
			{
				QPoint pos;
				QString path;
				if ((keyEvent->modifiers() & Qt::KeyboardModifier::ControlModifier) != 0)
				{
					//カレントディレクトリのContextMenuOpen
					path = _fsModel->filePath(currentIndex().parent());
					pos = parentWidget()->mapToGlobal(QPoint(0, 0));
				}
				else
				{
					//カレントIndexのContextMenuOpen
					path = _fsModel->filePath(currentIndex());
					auto rect = visualRect(currentIndex());
					pos = parentWidget()->mapToGlobal(rect.bottomLeft() + QPoint(0, rect.height()));
				}
				showWindowsContext(path, &pos);
			}
			return true;
		default:
			break;
		}
	}
	// standard event processing
	return QObject::eventFilter(obj, event);
}

void TabContentView::listCursorUp()
{
	auto index = this->currentIndex();
	if (index.row() == 0)return;
	auto next = index.siblingAtRow(index.row() - 1);
	this->setCurrentIndex(next);
}

void TabContentView::listCursorDown()
{
	auto index = this->currentIndex();
	auto rowCount = this->model()->rowCount(index.parent());
	if (index.row() == rowCount - 1)return;
	auto next = index.siblingAtRow(index.row() + 1);
	this->setCurrentIndex(next);
}

void TabContentView::enterDirectory()
{
	auto index = this->currentIndex();
	auto fileInfo = _fsModel->fileInfo(index);
	if (fileInfo.isDir())
	{
		auto path = fileInfo.absoluteFilePath();
		setPath(path);
	}
}

void TabContentView::goUpDirectory()
{
	auto index = this->rootIndex();
	auto filePath = _fsModel->filePath(index);
	auto dir = QDir(filePath);
	if (dir.isRoot())
	{
		return;
	}
	auto path = _fsModel->filePath(index.parent());
	setPath(path);
}

void TabContentView::on_TabContentView_clicked(const QModelIndex &index)
{
}

void TabContentView::on_TabContentView_doubleClicked(const QModelIndex &index)
{
	auto fileInfo = _fsModel->fileInfo(index);
	auto path = fileInfo.absoluteFilePath();
	qDebug() << "on_TabContentView_doubleClicked:" << path;

	if (fileInfo.isDir())
	{
		setPath(path);
	}
}

void TabContentView::setPath(const QString& dirPath)
{
	this->clearSelection();
	_fsModel->setRootPath(dirPath);
}

void TabContentView::directoryLoaded(const QString &path)
{
	qDebug() << "directoryLoaded:" << path;

	// 前回のパスが子ディレクトリであれば、そこを初期カーソル位置とする
	QModelIndex newCursorIndex = this->rootIndex();

	// setPath() によって発生した場合はカーソル位置を再設定する
	QModelIndex newDirIndex = _fsModel->index(path);
	this->setRootIndex(newDirIndex);

	if (!newCursorIndex.isValid() || newCursorIndex.parent() != newDirIndex || newCursorIndex.row() < 0)
	{
		// 初期カーソル位置はリストの先頭
		newCursorIndex = _fsModel->index(0, 0, newDirIndex);
	}

	this->setCurrentIndex(newCursorIndex);
	this->scrollTo(newCursorIndex);
}

void TabContentView::rootPathChanged(const QString &newPath)
{
}

void TabContentView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
	qDebug() << "currentChanged:" << _fsModel->filePath(current) << ", " << _fsModel->filePath(previous);
#if 0
	QModelIndex topLeft, bottomRight;
	if (current.row() < previous.row())
	{
		topLeft = fileSystemModel->index(current.row(), 0);
		bottomRight = fileSystemModel->index(previous.row(), fileSystemModel->columnCount());
	}
	else
	{
		topLeft = fileSystemModel->index(previous.row(), 0);
		bottomRight = fileSystemModel->index(current.row(), fileSystemModel->columnCount());
	}
#endif
	this->scrollTo(currentIndex());
	setFocus();
}
