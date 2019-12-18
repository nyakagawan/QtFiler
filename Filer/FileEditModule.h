#pragma once

#include <QWidget>

class FileEditModule : public QObject
{
	Q_OBJECT
public:
	FileEditModule(class MultiTabPane* pMultiTabPane, class QLineEdit* pLineEdit);

	bool eventFilter(QObject* obj, QEvent* event);
	void startInput(const QString& currentDir);
	void finishInput();

private Q_SLOTS:
	void lineEditTextChanged(const QString& text);
	void lineEditEditingFinished();
	void lineEditReturnPressed();

private:
	class MultiTabPane* _pMultiTabPane = {};
	class QLineEdit* _pLineEdit = {};
	QString _currentDir = {};

	QMetaObject::Connection _connLineEditTextChanged = {};
	QMetaObject::Connection _connLineEditEditingFinished = {};
	QMetaObject::Connection _connLineEditReturnPressed = {};
};
