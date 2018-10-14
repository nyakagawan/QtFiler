#include "stdafx.h"
#include "Settings.h"

namespace
{
	const char* KeyWindowSize = "windowSize";
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
}

void Settings::initialize()
{
	_windowSize = value(KeyWindowSize, _windowSize).toSize();
	qDebug() << KeyWindowSize << ": " << _windowSize;
}

QColor Settings::getColorSetting(const QString& colorSettingType)
{
	QColor ret;

	QMap<QString, QColor>::const_iterator itr = _colorSettings.find(colorSettingType);
	if (itr != _colorSettings.end())
	{
		ret = *itr;
	}

	return ret;
}
