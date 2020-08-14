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
	void setName(QString name, QString path);
	
	//设置容量，param:available，可用容量；collective，总容量
	void setAvailableAndCollective(float available, float collective);

	//设置sd卡的在线状态
	void setOnlineStatus(bool online);

private:
	Ui::CamSD ui;

	const QString m_baseName = QStringLiteral("影像SD-");
	QString m_name;
	QString m_path;
	float m_collective = 0.0;
	float m_available = 0.0;
	bool m_online = true;
};
