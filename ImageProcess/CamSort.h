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

#include "ui_CamSort.h"
#include "CamSD.h"
#include "Common.h"
#include "Log/Log.h"

//TODO: 架次显示，架次转存
//TODO：直接转存，格式化
//TODO: 生成excel文件

class CustomSortie : public QWidget
{
	Q_OBJECT
public:
	CustomSortie(int sortie, int size, QWidget *parent = Q_NULLPTR)
		: QWidget(parent)
	{
		m_sortie = sortie;
		box = new QCheckBox(QStringLiteral("架次%1  影像%2张").arg(sortie).arg(size));
		btn = new QPushButton(QStringLiteral("查看详情"));
		btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		btn->setBackgroundRole(QPalette::Highlight);

		layout = new QHBoxLayout(this);
		this->setLayout(layout);
		layout->addWidget(box);
		layout->addWidget(btn);

		connect(box, &QCheckBox::stateChanged, this, &CustomSortie::checkChanged);
		connect(btn, &QPushButton::clicked, this, &CustomSortie::showSortie);
	}
	~CustomSortie(){}

signals:
	void signalCheckChanged(int state, int sortie);
	void signalSortieDetail(int sortie);

private:
	void checkChanged(int state)
	{
		emit signalCheckChanged(state, m_sortie);
	}

	void showSortie()
	{
		emit signalSortieDetail(m_sortie);
	}

	int m_sortie = -1;
	QCheckBox *box = nullptr;
	QPushButton *btn = nullptr;
	QHBoxLayout *layout = nullptr;
};

class CamSort : public QWidget
{
	Q_OBJECT

		enum POSTYPE{DAJ = 0, ZONGHENG};

public:
	CamSort(QWidget *parent = Q_NULLPTR);
	~CamSort();

	//添加相机sd信息到界面
	void addCamSD(QString id, QString name, float available, float collective);

signals:
	void signalStatus(QString str);

private:
	void test();

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

	//获取sd卡的昵称与影像数量
	void getSDNicknameAndImageSize(SDInfo &sd, QString path);

	//架次分类
	void sortImage();

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
 
private:
	Ui::CamSort ui;
	QVBoxLayout *m_sortLayout = nullptr;

	QMap<QString, CamSD*> m_sdList;	//sd卡list，key：id，用来展示sd卡
	QMap<QString, UDisk*> m_disks;	//sd卡信息，包括影像信息，用于架次分类
	QMap<QString, SDInfo> m_sdInfo;	//sd信息，包括sd在线状态，存储容量等

	int m_posType = DAJ;			//pos类型
	QString m_savePath;				//存储路径
	QStringList m_posFiles;			//pos文件
	QList<int> m_transferSortie;	//要传输的架次
	int m_camNum = 0;				//

	QMap<int, bool> m_sortieSelectStatus;	//架次选择状态
};
