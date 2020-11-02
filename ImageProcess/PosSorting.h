#pragma once

#include <QObject>
#include <QFile>
#include <QThread>

#include "Common.h"
#include "Log/Log.h"

class PosSorting : public QObject
{
	Q_OBJECT

public:
	PosSorting(QObject *parent = Q_NULLPTR);
	~PosSorting();

	//����pos�ļ�����
	//type��0�󽮣�1�ݺ�
	void setPosType(int type);

	//����Ҫ��ȡ��pos�ļ�
	//files�ļ���
	void setPosFiles(QStringList files);

	//����m_disks
	void setDisks(QMap<QString, UDisk*> *disks);

	//��pos���ݼܴη���
	void getPosSorted();

	//����pos���ݼܴη���
	void getLocPosSorted();

	//��������
	//gap��Ӱ��ʱ������alt���߶Ȳ minImages:�ܴ�����Ӱ����
	static void setConfig(int gap, int alt, int minImages);

signals:
	void signalPosData(QMap<int, QList<PosInfo>> );

private:
	//��ȡpos����
	void getPosData();

	//��pos���ݽ�������
	void sortingPosData();

	//��ȡpos����
	//file��pos�ļ����� info���洢pos��Ϣ
	void readPos(QString file, QList<PosInfo> &info);

	//����pos���ݺ���
	void(*decodeData)(QString, PosInfo &);

	//�ж�Ӱ���Ƿ�������
	void isImageReady();

	//
	inline void initGap()
	{
		m_totalGap = 2;
		m_count = 1;
		//m_averageGap = 2.0;
	}

private:
	QList<PosInfo> m_posInfo;				///<pos����
	QStringList	m_posFiles;					///<pos�ļ�
	QMap<int, QList<PosInfo>> m_posInfoSorted;	//�ּܴκ�pos����
	int m_totalGap = 2;						///<�ܼ��
	int m_count = 1;						///<�����

	static int m_fightGap;					//�ܴμ��
	static int m_altitudeIntercept;			//�߶Ȳ�
	static int m_minFightImages;			//�ܴ�����Ӱ����

	int m_posType = -1;						//pos�ļ��洢����
	QMap<QString, UDisk*> *m_disks;			//�����ж�Ӱ������Ƿ�׼�����
};
