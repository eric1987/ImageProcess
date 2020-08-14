#pragma once

#include <QWidget>
#include "ui_CamSD.h"

class CamSD : public QWidget
{
	Q_OBJECT

public:
	CamSD(QWidget *parent = Q_NULLPTR);
	CamSD(QString name, float available, float collective, QWidget *parent = Q_NULLPTR);
	~CamSD();

	//����sd������
	void setName(QString name);
	
	//����������param:available������������collective��������
	void setAvailableAndCollective(float available, float collective);

	//����sd��������״̬
	void setSDStatus(bool online);

private:
	Ui::CamSD ui;

	const QString m_baseName = QStringLiteral("Ӱ��SD-");
	QString m_name;
	float m_collective = 0.0;
	float m_available = 0.0;
	bool m_online = true;
};
