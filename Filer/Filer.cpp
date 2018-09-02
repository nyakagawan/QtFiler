#include "stdafx.h"
#include "Filer.h"
#include "WindowsContextMenu.h"

Filer::Filer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	auto *model = new QFileSystemModel();
	model->setRootPath("C:/");
	model->setFilter(QDir::AllEntries | QDir::NoDot);
	ui.listView->setModel(model);
	ui.listView->setRootIndex(model->index("C:/Qt"));
	fileSystemModel = model;

	ui.listView->installEventFilter(this);
	ui.listView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.listView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(
		ui.listView,
		SIGNAL(customContextMenuRequested(QPoint)),
		this,
		SLOT(customContextMenuRequested(QPoint)));

	connect(
		fileSystemModel,
		SIGNAL(directoryLoaded(QString)),
		this,
		SLOT(directoryLoaded(QString)));

	connect(
		fileSystemModel,
		SIGNAL(rootPathChanged(QString)),
		this,
		SLOT(rootPathChanged(QString)));

	connect(
		fileSystemModel,
		SIGNAL(currentChanged(QFileInfo, QFileInfo)),
		this,
		SLOT(currentChanged(QFileInfo, QFileInfo)));
}

void Filer::customContextMenuRequested(const QPoint &pos)
{
	auto index = ui.listView->indexAt(pos);
	if (index.isValid())
	{
		auto path = fileSystemModel->filePath(index);
		showWindowsContext(path);
	}
	else
	{
		//開いているフォルダのContextMenuを開きたい
	}
}

bool Filer::eventFilter(QObject *obj, QEvent *event)
{
	//qDebug() << "eventFilter:" << event->type();

	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		//qDebug("Ate key press %d", keyEvent->key());
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
		default:
			break;
		}
	}
	// standard event processing
	return QObject::eventFilter(obj, event);
}

void Filer::listCursorUp()
{
	auto index = ui.listView->currentIndex();
	if (index.row() == 0)return;
	auto next = index.siblingAtRow(index.row() - 1);
	ui.listView->setCurrentIndex(next);
}

void Filer::listCursorDown()
{
	auto index = ui.listView->currentIndex();
	auto rowCount = ui.listView->model()->rowCount(index.parent());
	if (index.row() == rowCount - 1)return;
	auto next = index.siblingAtRow(index.row() + 1);
	ui.listView->setCurrentIndex(next);
}

void Filer::enterDirectory()
{
	auto index = ui.listView->currentIndex();
	auto fileInfo = fileSystemModel->fileInfo(index);
	if (fileInfo.isDir())
	{
		auto path = fileInfo.absoluteFilePath();
		setPath(path);
	}
}

void Filer::goUpDirectory()
{
	auto index = ui.listView->rootIndex();
	auto filePath = fileSystemModel->filePath(index);
	auto dir = QDir(filePath);
	if (dir.isRoot())
	{
		return;
	}
	auto path = fileSystemModel->filePath(index.parent());
	setPath(path);
}

void Filer::on_listView_clicked(const QModelIndex &index)
{
	auto fileInfo = fileSystemModel->fileInfo(index);
	auto path = fileInfo.absoluteFilePath();
	qDebug() << "on_listView_clicked:" << path;

	if (fileInfo.isDir())
	{
		setPath(path);
	}
}

void Filer::setPath(const QString& dirPath)
{
	ui.listView->clearSelection();
	fileSystemModel->setRootPath(dirPath);
}

void Filer::directoryLoaded(const QString &path)
{
	qDebug() << "directoryLoaded:" << path;

	// 前回のパスが子ディレクトリであれば、そこを初期カーソル位置とする
	QModelIndex newCursorIndex = ui.listView->rootIndex();

	// setPath() によって発生した場合はカーソル位置を再設定する
	QModelIndex newDirIndex = fileSystemModel->index(path);
	ui.listView->setRootIndex(newDirIndex);

	if (!newCursorIndex.isValid() || newCursorIndex.parent() != newDirIndex || newCursorIndex.row() < 0)
	{
		// 初期カーソル位置はリストの先頭
		newCursorIndex = fileSystemModel->index(0, 0, newDirIndex);
	}

	ui.listView->setCurrentIndex(newCursorIndex);
	ui.listView->scrollTo(newCursorIndex);
}

void Filer::rootPathChanged(const QString &newPath)
{
}

void Filer::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
	qDebug() << "currentChanged:" << fileSystemModel->filePath(current) << ", " << fileSystemModel->filePath(current);
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
}
