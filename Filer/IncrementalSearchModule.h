#pragma once

#include "ListEditBaseModule.h"

class IncrementalSearchModule : public LineEditBaseModule
{
	Q_OBJECT
public:
	using LineEditBaseModule::LineEditBaseModule;

	void startInput()
	{
		LineEditBaseModule::startInput("");
	}

	virtual bool eventFilter(QObject* obj, QEvent* event) override;

protected Q_SLOTS:
	virtual void lineEditTextChanged(const QString& text) override;
	virtual void lineEditReturnPressed() override;

private:
	QList<QModelIndex> _matchingIndexList = {};
};
