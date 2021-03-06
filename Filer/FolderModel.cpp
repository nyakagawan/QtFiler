#include "stdafx.h"
#include <QFileSystemModel>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QItemSelectionModel>
#include <QFileIconProvider>
#include "FolderModel.h"
#include "Types.h"
#include "PlatformCompat.h"

class MyFileSystemModel : public QFileSystemModel
{
	using QFileSystemModel::QFileSystemModel;

public:
#if 1
	int rowCount(const QModelIndex& parent = QModelIndex()) const override
	{
		int rowCount = rowCountReal(parent);
		if (rowCount > 0)
			return rowCount;
		return 1;
	}

	int rowCountReal(const QModelIndex& parent = QModelIndex()) const
	{
		return QFileSystemModel::rowCount(parent);
	}

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override
	{
		QModelIndex ret = QFileSystemModel::index(row, column, parent);
		if (ret == QModelIndex())
		{
			//qDebug() << "!!! empty";
		}
		return ret;
	}
#if 0
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
	{
		qDebug() << "row: " << index.row() << ", column: " << index.column();
		QVariant ret = QFileSystemModel::data(index, role);
		if (!ret.isValid())
		{
			qDebug() << "!!! invalid";
		}
		return ret;
	}
#endif
#endif
};

FolderModel::FolderModel(QObject* parent/* = Q_NULLPTR*/) :
	QSortFilterProxyModel(parent),
	_sortColumn(0),
	_sortDirsType(SortDirsType::NoSpecify),
	_sortDotFirst(true),
	_sortOrder(Qt::AscendingOrder),
	_selectionModel(new QItemSelectionModel(this))
{
	setSortCaseSensitivity(Qt::CaseInsensitive);

	_pFsModel = new MyFileSystemModel(this);
	QFileSystemModel* fsModel = _pFsModel;

	fsModel->setFilter(DEFAULT_FILTER_FLAGS);

	connect(fsModel,
		SIGNAL(rootPathChanged(QString)),
		this,
		SLOT(onRootPathChanged(QString)));
	connect(fsModel,
		SIGNAL(directoryLoaded(QString)),
		this,
		SLOT(onDirectoryLoaded(QString)));

	setSourceModel(fsModel);
}

FolderModel::~FolderModel()
{
	delete _selectionModel;
}

void FolderModel::onRootPathChanged(const QString& newPath)
{
	emitRootPathChanged(newPath);
}

void FolderModel::onFileRenamed(const QString& path, const QString& oldName, const QString& newName)
{
	emitFileRenamed(path, oldName, newName);
}

void FolderModel::onDirectoryLoaded(const QString& path)
{
	emitDirectoryLoaded(path);
}

QModelIndex FolderModel::index(const QString& path, int column/* = 0*/) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return mapFromSource(fsModel->index(path, column));
}

void FolderModel::setSortSectionType(SectionType sectionType)
{
	setSortColumn(getColumnFromSectionType(sectionType));
}

SectionType FolderModel::sortSectionType() const
{
	return getSectionTypeFromColumn(_sortColumn);
}

void FolderModel::setSortColumn(int column)
{
	_sortColumn = column;
}

int FolderModel::sortColumn() const
{
	return _sortColumn;
}

void FolderModel::setSortDirsType(SortDirsType dirsType)
{
	_sortDirsType = dirsType;
}

SortDirsType FolderModel::sortDirsType() const
{
	return _sortDirsType;
}

void FolderModel::setSortDotFirst(bool dotFirst)
{
	_sortDotFirst = dotFirst;
}

bool FolderModel::sortDotFirst() const
{
	return _sortDotFirst;
}

void FolderModel::setSortOrder(Qt::SortOrder order)
{
	_sortOrder = order;
}

Qt::SortOrder FolderModel::sortOrder() const
{
	return _sortOrder;
}

int FolderModel::columnCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	return 4;
}

