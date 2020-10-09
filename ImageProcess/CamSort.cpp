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

void CamSort::init()
{
	ui.selectPosType->addItem(QStringLiteral("��"));
	ui.selectPosType->addItem(QStringLiteral("�ݺ�"));

	ui.startSave->setCheckable(true);
	
	m_sortLayout = new QVBoxLayout;
	m_sortLayout->setContentsMargins(0, 0, 0, 0);
	m_sortLayout->setSpacing(5);
	ui.sortWidget->setLayout(m_sortLayout);
	ui.tabWidget->setTabsClosable(true);
	ui.tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, nullptr);
	m_transfer = new Transfer;

	readConfig();
	refreshSDInfo();
}

void CamSort::connects()
{
	connect(ui.selectPosFile, &QPushButton::clicked, this, &CamSort::selectPosFiles);
	connect(ui.selectPosType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &CamSort::changePosType);
	connect(ui.refreshSD, &QPushButton::clicked, this, &CamSort::refreshSDInfo);
	connect(ui.startSort, &QPushButton::clicked, this, &CamSort::getSorties);

	connect(ui.startSave, &QToolButton::clicked, this, &CamSort::sortTransfer);
	connect(ui.selectSavePath, &QPushButton::clicked, this, &CamSort::setSavePath);
	connect(ui.lineEdit, &QLineEdit::textChanged, this, &CamSort::changeSavePath);
	connect(ui.checkBox, &QCheckBox::stateChanged, this, &CamSort::setCheckSorties);

	connect(m_transfer, &Transfer::signalProcess, this, &CamSort::refreshProcess, Qt::QueuedConnection);
	connect(m_transfer, &Transfer::signalSortieTransFinished, this, &CamSort::transFinished, Qt::QueuedConnection);
	connect(ui.tabWidget, &QTabWidget::tabCloseRequested, this, &CamSort::onclosed);
}

void CamSort::selectPosFiles()
{
	ui.posWidget->clear();
	QFileDialog *fileDialog = new QFileDialog;
	fileDialog->setWindowTitle(QStringLiteral("ѡ��pos�ļ�"));
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
	Log::INFO(QStringLiteral("��ʼ������������̡�"));
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
			//�Ѵ���
			CamSD *sd = m_sdList[iter.key()];
			sd->setName(iter->nickname, iter->path);
			sd->setAvailableAndCollective(iter->available, iter->collective);
			sd->setOnlineStatus(iter->connected);
			//m_sdList[iter.key()]->setOnlineStatus(iter->connected);
		}
		else
		{
			//δ��
			CamSD *sd = new CamSD;
			sd->setName(iter->nickname, iter->path);
			sd->setAvailableAndCollective(iter->available, iter->collective);
			m_sdList[iter.key()] = sd;
			ui.camLayout->addWidget(sd);
		}
	}

	emit signalSDInfo(m_sdInfo);

	getImageExif();
}

