#pragma once
#include <QSettings>
#include <QPoint>
#include <QSize>

class Settings : QSettings
{
	Q_OBJECT

public:
	static void create();
	static Settings* getInstance();

	Settings();
	~Settings();

	void flush();

	QSize getWindowSize() const {return _windowSize;}
	void setWindowSize(const QSize& size) { _windowSize = size; }

private:
	void initialize();

private:
	static Settings* s_instance;
	QSize _windowSize;
};

