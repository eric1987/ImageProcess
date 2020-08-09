#pragma once

#include <QWidget>
#include "ui_CamSort.h"

class CamSort : public QWidget
{
	Q_OBJECT

public:
	CamSort(QWidget *parent = Q_NULLPTR);
	~CamSort();

private:
	//连接
	void connects();

	//返回选择pos文件
	QStringList selectPosFiles();

private:
	Ui::CamSort ui;
};
