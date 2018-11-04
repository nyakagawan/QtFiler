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

	QSize getWindowSize() const { return _windowSize; }
	void setWindowSize(const QSize& size) { _windowSize = size; }

	QColor getColorSetting(const QString& colorSettingType) const;

	int getLeftTabCurrentIndex() const { return _leftTabCurrentIndex; }
	void setLeftTabCurrnetIndex(int i) { _leftTabCurrentIndex = i; }

	int getRightTabCurrentIndex() const { return _rightTabCurrentIndex; }
	void setRightTabCurrnetIndex(int i) { _rightTabCurrentIndex = i; }

	QStringList getLeftTabs() const { return _leftTabs; }
	void setLeftTabs(const QStringList& list) { _leftTabs = list; }

	QStringList getRightTabs() const { return _rightTabs; }
	void setRightTabs(const QStringList& list) { _rightTabs = list; }

private:
	void initialize();

private:
	static Settings* s_instance;
	QSize _windowSize;
	int _leftTabCurrentIndex;
	int _rightTabCurrentIndex;
	QStringList _leftTabs;
	QStringList _rightTabs;

	const QMap<QString, QColor> _colorSettings =
	{
		{ "folderView_normal",              "#000000", },
		{ "folderView_normal_selected",     "#000000", },
		{ "folderView_folder",              "#0000ff", },
		{ "folderView_folder_selected",     "#0000ff", },
		{ "folderView_readOnly",            "#00ff00", },
		{ "folderView_readOnly_selected",   "#00ff00", },
		{ "folderView_hidden",              "#888888", },
		{ "folderView_hidden_selected",     "#888888", },
		{ "folderView_system",              "#ff0000", },
		{ "folderView_system_selected",     "#ff0000", },

		{ "folderView_background",          "#ffffff", },
		{ "folderView_selected_background", "#b2d7ff", },

		{ "folderView_cursor",              "#0000ff", },
		{ "folderView_cursor_inactive",     "#cccccc", },

		{ "folderPath_text",                "#000000", },
		{ "folderPath_background",          "#ffffff", },
	};
};

