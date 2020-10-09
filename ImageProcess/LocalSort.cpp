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
	m_sortLayout = new QVBoxLayout;
	ui.sortWidget->setLayout(m_sortLayout);
	ui.tabWidget->setTabsClosable(true);
	ui.tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, nullptr);
	m_transfer = new Transfer;
}

void LocalSort::connects()
{
	connect(ui.selectPosFile, &QPushButton::clicked, this, &LocalSort::slotOpenPos);
	connect(ui.selectPosType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &LocalSort::slotChangePosType);
	connect(ui.startSort, &QPushButton::clicked, this, &LocalSort::slotStartSort);
	connect(ui.checkBox, &QCheckBox::stateChanged, this, &LocalSort::setCheckSorties);
	connect(ui.widget1, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
	connect(ui.widget2, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
	connect(ui.widget3, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
	connect(ui.widget4, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
	connect(ui.widget5, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
	connect(ui.widget6, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);

	connect(ui.widget1, &ImageSelect::signalImageEdit, this, &LocalSort::imageAdd);
	connect(ui.widget2, &ImageSelect::signalImageEdit, this, &LocalSort::imageAdd);
	connect(ui.widget3, &ImageSelect::signalImageEdit, this, &LocalSort::imageAdd);
	connect(ui.widget4, &ImageSelect::signalImageEdit, this, &LocalSort::imageAdd);
	connect(ui.widget5, &ImageSelect::signalImageEdit, this, &LocalSort::imageAdd);
	connect(ui.widget6, &ImageSelect::signalImageEdit, this, &LocalSort::imageAdd);
	//connect(ui.tabWidget1, &QTabWidget::tabBarClicked, this, &LocalSort::addCamTab);

	connect(ui.lineEdit, &QLineEdit::textChanged, this, &LocalSort::blockPathChanged);
	connect(ui.selectPath, &QPushButton::clicked, this, &LocalSort::blockPathSelected);
	//connect(ui.genBlockBtn, &QPushButton::clicked, this, &LocalSort::createBlockFile);
	connect(ui.startSave, &QToolButton::clicked, this, &LocalSort::sortTransfer);
	connect(m_transfer, &Transfer::signalProcess, this, &LocalSort::refreshProcess, Qt::QueuedConnection);
	connect(m_transfer, &Transfer::signalSortieTransFinished, this, &LocalSort::transFinished, Qt::QueuedConnection);
	connect(ui.tabWidget, &QTabWidget::tabCloseRequested, this, &LocalSort::onclosed);
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

void LocalSort::slotStartSort()
{
	if (m_posFiles.isEmpty())
	{
		QMessageBox::information(this, QStringLiteral(""), QStringLiteral("未检测到pos文件或影像路径。"));
		return;
	}
	if (m_posSort == nullptr)
	{
		m_posSort = new PosSorting;
	}

	m_imageisReady.clear();
	for (int i = 0; i < 6; i++)
	{
		m_imageisReady[0] = false;
	}

	emit signalStatus(QStringLiteral("正在进行架次分类。"));	
	QThread *thread = new QThread;
	m_posSort->setPosType(m_posType);
	m_posSort->setPosFiles(m_posFiles);
	m_posSort->moveToThread(thread);

	qRegisterMetaType<PosInfo>();
	qRegisterMetaType<ImageInfo>();

	connect(m_posSort, &PosSorting::signalPosData, this, &LocalSort::receivePosData, Qt::QueuedConnection);
	connect(thread, &QThread::started, m_posSort, &PosSorting::getLocPosSorted);
	connect(thread, &QThread::finished, m_posSort, &QObject::deleteLater);
	thread->start();

	ui.widget1->sortImages();
	ui.widget2->sortImages();
	ui.widget3->sortImages();
	ui.widget4->sortImages();
	ui.widget5->sortImages();
	ui.widget6->sortImages();
}

void LocalSort::receivePosData(QMap<int, QList<PosInfo>> posData)
{
	m_posData = posData;
}

bool LocalSort::isDataPrepared()
{
	QMapIterator<int, bool> iter(m_imageisReady);
	while (iter.hasNext())
	{
		iter.next();
		if (!iter.value())
		{
			return false;
		}
	}
	return true;
}

void LocalSort::receiveImageData(QMap<int, QList<ImageInfo>> imageData)
{
	ImageSelect *send = qobject_cast<ImageSelect *>(sender());
	if (send == ui.widget1)
	{
		m_image1 = imageData;
		m_images[0] = imageData;
		m_imageisReady[0] = true;
	}
	else if (send == ui.widget2)
	{
		m_image2 = imageData;
		m_images[1] = imageData;
		m_imageisReady[1] = true;
	}
	else if (send == ui.widget3)
	{
		m_image3 = imageData;
		m_images[2] = imageData;
		m_imageisReady[2] = true;
	}
	else if (send == ui.widget4)
	{
		m_image4 = imageData;
		m_images[3] = imageData;
		m_imageisReady[3] = true;
	}
	else if (send == ui.widget5)
	{
		m_image5 = imageData;
		m_images[4] = imageData;
		m_imageisReady[4] = true;
	}
	else if (send == ui.widget6)
	{
		m_image6 = imageData;
		m_images[5] = imageData;
		m_imageisReady[5] = true;
	}
	if (!imageData.isEmpty())
	{
		m_localImages[getImageName(imageData)] = imageData;
	}

	if (isDataPrepared())
	{
		sortiesDisplay();
	}
}

void LocalSort::sortiesDisplay()
{
	QObjectList	children = ui.sortWidget->children();
	Q_FOREACH(QObject *obj, children)
	{
		CustomSortie *sortie = qobject_cast<CustomSortie*>(obj);
		m_sortLayout->removeWidget(sortie);
		delete sortie;
		sortie = nullptr;
	}

	//添加架次
	m_sortieSelectStatus.clear();
	m_sortieIndex.clear();
	QMapIterator<int, QList<PosInfo>> iter(m_posData);
	while (iter.hasNext())
	{
		iter.next();
		m_sortieSelectStatus[iter.key()] = false;
		QMap<QString, int> imageSize;
		int posSize = iter.value().size();
		QMapIterator<QString, QMap<int, QList<ImageInfo>>> iterImage(m_localImages);
		while (iterImage.hasNext())
		{
			iterImage.next();
			int size = iterImage.value()[iter.key()].size();
			imageSize.insert(iterImage.key(), size);
		}
		bool ret = Util::compareImageAndPos(imageSize, posSize);
		CustomSortie *sortie = new CustomSortie(iter.key(), iter.value().size(), ret, ui.sortWidget);
		connect(sortie, &CustomSortie::signalCheckChanged, this, &LocalSort::sortieSelectChanged);
		connect(sortie, &CustomSortie::signalSortieDetail, this, &LocalSort::showSortieDetail);

		m_sortLayout->addWidget(sortie);
	}
}

void LocalSort::sortieSelectChanged(int state, int sortie)
{
	m_sortieSelectStatus[sortie] = state;
}

void LocalSort::showSortieDetail(int sortie)
{
	if (m_sortieIndex.contains(sortie))
	{
		ui.tabWidget->setCurrentIndex(m_sortieIndex.value(sortie));
	}
	else
	{
		SortieWidget *widget = new SortieWidget;
		QMap<QString, QList<ImageInfo>> cams = getSortieImage(sortie);

		bool ret = widget->showDetail(cams, m_posData[sortie], sortie);
		QString str = ret ? QStringLiteral("一致") : QStringLiteral("不一致");
		ui.tabWidget->addTab(widget, QStringLiteral("架次%1-%2").arg(sortie).arg(str));
		int index = ui.tabWidget->count();
		m_sortieIndex.insert(sortie, index - 1);
		ui.tabWidget->setCurrentIndex(index - 1);
	}
}

void LocalSort::imageAdd(bool state)
{
	ImageSelect *send = qobject_cast<ImageSelect *>(sender());
	if (send == ui.widget1)
	{
		m_cams[0] = state;
	}
	else if (send == ui.widget2)
	{
		m_cams[1] = state;
	}
	else if (send == ui.widget3)
	{
		m_cams[2] = state;
	}
	else if (send == ui.widget4)
	{
		m_cams[3] = state;
	}
	else if (send == ui.widget5)
	{
		m_cams[4] = state;
	}
	else if (send == ui.widget6)
	{
		m_cams[5] = state;
	}
}

void LocalSort::addCamTab(int index)
{
	if (index == m_addIndex)
	{
		ImageSelect *select = new ImageSelect;
		ui.posAndImage->insertTab(m_addIndex, select, QString("cam%1").arg(m_addIndex));
		m_addIndex++;
	}
}

QMap<QString, QList<ImageInfo>> LocalSort::getSortieImage(int sortie)
{
	QMap<QString, QList<ImageInfo>> cams;
	for (size_t index = 0; index < m_cams.size(); index++)
	{
		if (m_cams[index] == true)
		{
			cams.insert(QString("cam%1").arg(index + 1), m_images.value(index).value(sortie));
		}
	}

	return cams;
}

QMap<int, QMap<int, QList<ImageInfo>>> LocalSort::needToGenBlockSorts()
{
	QMap<int, QMap<int, QList<ImageInfo>>> cams;
	QMapIterator<int, QMap<int, QList<ImageInfo>>> iter(m_images);
	while (iter.hasNext())
	{
		iter.next();
		QMap<int, QList<ImageInfo>> images;
		for (size_t i = 0; i < m_sortieSelectStatus.size(); i++)
		{
			if (m_sortieSelectStatus[i] == true)
			{
				images.insert(i, iter.value().value(i));
			}
		}
		cams.insert(iter.key(), images);
	}
	return cams;
}

void LocalSort::createBlockFile()
{
	QDir dir(m_savePath);
	QString path = dir.absolutePath();
	if (path.endsWith('/'))
	{
		path = path.left(path.length()-1);
	}
	QString file = path +"/" + "block.xlsx";
	//QMap<int, QMap<int, QList<ImageInfo>>> images = needToGenBlockSorts();
	//QList<Block> content = genBlockContent();

	QList<int> sorties;
	QMapIterator<int, bool> iter(m_sortieSelectStatus);
	while (iter.hasNext())
	{
		iter.next();
		if (iter.value() == true)
		{
			sorties.append(iter.key());
		}
	}

	QMap<int, QMap<int, QList<ImageInfo>>> images;
	QMapIterator<int, bool> iterCam(m_cams);
	while (iterCam.hasNext())
	{
		iterCam.next();
		if (iterCam.value() == true)
		{
			images.insert(iterCam.key(), m_images.value(iterCam.key()));
		}
	}

	GenBlockFile *genBlock = new GenBlockFile;
	genBlock->setFileAndImages(file, images);
	genBlock->setPosAndSorties(m_posData, sorties);
	connect(genBlock, &GenBlockFile::signalFinished, this, &LocalSort::slotGenBlockFinished);
	genBlock->start();
}

void LocalSort::slotGenBlockFinished(bool b)
{
	if (b)
	{
		QMessageBox::information(this, QStringLiteral("通知"), QStringLiteral("Block文件生成完毕。"));
	}
	else
	{
		QMessageBox::information(this, QStringLiteral("通知"), QStringLiteral("Block文件生成失败，请先进行架次分类并检查是否一致。"));
	}
}

void LocalSort::blockPathChanged()
{
	m_savePath = ui.lineEdit->text();
}

void LocalSort::blockPathSelected()
{
	m_savePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	ui.lineEdit->setText(m_savePath);
}

void LocalSort::sortTransfer(bool checked)
{
	ui.progressBar->setValue(0);
	if (checked)	//传输
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
			QMessageBox::information(this, QStringLiteral("警告"), QStringLiteral("请选择存储路径与要传输的架次。"));
			return;
		}
		m_transfer->setTransType(LOCAL);
		m_transfer->setDstPath(m_savePath);
		m_transfer->setTransImages(m_localImages);
		m_transfer->setLocalTransSortie(m_transferSortie);
		m_transfer->start();
		ui.startSave->setText(QStringLiteral("停止传输"));
		//ui.startSave->setChecked(true);
		unableOperation();
		rePosAndSave(m_transferSortie);
	}
	else       //停止传输
	{
		ui.startSave->setText(QStringLiteral("开始传输"));
		//ui.startSave->setChecked(false);
		enableOperation();
		m_transfer->stopTransfer();
	}
}

void LocalSort::unableOperation()
{
	ui.selectPosFile->setDisabled(true);
	ui.selectPosType->setDisabled(true);
	ui.startSort->setDisabled(true);
	ui.selectPath->setDisabled(true);
	ui.lineEdit->setDisabled(true);
}

void LocalSort::enableOperation()
{
	ui.selectPosFile->setEnabled(true);
	ui.selectPosType->setEnabled(true);
	ui.startSort->setEnabled(true);
	ui.selectPath->setEnabled(true);
	ui.lineEdit->setEnabled(true);
}

void LocalSort::rePosAndSave(QList<int> sorties)
{
	//查找影像与pos数量相同的架次
	QList<int> rePos;
	Q_FOREACH(int i, sorties)
	{
		int sizePos = m_posData.value(i).size();
		QMapIterator<QString, QMap<int,QList<ImageInfo>>> iter(m_localImages);
		bool isSizeSame = true;
		while (iter.hasNext())
		{
			iter.next();
			int size = iter.value()[i].size();
			if (size != sizePos)
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
		QMapIterator<QString, QMap<int, QList<ImageInfo>>> iter(m_localImages);
		while (iter.hasNext())
		{
			iter.next();
			QString posFile = setSaveReposFileName(iter.key(), i, m_savePath);
			QFile file(posFile);
			if (!file.open(QIODevice::WriteOnly))
			{
				Log::INFO(QStringLiteral("打开reppos文件失败。"));
				return;
			}
			QTextStream out(&file);

			QList<ImageInfo> images = iter.value()[i];
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

QString LocalSort::getImageName(QMap<int, QList<ImageInfo>> &images)
{
	QString imageName;
	if (!images.isEmpty())
	{
		if (!images.first().isEmpty())
		{
			imageName = images.first().first().imageName;
		}
	}
	QString nickName;
	if (imageName.startsWith("FT") || imageName.startsWith("ZH"))
	{
		if (imageName[2] >= 'A' && imageName[2] <= 'Z')
		{
			nickName = imageName[2];
		}
		else if (imageName[5] >= 'A' && imageName[5] <= 'Z')
		{
			nickName = imageName[5];
		}
	}
	return nickName;
}

QString LocalSort::setSaveReposFileName(QString cam, int sortie, QString path)
{
	QDir dir(path);
	QString file = dir.absolutePath() + QStringLiteral("/相机") + cam + QStringLiteral("架次%1.txt").arg(sortie);
	return file;
}

void LocalSort::refreshProcess(int process)
{
	ui.progressBar->setValue(process);
}

void LocalSort::transFinished()
{
	ui.progressBar->setValue(100);
	QMessageBox::information(this, QStringLiteral(""), QStringLiteral("架次文件传输完成。"));
	Log::INFO(QStringLiteral("架次文件传输文件。"));
	ui.startSave->setText(QStringLiteral("开始传输"));
	ui.startSave->setChecked(false);
	enableOperation();
}

void LocalSort::onclosed(int index)
{
	if (index != 0)
	{
		//ui.tabWidget->removeTab(index);
		delete ui.tabWidget->widget(index);
		int key = m_sortieIndex.key(index);
		m_sortieIndex.remove(key);
	}
}

void LocalSort::setCheckSorties(int state)
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
