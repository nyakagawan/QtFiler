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

	QColor getColorSetting(const QString& colorSettingType);

private:
	void initialize();

private:
	static Settings* s_instance;
	QSize _windowSize;

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

