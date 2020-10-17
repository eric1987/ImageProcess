#pragma once

#include <QObject>
#include <QDir>
#include <QRegExp>
#include <QThread>

#include "Common.h"
#include "Log/Log.h"
#include "Util.h"

enum TRANSTYPE { SORTIE = 1, DIRECT, LOCAL };	//SORTIE：按架次传输， DIRECT：直接传输

class Transfer : public QThread
{
	Q_OBJECT

public:	
	Transfer(QThread *parent = Q_NULLPTR);
	~Transfer();

	//设置传输类型
	void setTransType(int type);

	//设置要传输的路径
	void setDstPath(QString path);

	//设置要传输的架次信息
	void setTransSortie(QList<int> num, QMap<QString, UDisk*> *disks);

	//设置要传输的sd卡
	void setSDCards(QStringList idList, QMap<QString, SDInfo> &sdInfo);
	
	//设置要传输的影像
	void setTransImages(QMap<QString, QMap<int, QList<ImageInfo>>> images);

	//设置本地要传输的架次
	void setLocalTransSortie(QList<int> num);

	//停止传输
	void stopTransfer();

	//开始传输
	void run();

signals:
	void signalProcess(int value);
	void signalSortieTransFailed();
	void signalSortieTransFinished();
	
	void signalDirectProcess(int value);
	void signalDirectTransFailed();
	void signalDirectTransFinished();

private:
	//架次传输方法
	void sortiesTransfer();

	//获取架次影像总数
	int getSortieTotalSize();

	//获取本地架次影像总数
	int getLocalTotalSize();

	//创建存储路径 sortie:架次序号，nickname:sd卡昵称
	QString createDir(int sortie, QString nickname);

	//单文件架次传输方法 srcFile：源文件 dstPath：传输目标路径  sortie：传输的架次 dstName：目标文件名称
	bool sortieTransferSingleFile(QString srcFile, QString dstPath, int sortie, QString dstName);

	//获取影像是在哪个文件夹下
	//QString getParentFloder(QString srcFile);

	//直接传输方法
	void directTransfer();

	//本地传输方法
	void localTransfer();

	//直接传输单个文件方法
	bool directTransferSingleFile(QString path, QString srcFile, QString dstPath);

	//创建存储路径： nickname：sd卡昵称
	QString creatDirectDir(QString nickname);

	//获取直接传输影像总数
	int getDirectTotalSize();

	

private:
	int m_transType;		//传输类型
	QString m_dstPath;					//传输目的路径
	QList<int> m_sorties;				//传输的架次
	QStringList m_idList;				//直接传输的sd卡id
	QMap<QString, SDInfo> m_sdInfo;		//sd卡信息，包括sd卡路径
	QMap<QString, UDisk*> *m_disks = nullptr;		//sd卡信息，保存有分架次的影像信息
	int m_transferCount = 0;		//已传输数量
	int m_transferSize = 0;			//传输记录
	int m_sortieTotal = 0;		//架次传输的影像总数
	int m_sortieStep = 1;		//架次传输进度更新单步大小

	//直接传输变量
	int m_directTransCount = 0;		//直接传输已传输数量
	int m_directTransSize = 0;		//直接传输传输记录
	int m_directTotal = 0;			//直接传输要传输的影像总数
	int m_directStep = 1;			//直接传输进度更新的单步大小

	bool m_stop = false;		//停止符
	QMap<QString, QMap<int, QList<ImageInfo>>> m_logcalImages; //本地影像数据
};
