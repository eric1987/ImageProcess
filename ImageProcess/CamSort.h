#pragma once

#include <QWidget>
#include <QStandardPaths>
#include <windows.h>
#include <dbt.h>
#include <QStorageInfo>

#include "ui_CamSort.h"
#include "CamSD.h"
#include "Common.h"
#include "Log/Log.h"

class CamSort : public QWidget
{
	Q_OBJECT

		enum POSTYPE{DAJ = 0, ZONGHENG};

public:
	CamSort(QWidget *parent = Q_NULLPTR);
	~CamSort();

	//������sd��Ϣ������
	void addCamSD(QString id, QString name, float available, float collective);


private:
	//��ʼ��
	void init();

	//����
	void connects();

	//ѡ��pos�ļ�
	void selectPosFiles();

	//�ı�pos����
	void changePosType(int index);

	//ˢ��sd����Ϣ
	void refreshSDInfo();

	//��ȡ����sd��Ϣ
	void getSDInfo(QString path);

	//�ܴη���
	void sortImage();

	//�ܴδ���
	void sortTransfer();

	//����Ӱ��ܴδ洢·��
	void setSavePath();
	void changeSavePath(QString path);

	//���ò���
	void unableOperation();

	//���ò���
	void enableOperation();
	
	//��ȡӰ��exif��Ϣ
	void getImageExif();
 
private:
	Ui::CamSort ui;

	QMap<QString, CamSD> m_sdList;	//sd��list��key��id������չʾsd��
	QMap<QString, UDisk*> m_disks;	//sd����Ϣ������Ӱ����Ϣ�����ڼܴη���
	int m_posType = DAJ;			//pos����
	QString m_savePath;				//�洢·��

	QStringList m_posFiles;			//pos�ļ�
	bool m_sdChanged;				//sd�仯

	QList<int> m_transferSortie;	//Ҫ����ļܴ�
};
