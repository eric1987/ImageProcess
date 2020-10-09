#include "ImageSelect.h"

ImageSelect::ImageSelect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
	connects();
}

ImageSelect::~ImageSelect()
{
}

void ImageSelect::sortImages()
{
	m_imageData.clear();
	
	Image *image = new Image;
	QThread *thread = new QThread;
	image->moveToThread(thread);
	image->setImages(m_images);

	connect(image, &Image::signalInfos, this, &ImageSelect::signalSortedImages, Qt::QueuedConnection);
	connect(thread, &QThread::started, image, &Image::readAndSortLocalImage);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);
	thread->start();
}

void ImageSelect::init()
{
	m_headList.clear();
	m_headList << QStringLiteral("文件名") << QStringLiteral("文件路径");
	ui.tableWidget->setHorizontalHeaderLabels(m_headList);
	ui.tableWidget->setColumnCount(2);
}

void ImageSelect::connects()
{
	connect(ui.addImage, &QPushButton::clicked, this, &ImageSelect::slotAddImages);
	connect(ui.addFolder, &QPushButton::clicked, this, &ImageSelect::slotAddFolders);
	connect(ui.clearImage, &QPushButton::clicked, this, &ImageSelect::slotClearImages);
}

void ImageSelect::slotAddImages()
{
	QFileDialog *fileDialog = new QFileDialog;
	fileDialog->setWindowTitle(QStringLiteral("选择pos文件"));
	fileDialog->setNameFilter(tr("Image(*.jpg *.png *.arw *.cr2)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	fileDialog->setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
	QStringList fileNames;
	if (fileDialog->exec())
	{
		fileNames = fileDialog->selectedFiles();
		m_images += fileNames;
		addImageToTable(fileNames);
	}
	if (!m_images.isEmpty())
	{
		emit signalImageEdit(true);
	}
}

void ImageSelect::slotAddFolders()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	QStringList images = getDirImages(path);
	m_images += images;
	addImageToTable(images);
	if (!m_images.isEmpty())
	{
		emit signalImageEdit(true);
	}
}

void ImageSelect::slotClearImages()
{
	m_images.clear();
	ui.tableWidget->clear();
	ui.tableWidget->setRowCount(0);
	m_row = 0;

	emit signalImageEdit(false);
}

QStringList ImageSelect::getDirImages(QString path)
{
	QDir dir(path);
	QString basePath = dir.absolutePath() + "/";
	QStringList paths = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	QStringList filters;
	filters << "*.jpg" << "*.png" << "*.arw" << "*.cr2";
	dir.setNameFilters(filters);
	QStringList files = dir.entryList();
	for (int i = 0; i < files.size(); i++)
	{
		files[i] = basePath + files[i];
	}

	Q_FOREACH(QString subpath, paths)
	{
		files += getDirImages(basePath + subpath);
	}

	return files;
}

void ImageSelect::addImageToTable(QStringList images)
{
	Q_FOREACH(QString image, images)
	{
		int index = image.lastIndexOf('/');
		QString name = image.right(image.size() - index -1);
		QTableWidgetItem *itemName = new QTableWidgetItem(name);
		QTableWidgetItem *itemPath = new QTableWidgetItem(image);
		itemName->setTextAlignment(Qt::AlignJustify);
		itemPath->setTextAlignment(Qt::AlignJustify);

		ui.tableWidget->insertRow(m_row);
		ui.tableWidget->setItem(m_row, m_columnName, itemName);
		ui.tableWidget->setItem(m_row++, m_columnPath, itemPath);
	}
}

QStringList ImageSelect::getImages()
{
	return m_images;
}
