#pragma once

#include <QWidget>
#include <QStandardPaths>
#include <windows.h>
#include <dbt.h>
#include <QStorageInfo>
#include <QMapIterator>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QThread>
#include <QMessageBox>
#include <QTextStream>

#include "ui_CamSort.h"
#include "CamSD.h"
#include "Common.h"
#include "Image.h"
#include "Log/Log.h"
#include "PosSorting.h"
#include "SortieWidget.h"
#include "Transfer.h"
#include "Util.h"
#include "CustomSortie.h"


class CamSort : public QWidget
{
	Q_OBJECT

		enum POSTYPE{DAJ = 0, ZONGHENG};

public:
	CamSort(QWidget *parent = Q_NULLPTR);
	~CamSort();
	
	//刷新sd卡信息
	void refreshSDInfo();
signals:
	void signalStatus(QString str);
	void signalSDInfo(QMap<QString, SDInfo> sdInfo);

private:
	//初始化
	void init();

	//连接
	void connects();

	//选择pos文件
	void selectPosFiles();

	//改变pos类型
	void changePosType(int index);

	//获取单个sd信息
	void getSDInfo(QString path);

	//获取sd卡的昵称与影像数量
	void getSDNicknameAndImageSize(SDInfo &sd, QString path);

	//影像文件架次分类
	void getSorties();

	//收到pos分架次信息，并准备显示架次概述
	void prepareShowSorties(QMap<int, QList<PosInfo>> posData);

	//显示架次概述: sorties, key:架次，value：影像数量
	void showSorties(QMap<int, int> sorties);

	//显示架次详情
	void showSortieDetail(int sortie);

	//架次选择变化
	void sortieSelectChanged(int state, int sortie);

	//架次传输
	void sortTransfer();

	//设置影像架次存储路径
	void setSavePath();
	void changeSavePath(QString path);

	//禁用操作
	void unableOperation();

	//启用操作
	void enableOperation();
	
	//获取影像exif信息
	void getImageExif();
 
	//刷新架次传输进度
	void refreshProcess(int value);

	//架次传输完成
	void transFinished();

	//修改pos并保存
	void rePosAndSave(QList<int> sorties);

	//命名转存pos文件名称
	QString setSaveReposFileName(QString cam, int sortie, QString path);

	//全选或全部选架次
	void setCheckSorties(int state);

private:
	Ui::CamSort ui;
	QVBoxLayout *m_sortLayout = nullptr;

	QMap<QString, CamSD*> m_sdList;	//sd卡list，key：id，用来展示sd卡
	QMap<QString, UDisk*> m_disks;	//sd卡信息，包括影像信息，用于架次分类
	QMap<QString, SDInfo> m_sdInfo;	//sd信息，包括sd在线状态，存储容量等
	QMap<int, QList<PosInfo>> m_posData;	//分架次后的pos数据

	int m_posType = DAJ;			//pos类型
	QString m_savePath;				//存储路径
	QStringList m_posFiles;			//pos文件
	QList<int> m_transferSortie;	//要传输的架次
	int m_camNum = 0;				//

	QMap<int, bool> m_sortieSelectStatus;	//架次选择状态
	PosSorting *m_posSort = nullptr;
	QMap<int, SortieWidget*> m_sortieWidgets;	//架次结果界面

	Transfer *m_transfer = nullptr;
};
