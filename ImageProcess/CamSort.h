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

	//添加相机sd信息到界面
	void addCamSD(QString id, QString name, float available, float collective);


private:
	//初始化
	void init();

	//连接
	void connects();

	//选择pos文件
	void selectPosFiles();

	//改变pos类型
	void changePosType(int index);

	//刷新sd卡信息
	void refreshSDInfo();

	//获取单个sd信息
	void getSDInfo(QString path);

	//架次分类
	void sortImage();

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
 
private:
	Ui::CamSort ui;

	QMap<QString, CamSD> m_sdList;	//sd卡list，key：id，用来展示sd卡
	QMap<QString, UDisk*> m_disks;	//sd卡信息，包括影像信息，用于架次分类
	int m_posType = DAJ;			//pos类型
	QString m_savePath;				//存储路径

	QStringList m_posFiles;			//pos文件
	bool m_sdChanged;				//sd变化

	QList<int> m_transferSortie;	//要传输的架次
};
