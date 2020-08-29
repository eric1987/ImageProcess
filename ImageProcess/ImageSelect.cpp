#include "ImageSelect.h"

ImageSelect::ImageSelect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ImageSelect::~ImageSelect()
{
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
	fileDialog->setWindowTitle(QStringLiteral("Ñ¡ÔñposÎÄ¼þ"));
	fileDialog->setNameFilter(tr("Images(*.txt)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	fileDialog->setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
	QStringList fileNames;
	if (fileDialog->exec())
	{
		fileNames = fileDialog->selectedFiles();
		m_images += fileNames;
	}

}

void ImageSelect::slotAddFolders()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	QStringList images = getDirImages(path);
}

void ImageSelect::slotClearImages()
{

}

QStringList ImageSelect::getDirImages(QString path)
{
	return QStringList();
}
