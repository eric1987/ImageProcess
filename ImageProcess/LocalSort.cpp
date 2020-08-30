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
	ui.selectPosType->addItem(QStringLiteral("��"));
	ui.selectPosType->addItem(QStringLiteral("�ݺ�"));
}

void LocalSort::connects()
{
	connect(ui.selectPosFile, &QPushButton::clicked, this, &LocalSort::slotOpenPos);
	connect(ui.selectPosType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &LocalSort::slotChangePosType);
	connect(ui.startSort, &QPushButton::clicked, this, &LocalSort::slotStartSort);

	connect(ui.widget, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
	connect(ui.widget_2, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
	connect(ui.widget_3, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
	connect(ui.widget_4, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
	connect(ui.widget_5, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
	connect(ui.widget_6, &ImageSelect::signalSortedImages, this, &LocalSort::receiveImageData);
}

void LocalSort::slotOpenPos()
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

void LocalSort::slotChangePosType(int index)
{
	m_posType = index;
}

void LocalSort::slotStartSort()
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

	m_imageisReady.clear();
	for (int i = 0; i < 6; i++)
	{
		m_imageisReady[0] = false;
	}

	emit signalStatus(QStringLiteral("���ڽ��мܴη��ࡣ"));	
	QThread *thread = new QThread;
	m_posSort->setPosType(m_posType);
	m_posSort->setPosFiles(m_posFiles);
	m_posSort->moveToThread(thread);

	qRegisterMetaType<PosInfo>();
	qRegisterMetaType<ImageInfo>();

	connect(m_posSort, &PosSorting::signalPosData, this, &LocalSort::receivePosData, Qt::QueuedConnection);
	connect(thread, &QThread::started, m_posSort, &PosSorting::getPosSorted);
	connect(thread, &QThread::finished, m_posSort, &QObject::deleteLater);
	thread->start();

	ui.widget->sortImages();
	ui.widget_2->sortImages();
	ui.widget_3->sortImages();
	ui.widget_4->sortImages();
	ui.widget_5->sortImages();
	ui.widget_6->sortImages();
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
	if (send == ui.widget)
	{
		m_image1 = imageData;
		m_imageisReady[0] = true;
	}
	else if (send == ui.widget_2)
	{
		m_image2 = imageData;
		m_imageisReady[1] = true;
	}
	else if (send == ui.widget_3)
	{
		m_image3 = imageData;
		m_imageisReady[2] = true;
	}
	else if (send == ui.widget_4)
	{
		m_image4 = imageData;
		m_imageisReady[3] = true;
	}
	else if (send == ui.widget_5)
	{
		m_image5 = imageData;
		m_imageisReady[4] = true;
	}
	else if (send == ui.widget_6)
	{
		m_image6 = imageData;
		m_imageisReady[5] = true;
	}

	if (isDataPrepared())
	{
		sortiesDisplay();
	}
}

void LocalSort::sortiesDisplay()
{
	QMapIterator<int, QList<PosInfo>> iter(m_posData);
	while (iter.hasNext())
	{
		iter.next();

	}
}
