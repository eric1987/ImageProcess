#include "PosSorting.h"

int PosSorting::m_fightGap = 200;
int PosSorting::m_altitudeIntercept = 100;
int PosSorting::m_minFightImages = 30;

void JouAVDecode(QString str, PosInfo &pd)
{
	QStringList line = str.split('\t');
	line.removeAll("");

	int i = 0;
	QStringList strTime;
	Q_FOREACH(QString var, line)
	{
		switch (i++)
		{
		case 0:
			pd.index = var.toInt();
			break;
		case 1:
			strTime = var.split('T');
			if (strTime.size() > 2)
			{
				QDate date = QDate::fromString(strTime[0], "yyyyMMdd");
				QString strTimeTemp = strTime[1].remove(" ");
				QTime t = QTime::fromString(strTimeTemp, "hh:mm:ss");

				pd.timestamp = QDateTime(date, t).toSecsSinceEpoch();
				pd.time = strTime[1];
			}
			break;
		case 2:
			pd.latitude = var.toFloat();
			break;
		case 3:
			pd.longitude = var.toFloat();
			break;
		case 4:
			pd.altitude = var.toFloat();
			break;
		default:
			break;
		}
	}
}

void DJIDecode(QString str, PosInfo &pd)
{
	QStringList line = str.split('\t');
	line.removeAll("");

	int i = 0;
	Q_FOREACH(QString var, line)
	{
		switch (i++)
		{
		case 0:
			pd.date = var;
			break;
		case 1:
			pd.time = var.rightJustified(6, '0');
			pd.timestamp = QDateTime(QDate::fromString(pd.date, "yyyyMMdd"), 
				QTime::fromString(pd.time, "hhmmss")).toSecsSinceEpoch();
			pd.time.insert(4, ":");
			pd.time.insert(2, ":");
			break;
		case 2:
			pd.index = var.toInt();
			break;
		case 3:
			pd.latitude = var.toFloat();
			break;
		case 4:
			pd.longitude = var.toFloat();
			break;
		case 5:
			pd.altitude = var.toFloat();
			break;
		default:
			break;
		}
	}
}

PosSorting::PosSorting(QObject *parent)
	: QObject(parent)
{
}

PosSorting::~PosSorting()
{
}

void PosSorting::setPosType(int type)
{
	m_posType = type;

	switch (type)
	{
	case 0:
		decodeData = DJIDecode;
		break;
	case 1:
		decodeData = JouAVDecode;
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

void PosSorting::setPosFiles(QStringList files)
{
	m_posFiles = files;
}

void PosSorting::setDisks(QMap<QString, UDisk*> *disks)
{
	m_disks = disks;
}

void PosSorting::getPosSorted()
{
	getPosData();
	sortingPosData();
	isImageReady();
	emit signalPosData(m_posInfoSorted);
}

void PosSorting::setConfig(int gap, int alt, int minImages)
{
	m_fightGap = gap;
	m_altitudeIntercept = alt;
	m_minFightImages = minImages;
}

void PosSorting::getPosData()
{
	m_posInfo.clear();

	for each (QString var in m_posFiles)
	{
		readPos(var, m_posInfo);
	}
}

void PosSorting::sortingPosData()
{
	m_posInfoSorted.clear();
	if (m_posInfo.isEmpty())
	{
		return;
	}

	PosInfo lastpos = m_posInfo[0];
	QList<PosInfo> fightData;
	fightData.append(lastpos);

	int sortie = 0;
	for (int i = 1; i < m_posInfo.size(); ++i)
	{
		//�������ݸ߶ȴ��ھ����ݸ߶�100���ϣ�˵��������Ϊ��ʽ��������
		//�򶪵�֮ǰ������ͳ�ƣ����¿�ʼͳ��
#if 0
		if (m_posInfo[i].altitude - lastpos.altitude > m_minAltitudeVariation)
		{
			lastpos = m_posInfo[i];
			initGap();
		}

		m_totalGap += m_posInfo[i].timestamp - lastpos.timestamp;
		++m_count;
#endif

		//�����ܴ�
		if (fabs(m_posInfo[i].timestamp - lastpos.timestamp) > m_fightGap || fabs(m_posInfo[i].altitude - lastpos.altitude) > m_altitudeIntercept)
		{
			//�жϵ��ܴ�����������С�����ſɴ���ܴ�������
			if (fightData.size() > m_minFightImages)
			{
				m_posInfoSorted.insert(sortie++, fightData);
			}
			fightData.clear();
			//initGap();
		}

		fightData.append(m_posInfo[i]);
		lastpos = m_posInfo[i];
	}

	//�����ļܴ����ݼ��뵽��������
	if (fightData.size() > m_minFightImages)
	{
		m_posInfoSorted.insert(sortie++, fightData);
	}
}

void PosSorting::readPos(QString file, QList<PosInfo> &info)
{
	QFile data(file);
	if (!data.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		Log::INFO(QStringLiteral("��pos�ļ�ʧ�ܡ�"));
	}
	bool firstLine = true;

	while (!data.atEnd())
	{
		QByteArray line = data.readLine();
		QString str(line);
		if (m_posType == 0 && firstLine)
		{
			firstLine = false;
			continue;
		}
		PosInfo pd;
		decodeData(str, pd);
		info.append(pd);
	}

	data.close();
}

void PosSorting::isImageReady()
{
	QList<UDisk *> values = m_disks->values();

	while (true)
	{
		bool nloop = true;
		QThread::msleep(200);
		for (int i = 0; i < values.size(); i++)
		{
			UDisk *disk = values.at(i);
			if (disk->sortieStatus)
			{
				nloop &= true;
			}
			else
			{
				nloop &= false;
			}
		}
		if (nloop)
		{
			break;
		}
		else
		{
			continue;
		}
	}
}
