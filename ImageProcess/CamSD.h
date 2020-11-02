#pragma once

#include <QWidget>
#include "ui_CamSD.h"

class CamSD : public QWidget
{
	Q_OBJECT

public:
	CamSD(QWidget *parent = Q_NULLPTR);
	~CamSD();

	//设置sd卡名字
	//name：名称，path：路径
	void setName(QString name, QString path);
	
	//设置容量，param:available，可用容量；collective，总容量
	void setAvailableAndCollective(float available, float collective);

	//设置sd卡的在线状态， online：true在线，false不在线
	void setOnlineStatus(bool online);

private:
	Ui::CamSD ui;

	const QString m_baseName = QStringLiteral("影像SD-");	//基础名称
	QString m_name;		//影像sd卡名
	QString m_path;		//影像路径名
	float m_collective = 0.0;	//共有空间
	float m_available = 0.0;	//可用空间
	bool m_online = true;		//是否在线
};
