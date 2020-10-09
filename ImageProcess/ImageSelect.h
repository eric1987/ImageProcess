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
	QStringList getImages();

signals:
	void signalSortedImages(QMap<int, QList<ImageInfo>> imageData);
	void signalImageEdit(bool b);

private:
	void init();
	void connects();

	void slotAddImages();	//���Ӱ��
	void slotAddFolders();	//���Ӱ���ļ���
	void slotClearImages();	//���Ӱ��
	QStringList getDirImages(QString path);	//��ȡ·���µ�Ӱ��
	void addImageToTable(QStringList images);	//��Ӱ��ת��Ϊmodel
	

private:
	Ui::ImageSelect ui;
	QStringList m_images;
	QStringList m_headList;
	int m_row = 0;
	const int m_columnName = 0;
	const int m_columnPath = 1;
	QMap<int, QList<ImageInfo>> m_imageData;
};
