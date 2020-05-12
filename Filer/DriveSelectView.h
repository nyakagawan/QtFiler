#pragma once

#include <QTableView>

class DriveListModel : public QAbstractTableModel
{
	Q_OBJECT

private:
	enum SectionType
	{
		Name = 0,
		FreeSize,
		TotalSize,

		Unknown,
		MaxNum = Unknown
	};

	struct ListItem
	{
		virtual QString displayText() = 0;
		virtual QString rootPath() = 0;
		virtual qint64 bytesTotal() = 0;
		virtual qint64 bytesFree() = 0;
	};

	struct ListItemDrive : public ListItem
	{
		ListItemDrive(QStorageInfo info)
			:_storageInfo(info)
		{
		}
		virtual QString displayText() override final { return _storageInfo.rootPath(); }
		virtual QString rootPath() override final { return _storageInfo.rootPath(); }
		virtual qint64 bytesTotal() override final { return _storageInfo.bytesTotal(); }
		virtual qint64 bytesFree() override final { return _storageInfo.bytesFree(); }
	private:
		QStorageInfo _storageInfo{};
	};

	struct ListItemDesktop : public ListItem
	{
		virtual QString displayText() override final { return "1: Dekstop"; }
		virtual QString rootPath() override final;
		virtual qint64 bytesTotal() override final { return -1; }
		virtual qint64 bytesFree() override final { return -1; }
	};

public:
	explicit DriveListModel(QObject* parent = Q_NULLPTR);
	~DriveListModel();

	int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex& modelIndex, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	int findContainRow(const QString& path, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
	int findHeadCharRow(const QChar headChar) const;
	QString getRootPath(int row) const;

private:
	SectionType getSectionTypeFromColumn(int column) const;

	static qint64 bytesToGigaBytes(qint64 bytes)
	{
		return bytes / 1024 / 1024;
	}

private:
	QList<QSharedPointer<ListItem>> _driveList = {};
};

class DriveSelectView : public QTableView
{
	Q_OBJECT

public:
	DriveSelectView(QWidget* parent, QString currentPath);
	~DriveSelectView();

	QString getRootPath();

signals:
	void cursorMovedByHeadChar();

private:
	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void setCursor(const QModelIndex& index);
	void listCursorUp();
	void listCursorDown();

private:
	class DriveListModel* _pDriveListModel = nullptr;
};

