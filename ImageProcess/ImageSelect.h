//TODO: slotAddImage, slotAddFolders, slotClearImages
#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>

#include "ui_ImageSelect.h"

class ImageSelect : public QWidget
{
	Q_OBJECT

public:
	ImageSelect(QWidget *parent = Q_NULLPTR);
	~ImageSelect();

private:
	void connects();

	void slotAddImages();
	void slotAddFolders();
	void slotClearImages();
	QStringList getDirImages(QString path);	//获取路径下的影像
	void sortImages();

private:
	Ui::ImageSelect ui;
	QStringList m_images;
};