QVariant FolderModel::data(const QModelIndex& modelIndex, int role) const
{
	QVariant ret = QVariant();

	if (!modelIndex.isValid())
	{
		return ret;
	}

	if (_pFsModel->rowCountReal(modelIndex.parent()) == 0)
	{
		if (modelIndex.column() == 0)
		{
			ret = QString("__no_item__");
			return ret;
		}
	}

	SectionType sectionType = getSectionTypeFromColumn(modelIndex.column());
	Q_ASSERT(sectionType != SectionType::Unknown);

	switch (role)
	{
	case Qt::DisplayRole:
	case Qt::EditRole:
	{
		QFileInfo fi = fileInfo(modelIndex);

		switch (sectionType)
		{
		case SectionType::FileName:
#ifdef Q_OS_WIN
			if (fi.fileName() != ".." && isDrive(modelIndex))
			{
				ret = fi.absolutePath();
			}
			else
#endif
				if (!fi.isDir() && !fi.completeBaseName().isEmpty())
				{
					ret = fi.completeBaseName();
				}
				else
				{
					ret = fi.fileName();
				}
			if (ret == QString())
			{
				//qDebug() << "empty";
				ret = QString("___NO_ITEM___");
			}
			break;
		case SectionType::FileType:
			if (!fi.isDir() && !fi.completeBaseName().isEmpty())
			{
				ret = fi.suffix();
			}
			break;
		case SectionType::FileSize:
#ifdef Q_OS_WIN
			if (isDrive(modelIndex))
			{
				ret = QString("<Drive>");
			}
			else
#endif
				if (fi.isDir())
				{
					ret = QString("<Folder>");
				}
				else
				{
					auto size = fi.size();
					if (size < 1024)
					{
						ret = QString("%1").arg(size);
					}
					else if (size < (1024 * 1024))
					{
						ret = QString("%1K").arg((double)size / 1024, 0, 'f', 1);
					}
					else if (size < (1024 * 1024 * 1024))
					{
						ret = QString("%1M").arg((double)size / (1024.0 * 1024.0), 0, 'f', 1);
					}
					else
					{
						ret = QString("%1G").arg((double)size / (1024.0 * 1024.0 * 1024.0), 0, 'f', 1);
					}
				}
			break;
		case SectionType::LastModified:
			ret = fi.lastModified().toString("yyyy-MM-dd HH:mm:ss");
			break;
		default:
			break;
		}

		break;
	}
	case Qt::FontRole:
		return _font;

	case Qt::TextAlignmentRole:
		if (sectionType == SectionType::FileSize || sectionType == SectionType::LastModified)
		{
			return QVariant(Qt::AlignRight | Qt::AlignVCenter);
		}
		else
		{
			return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
		}

		break;

	case Qt::TextColorRole:
		ret = getTextBrush(modelIndex);

		break;

	case Qt::BackgroundRole:
		ret = getBackgroundBrush(modelIndex);

		break;

	case QFileSystemModel::FileIconRole:
		if (sectionType == SectionType::FileName)
		{
			ret = fileIcon(modelIndex);
		}

		break;

	case QFileSystemModel::FilePathRole:
		if (sectionType == SectionType::FileName)
		{
			ret = filePath(modelIndex);
		}

		break;

	case QFileSystemModel::FileNameRole:
		if (sectionType == SectionType::FileName)
		{
			ret = fileName(modelIndex);
		}

		break;

	}

	//    qDebug() << "data(" << modelIndex << "," << static_cast<Qt::ItemDataRole>(role) << ") : ret = " << ret;

	return ret;
}

QVariant FolderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant ret;

	SectionType sectionType = getSectionTypeFromColumn(section);
	Q_ASSERT(sectionType != SectionType::Unknown);

	if (role == Qt::DisplayRole)
	{
		switch (sectionType)
		{
		case SectionType::FileName:
			ret = tr("Name");
			break;
		case SectionType::FileType:
			ret = tr("Type");
			break;
		case SectionType::FileSize:
			ret = tr("Size");
			break;
		case SectionType::LastModified:
			ret = tr("Last modified");
			break;
		default:
			break;
		}

	}

	if (ret.isNull())
	{
		ret = QSortFilterProxyModel::headerData(section, orientation, role);
	}

	//    qDebug() << "headerData(" << section << "," << orientation << "," << static_cast<Qt::ItemDataRole>(role) << ") : ret = " << ret;

	return ret;
}

