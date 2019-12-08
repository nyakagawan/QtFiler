#include "stdafx.h"
#include "Filer.h"
#include "MultiTabPane.h"
#include "TabContentView.h"
#include "WindowsContextMenu.h"
#include "FolderViewStyledItemDelegate.h"
#include "FolderModel.h"
#include "Settings.h"
#include "ListSelectDialog.h"

TabContentView::TabContentView(QWidget *parent)
	: QTableView(parent)
	, _folderModel(new FolderModel(this))
	,_multiTabPane(qobject_cast<MultiTabPane*>(parent))
{
	Q_ASSERT(_multiTabPane != nullptr);

	_ui.setupUi(this);
	verticalHeader()->setDefaultSectionSize(24);

	setItemDelegate(new FolderViewStyledItemDelegate(this));

	_folderModel->setReadOnly(true);
	_folderModel->setDynamicSortFilter(true);
	_folderModel->setSortLocaleAware(true);
	setModel(_folderModel);

	{
		QMap<ColorRoleType, QColor> colors;

		colors[ColorRoleType::Normal] = Settings::getInstance()->getColorSetting("folderView_normal");
		colors[ColorRoleType::Normal_Selected] = Settings::getInstance()->getColorSetting("folderView_normal_selected");
		colors[ColorRoleType::Folder] = Settings::getInstance()->getColorSetting("folderView_folder");
		colors[ColorRoleType::Folder_Selected] = Settings::getInstance()->getColorSetting("folderView_folder_selected");
		colors[ColorRoleType::ReadOnly] = Settings::getInstance()->getColorSetting("folderView_readOnly");
		colors[ColorRoleType::ReadOnly_Selected] = Settings::getInstance()->getColorSetting("folderView_readOnly_selected");
		colors[ColorRoleType::Hidden] = Settings::getInstance()->getColorSetting("folderView_hidden");
		colors[ColorRoleType::Hidden_Selected] = Settings::getInstance()->getColorSetting("folderView_hidden_selected");
		colors[ColorRoleType::System] = Settings::getInstance()->getColorSetting("folderView_system");
		colors[ColorRoleType::System_Selected] = Settings::getInstance()->getColorSetting("folderView_system_selected");

		colors[ColorRoleType::Background] = Settings::getInstance()->getColorSetting("folderView_background");
		colors[ColorRoleType::Selected_Background] = Settings::getInstance()->getColorSetting("folderView_selected_background");
		_folderModel->initBrushes(colors);
	}

	setSelectionModel(_folderModel->getSelectionModel());
	setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

	connect(
		this,
		SIGNAL(customContextMenuRequested(QPoint)),
		this,
		SLOT(customContextMenuRequested(QPoint)));

	connect(
		_folderModel,
		SIGNAL(directoryLoaded(QString)),
		this,
		SLOT(directoryLoaded(QString)));

	connect(
		_folderModel,
		SIGNAL(currentChanged(QFileInfo, QFileInfo)),
		this,
		SLOT(currentChanged(QFileInfo, QFileInfo)));
}

TabContentView::~TabContentView()
{
	delete _folderModel;
}

void TabContentView::setModel(FolderModel *model)
{
	QTableView::setModel(model);

	horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

QItemSelectionModel::SelectionFlags TabContentView::selectionCommand(const QModelIndex& index, const QEvent* e/* = Q_NULLPTR*/) const
{
	QItemSelectionModel::SelectionFlags ret = QTableView::selectionCommand(index, e);

	// ここでは選択処理は行わない
	ret &= ~(QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Toggle);

	return ret;
}

void TabContentView::selectCurrent(QItemSelectionModel::SelectionFlag selectionFlag/* = QItemSelectionModel::Toggle*/)
{
	FolderModel* folderModel = qobject_cast<FolderModel*>(model());
	Q_ASSERT(folderModel);

	const QModelIndex index = currentIndex();
	if (folderModel->fileName(index) != "..")
	{
		folderModel->setSelect(index.row(), selectionFlag, index.parent());
	}
}

void TabContentView::refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
	emit dataChanged(topLeft, bottomRight);
}

QString TabContentView::getPath() const
{
	return _folderModel->rootPath();
}

