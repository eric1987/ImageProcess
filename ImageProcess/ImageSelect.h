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
	//开始影像分类
	void sortImages();
	//获取影像
	QStringList getImages();

signals:
	//发送分类好的影像
	void signalSortedImages(QMap<int, QList<ImageInfo>> imageData);
	//影像添加信号
	void signalImageEdit(bool b);

private:
	void init();		//初始化
	void connects();	//信号绑定

	void slotAddImages();	//添加影像
	void slotAddFolders();	//添加影像文件夹
	void slotClearImages();	//清除影像
	QStringList getDirImages(QString path);	//获取路径下的影像
	void addImageToTable(QStringList images);	//将影像转化为model
	

private:
	Ui::ImageSelect ui;
	QStringList m_images;		//影像名称
	QStringList m_headList;		//表头名称
	int m_row = 0;				//行数
	const int m_columnName = 0;	//第0列：显示名称
	const int m_columnPath = 1;	//第1列：显示路径
	QMap<int, QList<ImageInfo>> m_imageData;	//影像分类数据
};
