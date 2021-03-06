#pragma once
#include <QSortFilterProxyModel>
#include <QDir>
#include <QFont>
#include <QBrush>
#include <QItemSelectionModel>
#include "Types.h"

class QFileSystemModel;
class QFileIconProvider;

class FolderModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	explicit FolderModel(QObject *parent = Q_NULLPTR);
	~FolderModel();

	void setFont(const QFont& font);
	void initBrushes(const QMap<ColorRoleType, QColor>& colors);

	using QSortFilterProxyModel::index;
	QModelIndex index(const QString &path, int column = 0) const;

	void setSortSectionType(SectionType sectionType);
	SectionType sortSectionType() const;

	void setSortColumn(int column);
	int sortColumn() const;

	void setSortDirsType(SortDirsType dirsType);
	SortDirsType sortDirsType() const;

	void setSortDotFirst(bool dotFirst);
	bool sortDotFirst() const;

	void setSortOrder(Qt::SortOrder order);
	Qt::SortOrder sortOrder() const;

	int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex &modelIndex, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) Q_DECL_OVERRIDE;

	void refresh();

	QItemSelectionModel* getSelectionModel();
	void setSelect(int row, QItemSelectionModel::SelectionFlags selectionFlags, const QModelIndex &parentIndex);
	QModelIndexList getSelectedIndexList();
	void clearSelected();

#ifdef Q_OS_WIN
	bool isDrive(const QModelIndex& index) const;
#endif

	// QFileSystemModel specific API
	QModelIndex setRootPath(const QString &path);
	QString rootPath() const;
	QDir rootDirectory() const;

	void setIconProvider(QFileIconProvider *provider);
	QFileIconProvider *iconProvider() const;

	void setFilter(QDir::Filters filters);
	QDir::Filters filter() const;

	void setResolveSymlinks(bool enable);
	bool resolveSymlinks() const;

	void setReadOnly(bool enable);
	bool isReadOnly() const;

	void setNameFilterDisables(bool enable);
	bool nameFilterDisables() const;

	void setNameFilters(const QStringList &filters);
	QStringList nameFilters() const;

	QString filePath(const QModelIndex &index) const;
	bool isDir(const QModelIndex &index) const;
	qint64 size(const QModelIndex &index) const;
	QString type(const QModelIndex &index) const;
	QDateTime lastModified(const QModelIndex &index) const;

	QModelIndex mkdir(const QModelIndex &parent, const QString &name);
	bool rmdir(const QModelIndex &index);
	QString fileName(const QModelIndex &index) const;
	QIcon fileIcon(const QModelIndex &index) const;
	QFile::Permissions permissions(const QModelIndex &index) const;
	QFileInfo fileInfo(const QModelIndex &index) const;
	bool remove(const QModelIndex &index);
	bool removeItems(const QModelIndexList& indices);

Q_SIGNALS:
	void rootPathChanged(const QString &newPath);
	void fileRenamed(const QString &path, const QString &oldName, const QString &newName);
	void directoryLoaded(const QString &path);

private Q_SLOTS:
	void onRootPathChanged(const QString &newPath);
	void onFileRenamed(const QString &path, const QString &oldName, const QString &newName);
	void onDirectoryLoaded(const QString &path);

private:
	bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const Q_DECL_OVERRIDE;

	bool isSelected(const QModelIndex& index) const;

	SectionType getSectionTypeFromColumn(int column) const;
	int getColumnFromSectionType(SectionType sectionType) const;

	QBrush getTextBrush(const QModelIndex& index) const;
	QBrush getBackgroundBrush(const QModelIndex& index) const;
	QBrush getBrush(ColorRoleType colorRole) const;

	void emitRootPathChanged(const QString &newPath);
	void emitFileRenamed(const QString &path, const QString &oldName, const QString &newName);
	void emitDirectoryLoaded(const QString &path);

	int _sortColumn;
	SortDirsType _sortDirsType;
	bool _sortDotFirst;
	Qt::SortOrder _sortOrder;

	QFont _font;
	QMap<ColorRoleType, QBrush> _brushes;

	QItemSelectionModel* _selectionModel;

	class MyFileSystemModel* _pFsModel{};
};

