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
	void slotOpenPos();			//ѡ��pos�ļ�
	void slotChangePosType(int index);	//����pos����
	void slotStartSort();		//��ʼ����
	void receivePosData(QMap<int, QList<PosInfo>> posData);	//��������õ�pos����
	bool isDataPrepared();		//�����Ƿ�׼����
	void receiveImageData(QMap<int, QList<ImageInfo>> imageData);
	void sortiesDisplay();		//չʾ�ܴ���Ϣ

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