void FolderModel::sort(int column, Qt::SortOrder order/* = Qt::AscendingOrder*/)
{
	_sortColumn = column;
	_sortOrder = order;

	refresh();
}

void FolderModel::refresh()
{
	bool backup = dynamicSortFilter();
	setDynamicSortFilter(false);
	QSortFilterProxyModel::sort(_sortColumn, _sortOrder);
	setDynamicSortFilter(backup);
}

QItemSelectionModel* FolderModel::getSelectionModel()
{
	return _selectionModel;
}

void FolderModel::setSelect(int row, QItemSelectionModel::SelectionFlags selectionFlags, const QModelIndex& parentIndex)
{
	if (_selectionModel != Q_NULLPTR)
	{
		QItemSelection selection(index(row, 0, parentIndex), index(row, columnCount() - 1, parentIndex));
		_selectionModel->select(selection, selectionFlags);
	}
}

QModelIndexList FolderModel::getSelectedIndexList()
{
	QModelIndexList indexList;

	if (_selectionModel != Q_NULLPTR)
	{
		indexList = _selectionModel->selectedRows();
	}

	return indexList;
}

void FolderModel::clearSelected()
{
	if (_selectionModel != Q_NULLPTR)
	{
		_selectionModel->clear();
	}
}

bool FolderModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	QFileInfo l_info = fsModel->fileInfo(source_left);
	QFileInfo r_info = fsModel->fileInfo(source_right);
	bool ascOrder = (_sortOrder == Qt::AscendingOrder);

	if (_sortDotFirst)
	{
		if (l_info.fileName() == ".")
		{
			return ascOrder;
		}
		else if (r_info.fileName() == ".")
		{
			return !ascOrder;
		}
		else if (l_info.fileName() == ".." && r_info.fileName() != ".")
		{
			return ascOrder;
		}
		else if (r_info.fileName() == ".." && l_info.fileName() != ".")
		{
			return !ascOrder;
		}
	}

	if (_sortDirsType == SortDirsType::First)
	{
		if (!l_info.isDir() && r_info.isDir())
		{
			return !ascOrder;
		}
		else if (l_info.isDir() && !r_info.isDir())
		{
			return ascOrder;
		}
	}
	else if (_sortDirsType == SortDirsType::Last)
	{
		if (!l_info.isDir() && r_info.isDir())
		{
			return ascOrder;
		}
		else if (l_info.isDir() && !r_info.isDir())
		{
			return !ascOrder;
		}
	}

	SectionType sortSectionType = getSectionTypeFromColumn(_sortColumn);

	if (sortSectionType == SectionType::FileSize)
	{
		if (!l_info.isDir() && !r_info.isDir())
		{
			return l_info.size() < r_info.size();
		}
	}
	else if (sortSectionType == SectionType::FileType)
	{
		QString l_type = (!l_info.isDir() && !l_info.completeBaseName().isEmpty()) ? l_info.suffix() : "";
		QString r_type = (!r_info.isDir() && !r_info.completeBaseName().isEmpty()) ? r_info.suffix() : "";

		if (l_type.isEmpty() && r_type.isEmpty())
		{
			l_type = l_info.fileName();
			r_type = r_info.fileName();
		}

		if (sortCaseSensitivity() == Qt::CaseInsensitive && l_type.toLower() != r_type.toLower())
		{
			return l_type.toLower() < r_type.toLower();
		}
		else
		{
			return l_type < r_type;
		}
	}
	else if (sortSectionType == SectionType::LastModified)
	{
		return l_info.lastModified() < r_info.lastModified();
	}
	else
	{
		QString l_name = (!l_info.isDir() && !l_info.completeBaseName().isEmpty()) ? l_info.completeBaseName() : l_info.fileName();
		QString r_name = (!r_info.isDir() && !r_info.completeBaseName().isEmpty()) ? r_info.completeBaseName() : r_info.fileName();

		if (sortCaseSensitivity() == Qt::CaseInsensitive && l_name.toLower() != r_name.toLower())
		{
			return l_name.toLower() < r_name.toLower();
		}
		else
		{
			return l_name < r_name;
		}
	}

	return false;
}

