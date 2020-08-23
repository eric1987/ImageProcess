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

}

void ImageSelect::slotAddFolders()
{

}

void ImageSelect::slotClearImages()
{

}
