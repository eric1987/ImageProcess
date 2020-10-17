#pragma once

#include <QWidget>
#include "ui_CamSD.h"

class CamSD : public QWidget
{
	Q_OBJECT

public:
	CamSD(QWidget *parent = Q_NULLPTR);
	~CamSD();

	//����sd������
	void setName(QString name, QString path);
	
	//����������param:available������������collective��������
	void setAvailableAndCollective(float available, float collective);

	//����sd��������״̬
	void setOnlineStatus(bool online);

private:
	Ui::CamSD ui;

	const QString m_baseName = QStringLiteral("Ӱ��SD-");	//��������
	QString m_name;		//Ӱ��sd����
	QString m_path;		//Ӱ��·����
	float m_collective = 0.0;	//���пռ�
	float m_available = 0.0;	//���ÿռ�
	bool m_online = true;		//�Ƿ�����
};
