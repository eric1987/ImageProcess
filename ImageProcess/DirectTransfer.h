#pragma once

#include <QWidget>
#include "ui_DirectTransfer.h"

class DirectTransfer : public QWidget
{
	Q_OBJECT

public:
	DirectTransfer(QWidget *parent = Q_NULLPTR);
	~DirectTransfer();




private:
	Ui::DirectTransfer ui;
};
