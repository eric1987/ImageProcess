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
	//����
	void connects();

	//����ѡ��pos�ļ�
	QStringList selectPosFiles();

private:
	Ui::CamSort ui;
};
