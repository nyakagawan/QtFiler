#pragma once

#include "ListEditBaseModule.h"

class FileEditModule : public LineEditBaseModule
{
	Q_OBJECT
public:
	using LineEditBaseModule::LineEditBaseModule;

	virtual bool eventFilter(QObject* obj, QEvent* event) override;

protected Q_SLOTS:
	virtual void lineEditEditingFinished() override;
	virtual void lineEditReturnPressed() override;
};

class MakeDirLineEditModule : public LineEditBaseModule
{
	Q_OBJECT
public:
	using LineEditBaseModule::LineEditBaseModule;

	virtual bool eventFilter(QObject* obj, QEvent* event) override;

protected Q_SLOTS:
	virtual void lineEditEditingFinished() override;
	virtual void lineEditReturnPressed() override;
};