void TabContentView::incrementalSearch(const QString& searchFilename, int startOffset, int searchDir)
{
	auto fnSearch = [&]()
	{
		FolderModel* folderModel = qobject_cast<FolderModel*>(model());
		int iRow = currentIndex().row();
		int rowEnd = iRow;
		iRow += startOffset;
		int rowCount = model()->rowCount(currentIndex().parent());
		qDebug() << "rowCount: " << rowCount << ", rowEnd" << rowEnd;
		do
		{
			QModelIndex index = currentIndex().siblingAtRow(iRow);
			QString fileName = folderModel->fileName(index);
			//qDebug() << "fileName: " << fileName << ", iRow: " << iRow;
			if (fileName.indexOf(searchFilename, 0, Qt::CaseInsensitive) >= 0)
			{
				qDebug() << "setCursor: " << index << ", iRow: " << iRow << ", fileName: " << fileName;
				this->setCursor(index);
				break;
			}

			iRow += searchDir;
			if (iRow >= rowCount)
			{
				iRow = 0;
			}
			else if(iRow < 0)
			{
				iRow = rowCount - 1;
			}
		} while (iRow != rowEnd);
	};

	_isIncreamentalSearching = true;
	fnSearch();
	_isIncreamentalSearching = false;
}

void TabContentView::customContextMenuRequested(const QPoint &pos)
{
	auto index = this->indexAt(pos);
	if (index.isValid())
	{
		auto path = _folderModel->filePath(index);
		showWindowsContext(path, nullptr);
	}
	else
	{
		//開いているフォルダのContextMenuを開きたい
	}
}

void TabContentView::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_K:
	case Qt::Key_Up:
		listCursorUp();
		e->accept();
		return;
	case Qt::Key_J:
	case Qt::Key_Down:
		listCursorDown();
		e->accept();
		return;
	case Qt::Key_D:
		if (e->modifiers() & Qt::KeyboardModifier::ControlModifier)
		{
			qApp->postEvent(this, new QKeyEvent(QEvent::Type::KeyPress, Qt::Key_PageDown, Qt::KeyboardModifier::NoModifier));
		}
		else if(e->modifiers() == Qt::KeyboardModifier::NoModifier)
		{
			//ドライブ変更
			auto* dialog = new DriveSelectListDialog(this, getPath());
			qDebug() << "dialog 1";
			if (dialog->exec() == QDialog::Accepted)
			{
				qDebug() << "dialog Accepted: " << dialog->getRootPath();
				setPath(dialog->getRootPath());
			}
			qDebug() << "dialog 2";
			delete dialog;
		}
		return;
	case Qt::Key_PageUp:
	case Qt::Key_PageDown:
		QTableView::keyPressEvent(e);
		return;
	case Qt::Key_Space:
		selectCurrent();
		listCursorDown();
		e->accept();
		return;
	case Qt::Key_G://Enter directory
		enterDirectory();
		e->accept();
		return;
	case Qt::Key_U:
		if (e->modifiers() & Qt::KeyboardModifier::ControlModifier)
		{
			//PageUp
			qApp->postEvent(this, new QKeyEvent(QEvent::Type::KeyPress, Qt::Key_PageUp, Qt::KeyboardModifier::NoModifier));
		}
		else
		{
			//Up the directory hierarchy
			goUpDirectory();
		}
		e->accept();
		return;
	case Qt::Key_H:
		Filer::getInstance()->getLeftTabPane()->getCurrentView()->setFocus();
		e->accept();
		return;
	case Qt::Key_L:
		Filer::getInstance()->getRightTabPane()->getCurrentView()->setFocus();
		e->accept();
		return;
	case Qt::Key_T://ContextMenu
		{
			QPoint pos;
			QString path;
			if ((e->modifiers() & Qt::KeyboardModifier::ControlModifier) != 0)
			{
				//カレントディレクトリのContextMenuOpen
				path = _folderModel->filePath(currentIndex().parent());
				pos = parentWidget()->mapToGlobal(QPoint(0, 0));
			}
			else
			{
				//カレントIndexのContextMenuOpen
				path = _folderModel->filePath(currentIndex());
				auto rect = visualRect(currentIndex());
				pos = parentWidget()->mapToGlobal(rect.bottomLeft() + QPoint(0, rect.height()));
			}
			showWindowsContext(path, &pos);
			e->accept();
			return;
		}
	case Qt::Key_Return:
		{
			// Return は Designer のショートカットの設定では効かないようなので、ハードコーディングする
			Q_ASSERT(_folderModel);

			const auto path = _folderModel->fileInfo(currentIndex()).absoluteFilePath();
			if (e->modifiers() & Qt::ShiftModifier)
			{
				//emitOpenWithApp(path);
			}
			else
			{
				//emitOpen(path);
			}

			e->accept();
			return;
		}
	default:
		break;
	}

