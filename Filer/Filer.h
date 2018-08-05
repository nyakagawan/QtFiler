#pragma once

#include <QtWidgets/QWidget>
#include "ui_Filer.h"

class Filer : public QWidget
{
	Q_OBJECT

public:
	Filer(QWidget *parent = Q_NULLPTR);

private:
	Ui::FilerClass ui;
};
