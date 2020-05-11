#pragma once

#include "ListEditBaseModule.h"

class FileEditModule : public LineEditBaseModule
{
	Q_OBJECT
public:
	using LineEditBaseModule::LineEditBaseModule;

	virtual bool eventFilter(QObject* obj, QEvent* event) override;

protected Q_SLOTS:
	virtual void lineEditReturnPressed() override;
};

class MakeDirLineEditModule : public LineEditBaseModule
{
	Q_OBJECT
public:
	using LineEditBaseModule::LineEditBaseModule;

	virtual bool eventFilter(QObject* obj, QEvent* event) override;

protected Q_SLOTS:
	virtual void lineEditReturnPressed() override;
};

class CopyItemLineEditModule : public LineEditBaseModule
{
	Q_OBJECT
public:
	using LineEditBaseModule::LineEditBaseModule;

	void startInput(const QString& currentDir, const QList<QString>& copyItemPaths)
	{
		LineEditBaseModule::startInput(currentDir);
		_copyItemPaths = copyItemPaths;
	}

	virtual bool eventFilter(QObject* obj, QEvent* event) override;

protected Q_SLOTS:
	virtual void lineEditReturnPressed() override;

private:
	QList<QString> _copyItemPaths = {};
};