#ifdef Q_OS_WIN
bool FolderModel::isDrive(const QModelIndex& index) const
{
	QFileInfo fi = fileInfo(index);
	foreach(QFileInfo drive, QDir::drives())
	{
		if (drive.absoluteFilePath() == fi.absoluteFilePath())
		{
			return true;
		}
	}

	return false;
}
#endif

SectionType FolderModel::getSectionTypeFromColumn(int column) const
{
	// Todo: 将来的に可変にする
	if (column == 0)
	{
		return SectionType::FileName;
	}
	else if (column == 1)
	{
		return SectionType::FileType;
	}
	else if (column == 2)
	{
		return SectionType::FileSize;
	}
	else if (column == 3)
	{
		return SectionType::LastModified;
	}

	return SectionType::Unknown;
}

int FolderModel::getColumnFromSectionType(SectionType sectionType) const
{
	if (sectionType == SectionType::FileName)
	{
		return 0;
	}
	else if (sectionType == SectionType::FileType)
	{
		return 1;
	}
	else if (sectionType == SectionType::FileSize)
	{
		return 2;
	}
	else if (sectionType == SectionType::LastModified)
	{
		return 3;
	}

	return -1;
}

QBrush FolderModel::getTextBrush(const QModelIndex& index) const
{
	QBrush ret;

	// TODO: System file(for Win)

	if ((fileName(index) != "..") && (fileInfo(index).isHidden()))
	{
		if (isSelected(index))
		{
			ret = getBrush(ColorRoleType::Hidden_Selected);
		}
		else
		{
			ret = getBrush(ColorRoleType::Hidden);
		}
	}
	else if ((fileName(index) != "..") && (!fileInfo(index).isWritable()))
	{
		if (isSelected(index))
		{
			ret = getBrush(ColorRoleType::ReadOnly_Selected);
		}
		else
		{
			ret = getBrush(ColorRoleType::ReadOnly);
		}
	}
	else if (fileInfo(index).isDir())
	{
		if (isSelected(index))
		{
			ret = getBrush(ColorRoleType::Folder_Selected);
		}
		else
		{
			ret = getBrush(ColorRoleType::Folder);
		}
	}
	else
	{
		if (isSelected(index))
		{
			ret = getBrush(ColorRoleType::Normal_Selected);
		}
		else
		{
			ret = getBrush(ColorRoleType::Normal);
		}
	}

	return ret;
}

QBrush FolderModel::getBackgroundBrush(const QModelIndex& index) const
{
	QBrush ret;

	if (isSelected(index))
	{
		ret = getBrush(ColorRoleType::Selected_Background);
	}
	else
	{
		ret = getBrush(ColorRoleType::Background);
	}

	return ret;
}

QBrush FolderModel::getBrush(ColorRoleType colorRole) const
{
	QBrush ret;

	QMap<ColorRoleType, QBrush>::const_iterator itr = _brushes.find(colorRole);
	if (itr != _brushes.end())
	{
		ret = *itr;
	}

	return ret;
}

void FolderModel::setFont(const QFont& font)
{
	_font = font;
}

void FolderModel::initBrushes(const QMap<ColorRoleType, QColor>& colors)
{
	_brushes.clear();

	for (auto colorRole : colors.keys())
	{
		_brushes[colorRole] = QBrush(colors[colorRole]);
	}
}

