#include "stdafx.h"
#include "Settings.h"

namespace
{
	const char* KeyWindowSize = "windowSize";
	const char* KeyBottomPaneSize = "bottomPaneSize";
	const char* KeyLeftTabCurrentIndex = "leftTabCurrentIndex";
	const char* KeyRightTabCurrentIndex = "rightTabCurrentIndex";
	const char* KeyLeftTabs = "leftTabs";
	const char* KeyRightTabs = "rightTabs";
}

Settings* Settings::s_instance = nullptr;

void Settings::create()
{
	s_instance = new Settings();

	s_instance->initialize();
}

Settings* Settings::getInstance()
{
	return s_instance;
}

Settings::Settings()
	: QSettings(QSettings::IniFormat, QSettings::UserScope,
		QCoreApplication::organizationName(),
		QCoreApplication::applicationName())
{
	this->setIniCodec("UTF-8");
}

Settings::~Settings()
{
}

void Settings::flush()
{
	setValue(KeyWindowSize, _windowSize);
	setValue(KeyBottomPaneSize, _bottomPaneSize);
	setValue(KeyLeftTabCurrentIndex, _leftTabCurrentIndex);
	setValue(KeyRightTabCurrentIndex, _rightTabCurrentIndex);
	setValue(KeyLeftTabs, _leftTabs);
	setValue(KeyRightTabs, _rightTabs);
}

void Settings::initialize()
{
	_windowSize = value(KeyWindowSize, _windowSize).toSize();
	_bottomPaneSize = value(KeyBottomPaneSize, _bottomPaneSize).toSize();
	_leftTabCurrentIndex = value(KeyLeftTabCurrentIndex, _leftTabCurrentIndex).toInt();
	_rightTabCurrentIndex = value(KeyRightTabCurrentIndex, _rightTabCurrentIndex).toInt();
	_leftTabs = value(KeyLeftTabs, _leftTabs).toStringList();
	_rightTabs = value(KeyRightTabs, _rightTabs).toStringList();
}

QColor Settings::getColorSetting(const QString& colorSettingType) const
{
	QColor ret;

	QMap<QString, QColor>::const_iterator itr = _colorSettings.find(colorSettingType);
	if (itr != _colorSettings.end())
	{
		ret = *itr;
	}

	return ret;
}