void CamSort::getSDInfo(QString path)
{
	QDir dir(path);
	QStringList dirlist = dir.entryList(QDir::Dirs);
	QStorageInfo info(dir);

	//�ж��Ƿ�ΪӰ��SD��
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

	QString sdName = "";//Ӱ��SD������
	int totalSize = 0;	//Ӱ������
	if (!imageDirList.isEmpty())
	{
		for each (QString var in imageDirList)
		{
			QDir imageDir = QDir(dcimDir.absolutePath() + "/" + var);
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

void CamSort::getSorties()
{
	if (m_posFiles.isEmpty())
	{
		QMessageBox::information(this, QStringLiteral(""), QStringLiteral("δ��⵽pos�ļ���Ӱ��·����"));
		return;
	}

	if (m_posSort == nullptr)
	{
		m_posSort = new PosSorting;
	}

	emit signalStatus(QStringLiteral("���ڽ��мܴη��ࡣ"));
	QThread *thread = new QThread;
	m_posSort->setPosType(m_posType);
	m_posSort->setPosFiles(m_posFiles);
	m_posSort->setDisks(&m_disks);
	m_posSort->moveToThread(thread);

	qRegisterMetaType<PosInfo>();
	qRegisterMetaType<ImageInfo>();

	connect(m_posSort, &PosSorting::signalPosData, this, &CamSort::prepareShowSorties, Qt::QueuedConnection);
	connect(thread, &QThread::started, m_posSort, &PosSorting::getPosSorted);
	connect(thread, &QThread::finished, m_posSort, &QObject::deleteLater);
	thread->start();
}

void CamSort::prepareShowSorties(QMap<int, QList<PosInfo>> posData)
{
	emit signalStatus(QStringLiteral("�ܴη�������ɡ�"));
	QMapIterator<QString, UDisk *> it(m_disks);
	QList<int> sorts = posData.keys();
	//�Դ���ܴ�״̬����ʼ��
	while (it.hasNext())
	{
		it.next();
		Q_FOREACH(int sort, sorts)
		{
			it.value()->transImage[sort] = false;
		}
	}

	m_posData = posData;
	QMap<int, int> sorties;
	QMapIterator<int, QList<PosInfo>> iter(posData);
	while (iter.hasNext())
	{
		iter.next();
		sorties.insert(iter.key(), iter.value().size());
	}
	showSorties(sorties);
}

void CamSort::showSorties(QMap<int, int> sorties)
{
	//�Ƴ�����sortLayout�µ�widgets
	QObjectList	children = ui.sortWidget->children();
	Q_FOREACH (QObject *obj, children)
	{
		CustomSortie *sortie = qobject_cast<CustomSortie*>(obj);
		m_sortLayout->removeWidget(sortie);
		delete sortie;
		sortie = nullptr;
	}

	//��Ӽܴ�
	m_sortieSelectStatus.clear();
	m_sortieIndex.clear();
	QMapIterator<int, int> iter(sorties);
	while (iter.hasNext())
	{
		iter.next();
		QMap<QString, int> imageSize;
		int posSize = iter.value();
		QMapIterator<QString, UDisk*> iterImage(m_disks);
		while (iterImage.hasNext())
		{
			iterImage.next();
			int size = iterImage.value()->imageData[iter.key()].size();
			imageSize.insert(iterImage.key(), size);
		}
		m_sortieSelectStatus[iter.key()] = false;
		bool ret = Util::compareImageAndPos(imageSize, posSize);
		CustomSortie *sortie = new CustomSortie(iter.key(), iter.value(), ret, ui.sortWidget);
		connect(sortie, &CustomSortie::signalCheckChanged, this, &CamSort::sortieSelectChanged);
		connect(sortie, &CustomSortie::signalSortieDetail, this, &CamSort::showSortieDetail);

		m_sortLayout->addWidget(sortie);
	}
}

void CamSort::showSortieDetail(int sortie)
{
	if (m_sortieIndex.contains(sortie))
	{
		ui.tabWidget->setCurrentIndex(m_sortieIndex.value(sortie));
	}
	else
	{
		SortieWidget *widget = new SortieWidget;
		bool ret = widget->showDetail(&m_disks, m_posData[sortie], sortie);
		QString str = ret ? QStringLiteral("һ��") : QStringLiteral("��һ��");
		ui.tabWidget->addTab(widget, QStringLiteral("�ܴ�%1-%2").arg(sortie).arg	(str));
		int index = ui.tabWidget->count();
		m_sortieIndex.insert(sortie, index - 1);
		ui.tabWidget->setCurrentIndex(index - 1);
	}
}

void CamSort::sortieSelectChanged(int state, int sortie)
{
	m_sortieSelectStatus[sortie] = state;
}

void CamSort::sortTransfer()
{
	ui.progressBar->setValue(0);

	if (ui.startSave->isChecked())	//����
	{
		m_transferSortie.clear();

		QMapIterator<int, bool> iter(m_sortieSelectStatus);
		while (iter.hasNext())
		{
			iter.next();
			if (iter.value())
			{
				m_transferSortie.append(iter.key());
			}
		}

		if ("" == m_savePath || m_transferSortie.isEmpty())
		{
			QMessageBox::information(this, QStringLiteral("����"), QStringLiteral("��ѡ��洢·����Ҫ����ļܴΡ�"));
			return;
		}

		if (m_transfer == nullptr)
		{
			m_transfer = new Transfer;
		}
		
		m_transfer->setTransType(SORTIE);
		m_transfer->setDstPath(m_savePath);
		m_transfer->setTransSortie(m_transferSortie, &m_disks);
		m_transfer->start();

		ui.startSave->setText(QStringLiteral("ֹͣ����"));
		ui.startSave->setChecked(true);
		unableOperation();

		rePosAndSave(m_transferSortie);
	}
	else       //ֹͣ����
	{
		ui.startSave->setText(QStringLiteral("��ʼ����"));
		ui.startSave->setChecked(false);
		enableOperation();
		m_transfer->stopTransfer();
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
	Log::INFO(QStringLiteral("��ȡӰ��exif��Ϣ��"));
	
	QMapIterator<QString, SDInfo> iter(m_sdInfo);
	while (iter.hasNext())
	{
		iter.next();
		UDisk *disk = nullptr;
		if (m_disks.contains(iter.key()))
		{
			disk = m_disks[iter.key()];
		}
		else
		{
			disk = new UDisk;
			disk->diskID = iter.key();
			disk->path = iter.value().path + "DCIM";
			disk->cam = iter.value().nickname;
			m_disks.insert(iter.key(), disk);
		}
		
		if (!disk->sortieStatus)
		{
			QThread *thread = new QThread;
			Image *image = new Image;
			image->setUDiskInfo(disk);
			image->moveToThread(thread);
			connect(thread, &QThread::started, image, &Image::readInfo);
			connect(thread, &QThread::finished, thread, &QThread::deleteLater);
			thread->start();
		}
		else
		{
			continue;
		}
	}
}

void CamSort::refreshProcess(int value)
{
	ui.progressBar->setValue(value);
}

void CamSort::transFinished()
{
	ui.progressBar->setValue(100);
	QMessageBox::information(this, QStringLiteral(""), QStringLiteral("�ܴ��ļ�������ɡ�"));
	Log::INFO(QStringLiteral("�ܴ��ļ������ļ���"));
	ui.startSave->setText(QStringLiteral("��ʼ����"));
	ui.startSave->setChecked(false);
	enableOperation();
}

void CamSort::rePosAndSave(QList<int> sorties)
{
	//����Ӱ����pos������ͬ�ļܴ�
	QList<int> rePos;
	Q_FOREACH(int i, sorties)
	{
		int sizePos = m_posData.value(i).size();
		QMapIterator<QString, UDisk *> iter(m_disks);
		bool isSizeSame = true;
		while (iter.hasNext())
		{
			iter.next();
			if (iter.value()->imageData[i].size() != sizePos)
			{
				isSizeSame = false;
			}
		}
		if (isSizeSame)
		{
			rePos.append(i);
		}
	}

	Q_FOREACH(int i, rePos)
	{
		QMapIterator<QString, UDisk *> iter(m_disks);
		while (iter.hasNext())
		{
			iter.next();
			QString posFile = setSaveReposFileName(iter.value()->cam, i, m_savePath);
			QFile file(posFile);
			if (!file.open(QIODevice::WriteOnly))
			{
				Log::INFO(QStringLiteral("��reppos�ļ�ʧ�ܡ�"));
				return;
			}
			QTextStream out(&file);
			
			QList<ImageInfo> images = iter.value()->imageData[i];
			QList<PosInfo> pos = m_posData.value(i);
			for (int k = 0; k < images.size(); k++)
			{
				QString insertStr = QString("%1%2").arg(Util::getParentFloder(images[k].fileName)).arg(i, 2, 10, QLatin1Char('0'));
				QString fileName = images[k].imageName.insert(2, insertStr);

				out << fileName << " " << pos[k].time
					<< " " << pos[k].strLat << " " << pos[k].strLong
					<< " " << pos[k].strAlt << endl;
			}
			file.close();
		}
	}
}

QString CamSort::setSaveReposFileName(QString cam, int sortie, QString path)
{
	QDir dir(path);
	QString file = dir.absolutePath() + QStringLiteral("/���") + cam + QStringLiteral("�ܴ�%1.txt").arg(sortie);
	return file;
}

void CamSort::setCheckSorties(int state)
{
	QObjectList	children = ui.sortWidget->children();
	Q_FOREACH(QObject *obj, children)
	{
		CustomSortie *sortie = qobject_cast<CustomSortie*>(obj);
		if (sortie != nullptr)
		{
			sortie->setState(state);
		}
	}
}

void CamSort::readConfig()
{
	QFile file(QString("./config.ini"));
	if (!file.open(QIODevice::ReadOnly))
	{
		Log::INFO(QStringLiteral("�������ļ�ʧ�ܡ�"));
		return;
	}
	QString fightGap = file.readLine();
	QString fightMinImages = file.readLine();
	QString altitudeIntercept = file.readLine();
	QString imageSizeDiff = file.readLine();
	
	int gap;
	decodeConfig(fightGap, gap);
	int min;
	decodeConfig(fightMinImages, min);
	int alt;
	decodeConfig(altitudeIntercept, alt);
	int sizeDiff;
	decodeConfig(imageSizeDiff, sizeDiff);

	Image::setConfig(gap, sizeDiff, min);
	PosSorting::setConfig(gap, alt, min);

	Log::INFO(QStringLiteral("��ȡ�����ļ��ɹ�"));
	Log::INFO(QString("fightGap: %1").arg(gap));
	Log::INFO(QString("fightMinImages: %1").arg(min));
	Log::INFO(QString("altitudeIntercept: %1").arg(alt));
	Log::INFO(QString("imageSizeDiff: %1").arg(sizeDiff));
}

void CamSort::decodeConfig(QString str, int &value)
{
	QStringList configs = str.split(" ");
	if (configs.size() == 3)
	{
		value = configs[2].toInt();
	}
}

void CamSort::onclosed(int index)
{
	if (index != 0)
	{
		//ui.tabWidget->removeTab(index);
		delete ui.tabWidget->widget(index);
		int key = m_sortieIndex.key(index);
		m_sortieIndex.remove(key);
	}
}

