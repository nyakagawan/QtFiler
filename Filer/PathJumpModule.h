#pragma once

#include <QWidget>

class PathJumpModule : public QObject
{
	Q_OBJECT
public:
	PathJumpModule(class MultiTabPane* pMultiTabPane, class QLineEdit* pLineEdit);

	bool eventFilter(QObject* obj, QEvent* event);

	void startInput();

	void finishInput();

private Q_SLOTS:
	void lineEditTextChanged(const QString& text);

	void lineEditEditingFinished();

	void lineEditReturnPressed();

private:
	class MultiTabPane* _pMultiTabPane = {};
	class QLineEdit* _pLineEdit = {};

	QMetaObject::Connection _connLineEditTextChanged = {};
	QMetaObject::Connection _connLineEditEditingFinished = {};
	QMetaObject::Connection _connLineEditReturnPressed = {};

	static QList<QString> _history;
};
