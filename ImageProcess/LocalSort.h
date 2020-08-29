#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include "ui_LocalSort.h"

class LocalSort : public QWidget
{
	Q_OBJECT
		enum POSTYPE { DAJ = 0, ZONGHENG };
public:
	LocalSort(QWidget *parent = Q_NULLPTR);
	~LocalSort();

private:
	void init();
	void connects();
	void slotOpenPos();
	void slotChangePosType(int index);

private:
	Ui::LocalSort ui;
	QStringList m_posFiles;
	int m_posType = DAJ;

};
