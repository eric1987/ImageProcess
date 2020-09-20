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

	//设置pos文件类型
	void setPosType(int type);

	//设置要读取的pos文件
	void setPosFiles(QStringList files);

	//设置m_disks
	void setDisks(QMap<QString, UDisk*> *disks);

	//将pos数据架次化
	void getPosSorted();

	//本地pos数据架次化
	void getLocPosSorted();

	//设置配置
	static void setConfig(int gap, int alt, int minImages);

signals:
	void signalPosData(QMap<int, QList<PosInfo>> );

private:
	//获取pos数据
	void getPosData();

	//对pos数据进行排序
	void sortingPosData();

	//读取pos数据
	void readPos(QString file, QList<PosInfo> &info);

	//解析pos数据函数
	void(*decodeData)(QString, PosInfo &);

	//判断影像是否分类完毕
	void isImageReady();

	//
	inline void initGap()
	{
		m_totalGap = 2;
		m_count = 1;
		//m_averageGap = 2.0;
	}

private:
	QList<PosInfo> m_posInfo;				///<pos数据
	QStringList	m_posFiles;					///<pos文件
	QMap<int, QList<PosInfo>> m_posInfoSorted;	//分架次后pos数据
	int m_totalGap = 2;						///<总间隔
	int m_count = 1;						///<间隔数

	static int m_fightGap;					//架次间隔
	static int m_altitudeIntercept;			//高度差
	static int m_minFightImages;			//架次最少影像数

	int m_posType = -1;						//pos文件存储类型
	QMap<QString, UDisk*> *m_disks;			//用于判断影像分类是否准备完毕
};
