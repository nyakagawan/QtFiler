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

public:
	explicit DriveListModel(QObject *parent = Q_NULLPTR);
	~DriveListModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex &modelIndex, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	int findContainRow(const QString& path) const;
	QString getRootPath(int row) const;

private:
	SectionType getSectionTypeFromColumn(int column) const;

	static qint64 bytesToGigaBytes(qint64 bytes)
	{
		return bytes / 1024 / 1024;
	}

private:
	QList<QStorageInfo> _driveList = {};
};

class DriveSelectView : public QTableView
{
	Q_OBJECT

public:
	DriveSelectView(QWidget *parent, QString currentPath);
	~DriveSelectView();

	QString getRootPath();

private:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void setCursor(const QModelIndex& index);
	void listCursorUp();
	void listCursorDown();

private:
	class DriveListModel* _pDriveListModel = nullptr;
};

