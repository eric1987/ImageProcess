#include "CamSort.h"

CamSort::CamSort(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

CamSort::~CamSort()
{
}

void CamSort::connects()
{
	connect(ui.selectPosFile, &QPushButton::clicked, this, &CamSort::selectPosFiles);

}

QStringList CamSort::selectPosFiles()
{
	return QStringList();
}
