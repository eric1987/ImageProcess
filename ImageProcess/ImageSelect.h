//TODO: slotAddImage, slotAddFolders, slotClearImages
#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStandardItem>
#include <QThread>

#include "ui_ImageSelect.h"
#include "Common.h"
#include "Image.h"

class ImageSelect : public QWidget
{
	Q_OBJECT

public:
	ImageSelect(QWidget *parent = Q_NULLPTR);
	~ImageSelect();

	void sortImages();

signals:
	void signalSortedImages(QMap<int, QList<ImageInfo>> imageData);
	void signalImageEdit(bool b);

private:
	void init();
	void connects();

	void slotAddImages();	//添加影像
	void slotAddFolders();	//添加影像文件夹
	void slotClearImages();	//清除影像
	QStringList getDirImages(QString path);	//获取路径下的影像
	void addImageToTable(QStringList images);	//将影像转化为model

private:
	Ui::ImageSelect ui;
	QStringList m_images;
	QStringList m_headList;
	int m_row = 0;
	const int m_columnName = 0;
	const int m_columnPath = 1;
	QMap<int, QList<ImageInfo>> m_imageData;
};
