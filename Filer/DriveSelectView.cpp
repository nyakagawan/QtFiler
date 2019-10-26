#include "stdafx.h"
#include "DriveSelectView.h"
#include "FolderViewStyledItemDelegate.h"

//=============================================================================
//=============================================================================
DriveListModel::DriveListModel(QObject *parent/* = Q_NULLPTR*/)
	:QAbstractTableModel(parent)
{
	_driveList = QStorageInfo::mountedVolumes();
#if 0
	for (QStorageInfo i : _driveList)
	{
		qDebug() << "---------------";
		qDebug() << "rootPath: " << i.rootPath();
		qDebug() << "name: " << i.name();
		qDebug() << "displayName: " << i.displayName();
		qDebug() << "isRoot: " << i.isRoot();
		qDebug() << "isValid: " << i.isValid();
		qDebug() << "device: " << i.device();
		qDebug() << "subvolume: " << i.subvolume();
		qDebug() << "fileSystemType: " << i.fileSystemType();
		qDebug() << "bytesTotal: " << QString("%L1").arg(i.bytesTotal() / 1024 / 1024 / 1024) << "GB";
		qDebug() << "bytesFree: " << QString("%L1").arg(i.bytesFree() / 1024 / 1024 / 1024) << "GB";
		qDebug() << "bytesAvailable: " << QString("%L1").arg(i.bytesAvailable() / 1024 / 1024 / 1024) << "GB";
	}
#endif
}

DriveListModel::~DriveListModel()
{
}

int DriveListModel::rowCount(const QModelIndex& parent) const
{
	return _driveList.count();
}

int DriveListModel::columnCount(const QModelIndex& parent) const
{
	//���O
	//�󂫗e��
	//�ő�e��
	return SectionType::MaxNum;
}

QVariant DriveListModel::data(const QModelIndex &modelIndex, int role) const
{
	if (!modelIndex.isValid())
	{
		return QVariant();
	}

	SectionType sectionType = getSectionTypeFromColumn(modelIndex.column());
	if (sectionType == SectionType::Unknown)
	{
		qFatal("sectionType != SectionType::Unknown: %d", modelIndex.column());
	}

	const int iRow = modelIndex.row();

	switch (role)
	{
	case Qt::DisplayRole:
	{
		switch (sectionType)
		{
		case DriveListModel::Name:
			return _driveList[iRow].rootPath();
		case DriveListModel::FreeSize:
			return QString("%L1").arg(bytesToGigaBytes(_driveList[iRow].bytesFree()));
		case DriveListModel::TotalSize:
			return QString("%L1").arg(bytesToGigaBytes(_driveList[iRow].bytesTotal()));
		default:
			Q_ASSERT(false);
			break;
		}
		break;
	}
	case Qt::TextAlignmentRole:
	{
		switch (sectionType)
		{
		case DriveListModel::FreeSize:
		case DriveListModel::TotalSize:
			return QVariant(Qt::AlignRight | Qt::AlignVCenter);
		}
	}
	}

	return QVariant();
}

QVariant DriveListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	//qDebug("headerData: section:%d, orientation:%s, role:%d", section, QString(orientation), role);
	if (role != Qt::DisplayRole || orientation == Qt::Orientation::Vertical)
		return QAbstractTableModel::headerData(section, orientation, role);

	SectionType sectionType = getSectionTypeFromColumn(section);
	if (sectionType == SectionType::Unknown)
	{
		qFatal("sectionType != SectionType::Unknown: %d", section);
	}

	switch (sectionType)
	{
	case SectionType::Name:
		return tr("Drive Name");
	case SectionType::FreeSize:
		return tr("Free(GB)");
	case SectionType::TotalSize:
		return tr("Total(GB)");
	default:
		return QAbstractTableModel::headerData(section, orientation, role);
	}
}

int DriveListModel::findContainRow(const QString& path) const
{
	for (int i = 0; i < _driveList.count(); ++i)
	{
		if (path.startsWith(_driveList[i].rootPath()))
		{
			return i;
		}
	}
	return -1;
}

QString DriveListModel::getRootPath(int row) const
{
	if(row >= 0 && row < _driveList.count())
		return _driveList[row].rootPath();
	return QString();
}

DriveListModel::SectionType DriveListModel::getSectionTypeFromColumn(int column) const
{
	switch (column)
	{
	case 0:
		return SectionType::Name;
	case 1:
		return SectionType::FreeSize;
	case 2:
		return SectionType::TotalSize;
	default:
		return SectionType::Unknown;
	}
}

//=============================================================================
//=============================================================================
DriveSelectView::DriveSelectView(QWidget *parent, QString currentPath)
	:_pDriveListModel(new DriveListModel(this))
{
	setItemDelegate(new FolderViewStyledItemDelegate(this));

	setModel(_pDriveListModel);
	setSortingEnabled(false);
	verticalHeader()->hide();
	setGridStyle(Qt::SolidLine);

	horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

	setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

	int cursorRow = _pDriveListModel->findContainRow(currentPath);
	if (cursorRow >= 0)
	{
		setCursor(_pDriveListModel->index(cursorRow, 0));
	}
}


DriveSelectView::~DriveSelectView()
{
	delete _pDriveListModel;
}

QString DriveSelectView::getRootPath()
{
	return _pDriveListModel->getRootPath(currentIndex().row());
}

void DriveSelectView::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_K:
	case Qt::Key_Up:
		qDebug() << "ListSelectDialog::keyPressEvent up";
		listCursorUp();
		e->accept();
		return;
	case Qt::Key_J:
	case Qt::Key_Down:
		qDebug() << "ListSelectDialog::keyPressEvent down";
		listCursorDown();
		e->accept();
		return;
	default:
		qDebug() << "ListSelectDialog::keyPressEvent other";
		break;
	}
	e->ignore();

}

void DriveSelectView::setCursor(const QModelIndex& index)
{
	if (index.isValid())
	{
		setCurrentIndex(index);
		scrollTo(index);
	}
}

void DriveSelectView::listCursorUp()
{
	auto index = this->currentIndex();
	if (index.row() == 0)return;
	auto next = index.siblingAtRow(index.row() - 1);
	setCursor(next);
}

void DriveSelectView::listCursorDown()
{
	auto index = this->currentIndex();
	auto rowCount = this->model()->rowCount(index.parent());
	if (index.row() == rowCount - 1)return;
	auto next = index.siblingAtRow(index.row() + 1);
	setCursor(next);
}
