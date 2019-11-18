#pragma once

#include <QWidget>

class IncrementalSearchModule : public QObject
{
	Q_OBJECT
public:
	IncrementalSearchModule(class MultiTabPane* pMultiTabPane, class QLineEdit* pLineEdit);

	bool eventFilter(QObject* obj, QEvent* event);

	void startIncrementalSearch();

	void finishIncrementalSearch();

private Q_SLOTS:
	void lineEditBottomTextChanged(const QString& text);

	void lineEditBottomEditingFinished();

	void lineEditBottomReturnPressed();

private:
	class MultiTabPane* _pMultiTabPane = {};
	class QLineEdit* _pLineEdit = {};

	QMetaObject::Connection _connLineEditBottomTextChanged = {};
	QMetaObject::Connection _connLineEditBottomEditingFinished = {};
	QMetaObject::Connection _connLineEditBottomReturnPressed = {};

	QList<QModelIndex> _matchingIndexList = {};
};
