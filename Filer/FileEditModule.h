#pragma once

#include <QWidget>

class LineEditBaseModule : public QObject
{
	Q_OBJECT
public:
	LineEditBaseModule(class MultiTabPane* pMultiTabPane, class QLineEdit* pLineEdit);

	virtual bool eventFilter(QObject* obj, QEvent* event) = 0;
	virtual void startInput(const QString& currentDir);
	virtual void finishInput();

protected Q_SLOTS:
	virtual void lineEditTextChanged(const QString& text);
	virtual void lineEditEditingFinished();
	virtual void lineEditReturnPressed();

protected:
	class MultiTabPane* _pMultiTabPane = {};
	class QLineEdit* _pLineEdit = {};
	QString _currentDir = {};

	QMetaObject::Connection _connLineEditTextChanged = {};
	QMetaObject::Connection _connLineEditEditingFinished = {};
	QMetaObject::Connection _connLineEditReturnPressed = {};
};

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
