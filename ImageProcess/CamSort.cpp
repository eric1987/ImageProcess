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

void CamSort::test()
{
	QMap<int, int> sort;
	sort.insert(0, 80);
	sort.insert(1, 140);
	sort.insert(2, 77);
	sort.insert(3, 83);
	sort.insert(4, 92);
	sort.insert(5, 58);
	showSorties(sort);
}

void CamSort::init()
{
	ui.selectPosType->addItem(QStringLiteral("大疆"));
	ui.selectPosType->addItem(QStringLiteral("纵横"));

	ui.startSave->setCheckable(true);
	
	m_sortLayout = new QVBoxLayout;
	ui.sortWidget->setLayout(m_sortLayout);
	
	test();
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
	for (QMap<QString, SDInfo>::iterator it = m_sdInfo.begin(); it != m_sdInfo.end(); it++)
	{
		it.value().connected = false;
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

	for (QMap<QString, SDInfo>::const_iterator iter = m_sdInfo.begin(); iter != m_sdInfo.end(); iter++)
	{
		if (m_sdList.contains(iter.key()))
		{
			//已存在
			m_sdList[iter.key()]->setOnlineStatus(iter->connected);
		}
		else
		{
			//未有
			CamSD *sd = new CamSD;
			sd->setName(iter->nickname, iter->path);
			sd->setAvailableAndCollective(iter->available, iter->collective);
			m_sdList[iter.key()] = sd;
			ui.camLayout->addWidget(sd);
		}
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
		SDInfo sd;
		sd.deviceID = info.device();
		sd.available = info.bytesAvailable() / 1024 / 1024 / 1024.0;
		sd.collective = info.bytesTotal() / 1024 / 1024 / 1024.0;
		sd.path = path;
		sd.connected = true;
		
		getSDNicknameAndImageSize(sd, path);
		m_sdInfo[info.device()] = sd;
	}
	else
	{
		return;
	}
}

void CamSort::getSDNicknameAndImageSize(SDInfo &sd, QString path)
{
	QDir dcimDir(path + "DCIM");
	QStringList imageDirList = dcimDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	bool nameSet = false;

	QString sdName = "";//影像SD的名称
	int totalSize = 0;	//影像数量
	if (!imageDirList.isEmpty())
	{
		for each (QString var in imageDirList)
		{
			QDir imageDir = QDir(dcimDir.absolutePath() + "/" + imageDirList.first());
			QStringList filters;
			filters << "*.jpg" << "*.png" << "*.arw" << "*.cr2";
			imageDir.setNameFilters(filters);
			QStringList list = imageDir.entryList();
			totalSize += list.size();

			if (!list.isEmpty() && !nameSet)
			{
				QString file = list.first();
				if (file.startsWith("FT") || file.startsWith("ZH"))
					sdName = file.at(2);
				else
					sdName = QString::number(++m_camNum);

				sd.nickname = sdName;
				nameSet = true;
			}
		}
	}

	sd.imageSize = totalSize;
}

void CamSort::sortImage()
{

}

void CamSort::showSorties(QMap<int, int> sorties)
{
	//移除所有sortLayout下的widgets
	QObjectList	children = m_sortLayout->children();
	Q_FOREACH (QObject *obj, children)
	{
		CustomSortie *sortie = qobject_cast<CustomSortie*>(obj);
		m_sortLayout->removeWidget(sortie);
	}

	//添加架次
	m_sortieSelectStatus.clear();
	QMapIterator<int, int> iter(sorties);
	while (iter.hasNext())
	{
		iter.next();
		m_sortieSelectStatus[iter.key()] = false;
		CustomSortie *sortie = new CustomSortie(iter.key(), iter.value());
		connect(sortie, &CustomSortie::signalCheckChanged, this, &CamSort::sortieSelectChanged);
		connect(sortie, &CustomSortie::signalSortieDetail, this, &CamSort::showSortieDetail);

		m_sortLayout->addWidget(sortie);
	}
}

void CamSort::showSortieDetail(int sortie)
{

}

void CamSort::sortieSelectChanged(int state, int sortie)
{
	m_sortieSelectStatus[sortie] = state;
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

void CamSort::getImageExif()
{

}
