#include "stdafx.h"
#include "MultiTabPane.h"
#include "TabContentView.h"
#include "Settings.h"

//-----------------------------------------------------------------------------
// IncrementalSearchModule
//-----------------------------------------------------------------------------
IncrementalSearchModule::IncrementalSearchModule(MultiTabPane* pMultiTabPane, QLineEdit* pLineEdit)
	:_pMultiTabPane(pMultiTabPane)
	, _pLineEdit(pLineEdit)
{
	_pLineEdit->installEventFilter(this);
}

bool IncrementalSearchModule::eventFilter(QObject* obj, QEvent* event)
{
	if (!_pLineEdit)
		return  false;

	//qDebug() << "IncrementalSearchModule::eventFilter";
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* e = static_cast<QKeyEvent*>(event);
		//qDebug("Ate key press %d", e->key());
		switch (e->key())
		{
		case Qt::Key_Escape:
			//編集終了
			finishIncrementalSearch();
			return true;

		case Qt::Key_Up:
		case Qt::Key_Down:
		{
			const QString& searchText = _pLineEdit->text();
			if (!searchText.isEmpty())
			{
				int searchDir = e->key() == Qt::Key_Up ? -1 : 1;
				_pMultiTabPane->getCurrentView()->incrementalSearch(searchText, searchDir, searchDir);
			}
			return true;
		}

		default:
			break;
		}
	}

	return false;
}

void IncrementalSearchModule::startIncrementalSearch()
{
	_pLineEdit->setText("");
	_pLineEdit->setReadOnly(false);
	_pLineEdit->setFocus();

	_connLineEditBottomTextChanged = connect(
		_pLineEdit,
		SIGNAL(textChanged(QString)),
		this,
		SLOT(lineEditBottomTextChanged(QString)));

	_connLineEditBottomEditingFinished = connect(
		_pLineEdit,
		SIGNAL(editingFinished()),
		this,
		SLOT(lineEditBottomEditingFinished()));

	_connLineEditBottomReturnPressed = connect(
		_pLineEdit,
		SIGNAL(returnPressed()),
		this,
		SLOT(lineEditBottomReturnPressed()));
}

void IncrementalSearchModule::finishIncrementalSearch()
{
	disconnect(_connLineEditBottomTextChanged);
	disconnect(_connLineEditBottomEditingFinished);
	disconnect(_connLineEditBottomReturnPressed);

	QString text = _pLineEdit->text();

	_pLineEdit->setText("");
	_pLineEdit->setReadOnly(true);
	_pMultiTabPane->getCurrentView()->setFocus();
}

void IncrementalSearchModule::lineEditBottomTextChanged(const QString& text)
{
	qDebug() << "lineEditBottomTextChanged: " << text;
	_pMultiTabPane->getCurrentView()->incrementalSearch(text, 0, 1);
}

void IncrementalSearchModule::lineEditBottomEditingFinished()
{
	qDebug() << "lineEditBottomEditingFinished";
	finishIncrementalSearch();
}

void IncrementalSearchModule::lineEditBottomReturnPressed()
{
	qDebug() << "lineEditBottomReturnPressed";
	finishIncrementalSearch();
}

//-----------------------------------------------------------------------------
// MultiTabPane
//-----------------------------------------------------------------------------
MultiTabPane::MultiTabPane(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	_tabWidget = new QTabWidget(this);
	ui.verticalLayout_center->addWidget(_tabWidget);

	ui.lineEdit_top->setReadOnly(true);
	ui.lineEdit_bottom->setReadOnly(true);

	_pIncrementalSearch = new IncrementalSearchModule(this, ui.lineEdit_bottom);
}

MultiTabPane::~MultiTabPane()
{
	delete _pIncrementalSearch;
}

TabContentView * MultiTabPane::getCurrentView()
{
	return qobject_cast<TabContentView*>(_tabWidget->currentWidget());
}

void MultiTabPane::getTabs(QStringList & list) const
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

bool MultiTabPane::eventFilter(QObject *obj, QEvent *event)
{
	//qDebug() << "MultiTabPane::eventFilter:" << event->type();

	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *e = static_cast<QKeyEvent *>(event);
		//qDebug("Ate key press %d", e->key());
		switch (e->key())
		{
		case Qt::Key_J:
			if (e->modifiers() & Qt::ShiftModifier)
			{
				//Shift + J でタブ移動（左）
				if (_tabWidget->currentIndex() >= 0 && _tabWidget->count() > 1)
				{
					auto newCurrentIndex = _tabWidget->currentIndex() - 1;
					if (newCurrentIndex < 0)
					{
						newCurrentIndex = _tabWidget->count() - 1;
					}
					_tabWidget->setCurrentIndex(newCurrentIndex);
				}
				return true;
			}
			break;
		case Qt::Key_K:
			if (e->modifiers() & Qt::ShiftModifier)
			{
				//Shift + K でタブ移動（右）
				if (_tabWidget->currentIndex() >= 0 && _tabWidget->count() > 1)
				{
					auto newCurrentIndex = _tabWidget->currentIndex() + 1;
					if (newCurrentIndex >= _tabWidget->count())
					{
						newCurrentIndex = 0;
					}
					_tabWidget->setCurrentIndex(newCurrentIndex);
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
				_pIncrementalSearch->startIncrementalSearch();
				return true;
			}
			break;
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
