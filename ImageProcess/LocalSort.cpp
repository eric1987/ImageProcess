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
}

void LocalSort::connects()
{
	connect(ui.selectPosFile, &QPushButton::clicked, this, &LocalSort::slotOpenPos);
	connect(ui.selectPosType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &LocalSort::slotChangePosType);
	connect(ui.startSort, &QPushButton::clicked, this, &LocalSort::slotStartSort);

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
	m_sortieWidgets.clear();
	QMapIterator<int, QList<PosInfo>> iter(m_posData);
	while (iter.hasNext())
	{
		iter.next();
		m_sortieSelectStatus[iter.key()] = false;
		CustomSortie *sortie = new CustomSortie(iter.key(), iter.value().size(), ui.sortWidget);
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
	if (m_sortieWidgets.contains(sortie))
	{
		ui.tabWidget->setCurrentIndex(sortie + 1);
	}
	else
	{
		SortieWidget *widget = new SortieWidget;
		m_sortieWidgets.insert(sortie, widget);
		QMap<QString, QList<ImageInfo>> cams = getSortieImage(sortie);

		bool ret = widget->showDetail(cams, m_posData[sortie], sortie);
		//bool ret = widget->showDetail(&m_disks, m_posData[sortie], sortie);
		QString str = ret ? QStringLiteral("一致") : QStringLiteral("不一致");
		ui.tabWidget->addTab(widget, QStringLiteral("架次%1-%2").arg(sortie).arg(str));
		ui.tabWidget->setCurrentIndex(sortie + 1);
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
		ui.tabWidget1->insertTab(m_addIndex, select, QString("cam%1").arg(m_addIndex));
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
