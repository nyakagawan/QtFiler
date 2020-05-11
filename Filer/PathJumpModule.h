#pragma once

#include "ListEditBaseModule.h"

class PathJumpModule : public LineEditBaseModule
{
	Q_OBJECT
public:
	using LineEditBaseModule::LineEditBaseModule;

	void startInput()
	{
		LineEditBaseModule::startInput("");
	}

	bool eventFilter(QObject* obj, QEvent* event);

private Q_SLOTS:
	virtual void lineEditReturnPressed() override;

private:
	static QList<QString> _history;
};
