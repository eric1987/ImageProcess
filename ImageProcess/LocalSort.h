#pragma once

#include <QWidget>
#include "ui_LocalSort.h"

class LocalSort : public QWidget
{
	Q_OBJECT

public:
	LocalSort(QWidget *parent = Q_NULLPTR);
	~LocalSort();

private:
	Ui::LocalSort ui;
};
