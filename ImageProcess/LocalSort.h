#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

#include "PosSorting.h"
#include "ui_LocalSort.h"

class LocalSort : public QWidget
{
	Q_OBJECT
		enum POSTYPE { DAJ = 0, ZONGHENG };
public:
	LocalSort(QWidget *parent = Q_NULLPTR);
	~LocalSort();

signals:
	void signalStatus(QString mes);

private:
	void init();
	void connects();			//
	void slotOpenPos();			//选择pos文件
	void slotChangePosType(int index);	//设置pos类型
	void slotStartSort();		//开始排序
	void receivePosData(QMap<int, QList<PosInfo>> posData);	//接收排序好的pos数据
	bool isDataPrepared();		//数据是否准备好
	void receiveImageData(QMap<int, QList<ImageInfo>> imageData);
	void sortiesDisplay();		//展示架次信息

private:
	Ui::LocalSort ui;
	QStringList m_posFiles;
	int m_posType = DAJ;
	PosSorting *m_posSort = nullptr;
	const int m_camSize = 6;
	QMap<int, QList<PosInfo>> m_posData;
	QMap<int, QList<ImageInfo>> m_image1;
	QMap<int, QList<ImageInfo>> m_image2;
	QMap<int, QList<ImageInfo>> m_image3;
	QMap<int, QList<ImageInfo>> m_image4;
	QMap<int, QList<ImageInfo>> m_image5;
	QMap<int, QList<ImageInfo>> m_image6;
	QMap<int, bool> m_imageisReady;
};
