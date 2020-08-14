#include "CamSD.h"

CamSD::CamSD(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

CamSD::CamSD(QString name, float available, float collective, QWidget *parent)
	: QWidget(parent)
{
	setName(name);
	setAvailableAndCollective(available, collective);
}

CamSD::~CamSD()
{
}

void CamSD::setName(QString name)
{
	m_name = name;
	QString showName = m_baseName + QString("(%1)").arg(name);
	ui.sd->setText(showName);
}

void CamSD::setAvailableAndCollective(float available, float collective)
{
	m_available = available;
	m_collective = collective;
	float value = available / collective;
	
	ui.progressBar->setValue(value);
	QString usage = QStringLiteral("%1GB可用,共%2GB").arg(available).arg(collective);
	ui.usage->setText(usage);
}

void CamSD::setSDStatus(bool online)
{
	m_online = online;
	QString showName;
	if (!m_online)
	{
		showName = m_baseName + QString("(%1 离线)").arg(m_name);
	}
	else
	{
		showName = m_baseName + QString("(%1)").arg(m_name);
	}
	ui.sd->setText(showName);
}
