#include <QFileDialog>

#include "CamSort.h"

CamSort::CamSort(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
	connects();
}

CamSort::~CamSort()
{
}

void CamSort::addCamSD(QString id, QString name, float available, float collective)
{
	if (true)
	{

	}
}

void CamSort::init()
{
	ui.selectPosType->addItem(QStringLiteral("大疆"));
	ui.selectPosType->addItem(QStringLiteral("纵横"));

	ui.startSave->setCheckable(true);

}

void CamSort::connects()
{
	connect(ui.selectPosFile, &QPushButton::clicked, this, &CamSort::selectPosFiles);
	connect(ui.selectPosType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &CamSort::changePosType);
	connect(ui.refreshSD, &QPushButton::clicked, this, &CamSort::refreshSDInfo);
	connect(ui.startSort, &QPushButton::clicked, this, &CamSort::sortImage);

	connect(ui.startSave, &QToolButton::clicked, this, &CamSort::sortTransfer);
	connect(ui.selectSavePath, &QPushButton::clicked, this, &CamSort::setSavePath);
	connect(ui.lineEdit, &QLineEdit::textChanged, this, &CamSort::changeSavePath);
}

void CamSort::selectPosFiles()
{
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

void CamSort::changePosType(int index)
{
	m_posType = index;
}

void CamSort::refreshSDInfo()
{
	Log::INFO(QStringLiteral("开始检查所有驱动盘。"));
	m_sdChanged = false;
	for (QMap<QString, UDisk *>::iterator it = m_disks.begin(); it != m_disks.end(); it++)
	{
		(it.value())->connected = false;
	}

	QFileInfoList list = QDir::drives();
	for each (QFileInfo disk in list)
	{
		UINT driver_type = GetDriveType((WCHAR *)disk.filePath().utf16());
		if (driver_type == 2)
		{
			getSDInfo(disk.path());
		}
	}

	//sd changed, then refresh sd info
	if (m_sdChanged)
	{
		Log::INFO(QStringLiteral("SD卡状态发生变化。"));
		//emit signalUDisk(m_disks);
	}
}

void CamSort::getSDInfo(QString path)
{
	QDir dir(path);
	QStringList dirlist = dir.entryList(QDir::Dirs);
	QStorageInfo info(dir);

	//判断是否为影像SD卡
	if (dirlist.contains("DCIM"))
	{
		if (m_disks.contains(info.device()))
		{
			m_disks.value(info.device())->connected = true;
// 			if (!compareSD(sd, info))
// 			{
// 				m_sdChanged = true;
// 			}
// 			else
// 			{
// 				;
// 			}
		}
		else
		{
			//getNewSDInfo(sd, info);
			m_sdChanged = true;
		}
	}
	else
	{
		return;
	}
}

void CamSort::sortImage()
{

}

void CamSort::sortTransfer()
{
	if (ui.startSave->isChecked())	//传输
	{
		ui.startSave->setText(QStringLiteral("停止传输"));
		ui.startSave->setChecked(true);
		unableOperation();
	}
	else       //停止传输
	{
		ui.startSave->setText(QStringLiteral("开始传输"));
		ui.startSave->setChecked(false);
		enableOperation();
	}
}

void CamSort::setSavePath()
{
	m_savePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	ui.lineEdit->setText(m_savePath);
}

void CamSort::changeSavePath(QString path)
{
	m_savePath = path;
}

void CamSort::unableOperation()
{
	ui.selectPosFile->setDisabled(true);
	ui.selectPosType->setDisabled(true);
	ui.refreshSD->setDisabled(true);
	ui.startSort->setDisabled(true);
	ui.selectSavePath->setDisabled(true);
	ui.lineEdit->setDisabled(true);
}

void CamSort::enableOperation()
{
	ui.selectPosFile->setEnabled(true);
	ui.selectPosType->setEnabled(true);
	ui.refreshSD->setEnabled(true);
	ui.startSort->setEnabled(true);
	ui.selectSavePath->setEnabled(true);
	ui.lineEdit->setEnabled(true);
}
