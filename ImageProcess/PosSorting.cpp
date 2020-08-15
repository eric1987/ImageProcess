#include "PosSorting.h"

void JouAVDecode(QString str, PosInfo &pd)
{
	QStringList datas = str.split('\t');

	pd.index = datas[0].toInt();
	QStringList strTime = datas[1].split('T');
	QDate date = QDate::fromString(strTime[0], "yyyyMMdd");
	QString strTimeTemp = strTime[1].remove(" ");
	QTime t = QTime::fromString(strTimeTemp, "hh:mm:ss");

	pd.timestamp = QDateTime(date, t).toSecsSinceEpoch();
	pd.time = strTime[1];
	pd.latitude = datas[2].toFloat();
	pd.longitude = datas[3].toFloat();
	pd.altitude = datas[4].toFloat();
	pd.pitching = datas[5].toFloat();
	pd.rolling = datas[6].toFloat();
	pd.driftAngle = datas[7].toFloat();
	pd.heading = datas[8].toFloat();
	pd.groundSpeed = datas[9].toFloat();
}

void DJIDecode(QString str, PosInfo &pd)
{
	QStringList datas = str.split('\t');

	QDate date = QDate::fromString(datas[0], "yyyyMMdd");
	QTime t = QTime::fromString(datas[1], "hhmmss");
	pd.timestamp = QDateTime(date, t).toSecsSinceEpoch();
	QString time = datas[1];
	time.insert(4, ":");
	time.insert(2, ":");
	pd.time = time;
	pd.latitude = datas[3].toFloat();
	pd.longitude = datas[4].toFloat();
	pd.altitude = datas[5].toFloat();
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
	m_posInfo;
	sortingPosData();
	isImageReady();
	emit signalPosData(m_posInfoSorted);
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
	QList<PosInfo> fightViewData;
	int sortie = 0;
	for (int i = 1; i < m_posInfo.size(); ++i)
	{
		//�������ݸ߶ȴ��ھ����ݸ߶�100���ϣ�˵��������Ϊ��ʽ��������
		//�򶪵�֮ǰ������ͳ�ƣ����¿�ʼͳ��
		if (m_posInfo[i].altitude - lastpos.altitude > m_minAltitudeVariation)
		{
			lastpos = m_posInfo[i];
			initGap();
		}

		m_totalGap += m_posInfo[i].timestamp - lastpos.timestamp;
		++m_count;

		//�����ܴ�
		if (m_posInfo[i].timestamp - lastpos.timestamp > m_averageGap * m_sortieTime)
		{
			//�жϵ��ܴ�����������С�����ſɴ���ܴ�������
			if (fightViewData.size() > m_fightImageMinNum)
			{
				m_posInfoSorted.insert(sortie++, fightViewData);
			}
			fightViewData.clear();
			initGap();
		}


		fightViewData.append(m_posInfo[i]);
		lastpos = m_posInfo[i];
	}

	//�����ļܴ����ݼ��뵽��������
	if (fightViewData.size() > m_fightImageMinNum)
	{
		m_posInfoSorted.insert(sortie++, fightViewData);
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
		if (m_posType == 1 && firstLine)
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