bool FolderModel::isSelected(const QModelIndex& index) const
{
	if (_selectionModel != Q_NULLPTR)
	{
		return _selectionModel->isSelected(index);
	}

	return false;
}

void FolderModel::emitRootPathChanged(const QString& newPath)
{
	emit rootPathChanged(newPath);
}

void FolderModel::emitFileRenamed(const QString& path, const QString& oldName, const QString& newName)
{
	emit fileRenamed(path, oldName, newName);
}

void FolderModel::emitDirectoryLoaded(const QString& path)
{
	emit directoryLoaded(path);
}

// QFileSystemModel specific API
QModelIndex FolderModel::setRootPath(const QString& path)
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return mapFromSource(fsModel->setRootPath(path));
}

QString FolderModel::rootPath() const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->rootPath();
}

QDir FolderModel::rootDirectory() const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->rootDirectory();
}

void FolderModel::setIconProvider(QFileIconProvider* provider)
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	fsModel->setIconProvider(provider);
}

QFileIconProvider* FolderModel::iconProvider() const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->iconProvider();
}

void FolderModel::setFilter(QDir::Filters filters)
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	fsModel->setFilter(filters);
}

QDir::Filters FolderModel::filter() const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->filter();
}

void FolderModel::setResolveSymlinks(bool enable)
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	fsModel->setResolveSymlinks(enable);
}

bool FolderModel::resolveSymlinks() const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->resolveSymlinks();
}

void FolderModel::setReadOnly(bool enable)
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	fsModel->setReadOnly(enable);
}

bool FolderModel::isReadOnly() const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->isReadOnly();
}

void FolderModel::setNameFilterDisables(bool enable)
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	fsModel->setNameFilterDisables(enable);
}

bool FolderModel::nameFilterDisables() const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->nameFilterDisables();
}

void FolderModel::setNameFilters(const QStringList& filters)
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->setNameFilters(filters);
}

QStringList FolderModel::nameFilters() const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->nameFilters();
}

QString FolderModel::filePath(const QModelIndex& index) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->filePath(mapToSource(index));
}

bool FolderModel::isDir(const QModelIndex& index) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->isDir(mapToSource(index));
}

qint64 FolderModel::size(const QModelIndex& index) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->size(mapToSource(index));
}

QString FolderModel::type(const QModelIndex& index) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->type(mapToSource(index));
}

QDateTime FolderModel::lastModified(const QModelIndex& index) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->lastModified(mapToSource(index));
}

QModelIndex FolderModel::mkdir(const QModelIndex& parent, const QString& name)
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return mapFromSource(fsModel->mkdir(mapToSource(parent), name));
}

bool FolderModel::rmdir(const QModelIndex& index)
{
	return removeItems(QModelIndexList{ index });
}

QString FolderModel::fileName(const QModelIndex& index) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->fileName(mapToSource(index));
}

QIcon FolderModel::fileIcon(const QModelIndex& index) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->fileIcon(mapToSource(index));
}

QFile::Permissions FolderModel::permissions(const QModelIndex& index) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->permissions(mapToSource(index));
}

QFileInfo FolderModel::fileInfo(const QModelIndex& index) const
{
	QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

	return fsModel->fileInfo(mapToSource(index));
}

bool FolderModel::remove(const QModelIndex& index)
{
	return removeItems(QModelIndexList{ index });
}

bool FolderModel::removeItems(const QModelIndexList& indices)
{
#ifdef Q_OS_WIN
	QList<QString> pathArray{};
	for (const QModelIndex& index : indices)
	{
		pathArray.append(filePath(index));
	}

	auto hr = PlatformCompat::MoveToTrash(pathArray);
	if (FAILED(hr))
	{
		qDebug() << "move to trash failed: " << QString::number(hr).toInt(0, 8);
		return false;
	}
	return true;
#else
	for (const QModelIndex& index : indices)
	{
		return fsModel->rmdir(mapToSource(index));
}
#endif
}
