#pragma once

#include <QWidget>
#include "ui_MultiTabPane.h"

class MultiTabPane : public QWidget
{
	Q_OBJECT

public:
	MultiTabPane(QWidget *parent = Q_NULLPTR);
	~MultiTabPane();

private:
	Ui::MultiTabPane ui;
};
