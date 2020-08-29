#include "LocalSort.h"

LocalSort::LocalSort(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
	connects();
}

LocalSort::~LocalSort()
{
}

void LocalSort::init()
{
	ui.selectPosType->addItem(QStringLiteral("大疆"));
	ui.selectPosType->addItem(QStringLiteral("纵横"));
}

void LocalSort::connects()
{
	connect(ui.selectPosFile, &QPushButton::clicked, this, &LocalSort::slotOpenPos);
	connect(ui.selectPosType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &LocalSort::slotChangePosType);
}

void LocalSort::slotOpenPos()
{
	ui.posWidget->clear();
	QFileDialog *fileDialog = new QFileDialog;
	fileDialog->setWindowTitle(QStringLiteral("选择pos文件"));
	fileDialog->setNameFilter(tr("Images(*.txt)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	fileDialog->setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
	QStringList fileNames;
	if (fileDialog->exec())
	{
		fileNames = fileDialog->selectedFiles();
	}
	m_posFiles = fileNames;
	ui.posWidget->addItems(fileNames);
}

void LocalSort::slotChangePosType(int index)
{
	m_posType = index;
}
