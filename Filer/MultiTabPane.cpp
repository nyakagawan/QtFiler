#include "stdafx.h"
#include "MultiTabPane.h"
#include "TabContentView.h"
#include "IncrementalSearchModule.h"
#include "PathJumpModule.h"
#include "FileEditModule.h"
#include "Settings.h"

//-----------------------------------------------------------------------------
// MultiTabPane
//-----------------------------------------------------------------------------
MultiTabPane::MultiTabPane(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	_tabWidget = new QTabWidget(this);
	ui.verticalLayout_center->addWidget(_tabWidget);

	ui.lineEdit_top->setReadOnly(true);
	ui.lineEdit_bottom->setReadOnly(true);

	_pIncrementalSearch = new IncrementalSearchModule(this, ui.lineEdit_bottom);
	_pPathJump = new PathJumpModule(this, ui.lineEdit_bottom);
	_pFileEdit = new FileEditModule(this, ui.lineEdit_bottom);
	_pMakeDir = new MakeDirLineEditModule(this, ui.lineEdit_bottom);
	_pCopyItem = new CopyItemLineEditModule(this, ui.lineEdit_bottom);
}

MultiTabPane::~MultiTabPane()
{
	delete _pIncrementalSearch;
	delete _pPathJump;
	delete _pFileEdit;
	delete _pMakeDir;
	delete _pCopyItem;
}

TabContentView* MultiTabPane::getCurrentView()
{
	return qobject_cast<TabContentView*>(_tabWidget->currentWidget());
}

void MultiTabPane::getTabs(QStringList& list) const
{
	for (int i = 0; i < _tabWidget->count(); ++i)
	{
		auto tabContentView = qobject_cast<TabContentView*>(_tabWidget->widget(i));
		list.append(tabContentView->getPath());
	}
}

void MultiTabPane::setCurrentTabIndex(int index)
{
	if (index < 0 || index >= _tabWidget->count())
	{
		return;
	}
	_tabWidget->setCurrentIndex(index);
	setCurrentTabPathText(getCurrentView()->getPath());
}

void MultiTabPane::addTab(const QString& path)
{
	auto tabContentView = new TabContentView(this);
	tabContentView->installEventFilter(this);
	_tabWidget->addTab(tabContentView, "");
	tabContentView->setPath(path);
}

void MultiTabPane::onRootPathChanged(TabContentView* tabContentView, const QString& path)
{
	auto index = _tabWidget->indexOf(tabContentView);

	QFileInfo fileInfo(path);
	QString tabName = fileInfo.fileName();
	if (tabName.isEmpty())
	{
		tabName = path;
	}
	_tabWidget->setTabText(index, tabName);
	setCurrentTabPathText(path);
}

bool MultiTabPane::eventFilter(QObject* obj, QEvent* event)
{
	//qDebug() << "MultiTabPane::eventFilter:" << event->type();

	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* e = static_cast<QKeyEvent*>(event);
		//qDebug("Ate key press %d", e->key());
		switch (e->key())
		{
		case Qt::Key_J:
			if (e->modifiers() & Qt::ControlModifier)
			{
				//Ctrl + J でタブ移動（右）
				if (_tabWidget->currentIndex() >= 0 && _tabWidget->count() > 1)
				{
					auto newCurrentIndex = _tabWidget->currentIndex() + 1;
					if (newCurrentIndex >= _tabWidget->count())
					{
						newCurrentIndex = 0;
					}
					_tabWidget->setCurrentIndex(newCurrentIndex);
					setCurrentTabPathText(getCurrentView()->getPath());
				}
				return true;
			}
			if (e->modifiers() & Qt::ShiftModifier)
			{
				//Shift + J でパス指定ジャンプ
				_pPathJump->startInput();
				return true;
			}
			break;
		case Qt::Key_K:
			if (e->modifiers() & Qt::ControlModifier)
			{
				//Ctrl + K でタブ移動（左）
				if (_tabWidget->currentIndex() >= 0 && _tabWidget->count() > 1)
				{
					auto newCurrentIndex = _tabWidget->currentIndex() - 1;
					if (newCurrentIndex < 0)
					{
						newCurrentIndex = _tabWidget->count() - 1;
					}
					_tabWidget->setCurrentIndex(newCurrentIndex);
					setCurrentTabPathText(getCurrentView()->getPath());
				}
				return true;
			}
			break;
		case Qt::Key_N:
			if (e->modifiers() & Qt::ControlModifier)
			{
				//Ctrl + N でタブ作成
				QString openPath = "C:/";
				if (getCurrentView() != nullptr)
				{
					openPath = getCurrentView()->getPath();
				}
				addTab(openPath);
				return true;
			}
			break;
		case Qt::Key_W:
			if (e->modifiers() & Qt::ControlModifier)
			{
				//Ctrl + W でタブ削除
				if (getCurrentView() != nullptr && _tabWidget->count() > 1)
				{
					_tabWidget->removeTab(_tabWidget->currentIndex());
					_tabWidget->currentWidget()->setFocus();
				}
				return true;
			}
			break;
		case Qt::Key_F:
			if (e->modifiers() == Qt::NoModifier)
			{
				//インクリメンタルサーチ開始
				_pIncrementalSearch->startInput();
				return true;
			}
			break;
		case Qt::Key_E:
			if (e->modifiers() == Qt::ShiftModifier)
			{
				//ファイルエディット入力開始
				TabContentView* pView = getCurrentView();
				if (pView)
				{
					_pFileEdit->startInput(pView->getPath());
				}
				return true;
			}
			break;
		case Qt::Key_M:
			if (e->modifiers() == Qt::ShiftModifier)
			{
				//ディレクトリ作成入力開始
				TabContentView* pView = getCurrentView();
				if (pView)
				{
					_pMakeDir->startInput(pView->getPath());
				}
				return true;
			}
			break;
		case Qt::Key_C:
		{
			//Copy to the path user will determine
			if (e->modifiers() & Qt::ShiftModifier)
			{
				TabContentView* pView = getCurrentView();
				if (pView)
				{
					const auto& selectedItemPaths = pView->getSelectedPaths();
					if (selectedItemPaths.count() > 0)
					{
						_pCopyItem->startInput(pView->getPath(), selectedItemPaths);
					}
				}
				return true;
			}
			break;
		}
		default:
			break;
		}
	}

	return false;
}

void MultiTabPane::setCurrentTabPathText(const QString& path)
{
	ui.lineEdit_top->setText(path);
}
