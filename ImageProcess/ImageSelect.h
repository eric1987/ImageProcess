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
	//Ӱ�����
	void sortImages();
	//��ȡӰ��
	QStringList getImages();

signals:
	//���ͷ���õ�Ӱ��
	void signalSortedImages(QMap<int, QList<ImageInfo>> imageData);
	//Ӱ������ź�
	void signalImageEdit(bool b);

private:
	void init();		//��ʼ��
	void connects();	//�źŰ�

	void slotAddImages();	//���Ӱ��
	void slotAddFolders();	//���Ӱ���ļ���
	void slotClearImages();	//���Ӱ��
	QStringList getDirImages(QString path);	//��ȡ·���µ�Ӱ��
	void addImageToTable(QStringList images);	//��Ӱ��ת��Ϊmodel
	

private:
	Ui::ImageSelect ui;
	QStringList m_images;		//Ӱ������
	QStringList m_headList;		//��ͷ����
	int m_row = 0;				//����
	const int m_columnName = 0;	//��0�У���ʾ����
	const int m_columnPath = 1;	//��1�У���ʾ·��
	QMap<int, QList<ImageInfo>> m_imageData;	//����ʹ��
};
