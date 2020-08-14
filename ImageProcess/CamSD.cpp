#include "CamSD.h"

CamSD::CamSD(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

CamSD::~CamSD()
{
}

void CamSD::setName(QString name, QString path)
{
	m_name = name;
	m_path = path;
	QString showName = m_baseName + QString("%1(%2)").arg(name).arg(path);
	ui.sd->setText(showName);
}

void CamSD::setAvailableAndCollective(float available, float collective)
{
	m_available = available;
	m_collective = collective;
	float value = (collective - available) / collective * 100;
	
	QString total = QString::number(collective, 'f', 1);
	QString avail = QString::number(available, 'f', 1);
	ui.progressBar->setValue(value);
	QString usage = QStringLiteral("%1GB可用,共%2GB").arg(avail).arg(total);
	ui.usage->setText(usage);
}

void CamSD::setOnlineStatus(bool online)
{
	m_online = online;
	QString showName;
	if (!m_online)
	{
		showName = m_baseName + QStringLiteral("%1(离线)").arg(m_name);
	}
	else
	{
		showName = m_baseName + QString("%1(%2)").arg(m_name).arg(m_path);
	}
	ui.sd->setText(showName);
}