#if 0
	if (e->modifiers() & Qt::ShiftModifier && !e->text().isEmpty())
	{
		QString text = e->text();

		if (e->key() == Qt::Key_Greater)
		{
			// SHIFT + '.'キーを押すと '>' となってしまうため、強制的に '.' にする
			text = ".";
		}

		// keyboradSearch() 内で文字を連結させず、必ず1文字目として検索させるようにするため、一時的にキー入力のインターバル値を0にする
		int backupInterval = QApplication::keyboardInputInterval();
		QApplication::setKeyboardInputInterval(0);

		keyboardSearch(text);

		QApplication::setKeyboardInputInterval(backupInterval);

		e->accept();

		return;
	}
#else
	//イクリメンタルサーチはそのモードに入っているときだけ
#endif

	e->ignore();
}

void TabContentView::setCursor(const QModelIndex& index)
{
	if (index.isValid())
	{
		setCurrentIndex(index);
		scrollTo(index);
	}
}

void TabContentView::listCursorUp()
{
	auto index = this->currentIndex();
	if (index.row() == 0)return;
	auto next = index.siblingAtRow(index.row() - 1);
	setCursor(next);
}

void TabContentView::listCursorDown()
{
	auto index = this->currentIndex();
	auto rowCount = this->model()->rowCount(index.parent());
	if (index.row() == rowCount - 1)return;
	auto next = index.siblingAtRow(index.row() + 1);
	setCursor(next);
}

void TabContentView::enterDirectory()
{
	auto index = this->currentIndex();
	if (_folderModel->isDir(index))
	{
		auto path = _folderModel->filePath(index);
		setPath(path);
	}
}

void TabContentView::goUpDirectory()
{
	auto index = this->rootIndex();
	auto filePath = _folderModel->filePath(index);
	auto dir = QDir(filePath);
	if (dir.isRoot())
	{
		return;
	}
	if (dir.cdUp())
	{
		setPath(dir.path());
	}
}

void TabContentView::on_TabContentView_clicked(const QModelIndex &index)
{
}

void TabContentView::on_TabContentView_doubleClicked(const QModelIndex &index)
{
	auto path = _folderModel->filePath(index);
	qDebug() << "on_TabContentView_doubleClicked:" << path;

	if (_folderModel->isDir(index))
	{
		setPath(path);
	}
}

bool TabContentView::setPath(const QString& path)
{
	QFileInfo fi(path);
	FolderModel* folderModel = qobject_cast<FolderModel*>(model());

	QString dirPath{};
	if (fi.isDir())
	{
		dirPath = path;
	}
	else
	{
		QDir dir(fi.dir());
		if (!dir.exists())
			return false;

		dirPath = dir.absolutePath();
	}

	//qDebug() << "dirPath: " << dirPath;

	this->clearSelection();

	_folderModel->setRootPath(dirPath);

	//タブ管理者に通知
	_multiTabPane->onRootPathChanged(this, dirPath);

	if (fi.isFile())
	{
		//ファイルの場合はカーソルを合わせる
		setCursor(folderModel->index(path));
	}

	return true;
}

void TabContentView::directoryLoaded(const QString &path)
{
	qDebug() << "directoryLoaded:" << path;

	// setPath() によって発生した場合はカーソル位置を再設定する
	QModelIndex newDirIndex = _folderModel->index(path);
	this->setRootIndex(newDirIndex);

	//カーソル位置を復元
	int row = 0;
	const auto itr = _rootPathToCursorRow.find(path);
	if (itr != _rootPathToCursorRow.constEnd())
	{
		row = qBound(0, itr.value(), _folderModel->rowCount(newDirIndex));
	}

	auto newCursorIndex = _folderModel->index(row, 0, newDirIndex);
	setCursor(newCursorIndex);
}

void TabContentView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
	qDebug() << "currentChanged:" << _folderModel->filePath(current) << ", " << _folderModel->filePath(previous);

	QModelIndex topLeft, bottomRight;
	if (current.row() < previous.row())
	{
		topLeft = _folderModel->index(current.row(), 0);
		bottomRight = _folderModel->index(previous.row(), _folderModel->columnCount());
	}
	else
	{
		topLeft = _folderModel->index(previous.row(), 0);
		bottomRight = _folderModel->index(current.row(), _folderModel->columnCount());
	}

	// カーソルが移動した箇所を再描画する
	refresh(topLeft, bottomRight);

	this->scrollTo(currentIndex());

	if (!_isIncreamentalSearching)
	{
		setFocus();
	}

	//カーソル位置を記憶しておく
	auto row = qMax(current.row(), 0);
	_rootPathToCursorRow[_folderModel->rootPath()] = row;
}
