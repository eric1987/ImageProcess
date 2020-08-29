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
	void setPosType(int type);

	//����Ҫ��ȡ��pos�ļ�
	void setPosFiles(QStringList files);

	//����m_disks
	void setDisks(QMap<QString, UDisk*> *disks);

	//��pos���ݼܴλ�
	void getPosSorted();

signals:
	void signalPosData(QMap<int, QList<PosInfo>> );

private:
	//��ȡpos����
	void getPosData();

	//��pos���ݽ�������
	void sortingPosData();

	//��ȡpos����
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
		m_averageGap = 2.0;
	}

private:
	QList<PosInfo> m_posInfo;				///<pos����
	QStringList	m_posFiles;					///<pos�ļ�
	QMap<int, QList<PosInfo>> m_posInfoSorted;	//�ּܴκ�pos����
	const int m_minAltitudeVariation = 100;	///<��С�߶Ȳ�
	int m_totalGap = 2;						///<�ܼ��
	int m_count = 1;						///<�����
	float m_averageGap = 2.0;				///<ƽ�����
	const int m_fightGap = 200;				//�ܴμ��
	const int m_altitudeIntercept = 100;	//�߶Ȳ�
	const int m_sortieTime = 100;			///<�ܴ�ʱ�����������Ƭ����ı���
	const int m_fightImageMinNum = 30;		///<�ܴ�����������Ƭ����

	int m_posType = -1;						//pos�ļ��洢����
	QMap<QString, UDisk*> *m_disks;			//�����ж�Ӱ������Ƿ�׼�����
};
