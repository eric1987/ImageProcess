#include "Transfer.h"

Transfer::Transfer(QThread *parent)
	: QThread(parent)
{
}

Transfer::~Transfer()
{
}

void Transfer::setTransType(int type)
{
	m_transType = type;
}

void Transfer::setDstPath(QString path)
{
	m_dstPath = path;
}

void Transfer::setTransSortie(QList<int> num, QMap<QString, UDisk*> *disks)
{
	m_sorties = num;
	m_disks = disks;
}

void Transfer::setSDCards(QStringList idList, QMap<QString, SDInfo> &sdInfo)
{
	m_idList = idList;
	m_sdInfo = sdInfo;
}

void Transfer::setTransImages(QMap<QString, QMap<int, QList<ImageInfo>>> images)
{
	m_logcalImages = images;
}

void Transfer::setLocalTransSortie(QList<int> num)
{
	m_sorties = num;
}

void Transfer::stopTransfer()
{
	m_stop = true;
}

void Transfer::run()
{
	m_stop = false;
	switch (m_transType)
	{
	case SORTIE:
		sortiesTransfer();
		break;
	case DIRECT:
		directTransfer();
		break;
	case LOCAL:
		localTransfer();
	default:
		break;
	}
}

void Transfer::sortiesTransfer()
{
	m_sortieTotal = getSortieTotalSize();
	m_transferSize = 0;
	if (m_sortieTotal * 0.01 > 1)
	{
		m_sortieStep = m_sortieTotal * 0.01;
	}
	else
	{
		m_sortieStep = 1;
	}

	QMapIterator<QString, UDisk*> iter(*m_disks);
	while (iter.hasNext())
	{
		iter.next();
		Q_FOREACH(int sort, m_sorties)
		{
			if (iter.value()->imageData.contains(sort))
			{
				if (iter.value()->transImage[sort])
				{
					continue;
				}
				QString dst = createDir(sort, iter.value()->cam);
				Q_FOREACH(ImageInfo image, iter.value()->imageData[sort])
				{
					if (m_stop)
					{
						Log::INFO(QStringLiteral("ֹͣ�ܴδ��䡣"));
						return;
					}
					if (image.transfer)
					{
						continue;
					}
					if (!sortieTransferSingleFile(image.fileName, dst, sort, image.imageName))
					{
						Log::INFO(QStringLiteral("�ܴδ���ʧ�ܣ������%1���ļ���%2").arg(iter.value()->cam).arg(image.fileName));
						emit signalSortieTransFailed();
						return;
					}
					else
					{
						image.transfer = true;
					}
				}
				iter.value()->transImage[sort] = true;
			}
		}
		iter.value()->sortieTransStatus = true;
	}

	emit signalSortieTransFinished();
}

int Transfer::getSortieTotalSize()
{
	int total = 0;
	QMapIterator<QString, UDisk*> iter(*m_disks);
	while (iter.hasNext())
	{
		iter.next();
		Q_FOREACH(int sort, m_sorties)
		{
			if (iter.value()->imageData.contains(sort))
			{
				total += iter.value()->imageData[sort].size();
			}
		}
	}
	return total;
}

int Transfer::getLocalTotalSize()
{
	int total = 0;
	QMapIterator<QString, QMap<int, QList<ImageInfo>>> iter(m_logcalImages);
	while (iter.hasNext())
	{
		iter.next();
		Q_FOREACH(int sort, m_sorties)
		{
			if (iter.value().contains(sort))
			{
				total += iter.value()[sort].size();
			}
		}
	}
	return total;
}

QString Transfer::createDir(int sortie, QString nickname)
{
	QString path = QString("%1/%2").arg(sortie+1, 2, 10, QLatin1Char('0')).arg(nickname);
	QString dst = m_dstPath + "/" + path;
	QDir dir(dst);
	if (!dir.exists())
	{
		dir.mkpath(dst);
	}
	return dst;
}

bool Transfer::sortieTransferSingleFile(QString srcFile, QString dstPath, int sortie, QString dstName)
{
	QString insert = QString("%1%2").arg(Util::getParentFloder(srcFile)).arg(sortie, 2, 10, QLatin1Char('0'));
	QString fileName = dstName.insert(2, insert);
	QString absoluteFile = dstPath + "/" + fileName;

	QFile file(srcFile);
	if (file.exists())
	{
		bool ret = file.copy(srcFile, absoluteFile);
		if (m_transferCount++ >= m_sortieStep)
		{
			m_transferCount = 0;	
			emit signalProcess(m_transferSize++);
		}
		return ret;
	}
	else
	{
		return false;	//����ʧ��
	}
	
}

#if 0
QString Transfer::getParentFloder(QString srcFile)
{
	QString pattern = "\\d{3}MSDCF";
	QRegExp reg;
	reg.setPattern(pattern);
	reg.indexIn(srcFile);
	QString folder = reg.cap(0);

	return folder.at(2);
}
#endif

void Transfer::directTransfer()
{
	m_directTotal = getDirectTotalSize();
	if (m_directTotal * 0.01 > 1)
	{
		m_directStep = m_directTotal * 0.01;
	}
	else
	{
		m_directStep = 1;
	}

	Q_FOREACH(QString id, m_idList)
	{
		if (m_sdInfo.contains(id))
		{
			SDInfo info = m_sdInfo[id];
			QString dst = creatDirectDir(info.nickname);
			
			QDir dir(info.path + "DCIM");
			QStringList imageDirs = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
			
			Q_FOREACH(QString imageDir, imageDirs)
			{
				if (!info.directTransStatus.contains(imageDir))
				{
					info.directTransStatus[imageDir] = false;
				}
				else
				{
					if (info.directTransStatus[imageDir])
					{
						continue;
					}
				}

				QString srcPath = dir.absolutePath() + "/" + imageDir;
				QDir fileDir(srcPath);
				QStringList filters;
				filters << "*.jpg" << "*.png" << "*.arw" << "*.cr2";
				fileDir.setNameFilters(filters);
				QStringList files = fileDir.entryList();
				Q_FOREACH(QString file, files)
				{
					if (m_stop)
					{
						Log::INFO(QStringLiteral("ֹͣ�ܴδ��䡣"));
						return;
					}

					QString srcFile = fileDir.absolutePath() + "/" + file;
					QString dstPath = m_dstPath + "/" + info.nickname;
					if (!directTransferSingleFile(fileDir.absolutePath(), file, dstPath))
					{
						Log::INFO(QStringLiteral("ֱ���������ʧ�ܣ��ļ���%1").arg(srcFile));
						emit signalDirectTransFailed();
						return;
					}
				}
			}
		}
	}

	emit signalDirectTransFinished();
}

void Transfer::localTransfer()
{
	m_sortieTotal = getLocalTotalSize();
	m_transferSize = 0;
	if (m_sortieTotal * 0.01 > 1)
	{
		m_sortieStep = m_sortieTotal * 0.01;
	}
	else
	{
		m_sortieStep = 1;
	}

	QMapIterator<QString, QMap<int, QList<ImageInfo>>> iter(m_logcalImages);
	while (iter.hasNext())
	{
		iter.next();
		QString camName = iter.key();
		Q_FOREACH(int sort, m_sorties)
		{
			QString dst = createDir(sort, camName);
			Q_FOREACH(ImageInfo image, iter.value()[sort])
			{
				if (m_stop)
				{
					Log::INFO(QStringLiteral("ֹͣ�ܴδ��䡣"));
					return;
				}
				if (image.transfer)
				{
					continue;
				}
				if (!sortieTransferSingleFile(image.fileName, dst, sort, image.imageName))
				{
					Log::INFO(QStringLiteral("�ܴδ���ʧ�ܣ������%1���ļ���%2").arg(camName).arg(image.fileName));
					emit signalSortieTransFailed();
					return;
				}
				else
				{
					image.transfer = true;
				}
			}
		}
	}
	emit signalSortieTransFinished();
}

bool Transfer::directTransferSingleFile(QString path, QString srcFile, QString dstPath)
{
	QString insertStr = Util::getParentFloder(path) + QString("00");
	QString temp = srcFile;
	QString finalName = temp.insert(2, insertStr);
	QFile imageFile(path + "/" + srcFile);
	QString dstFile = dstPath + "/" + finalName;
	if (imageFile.exists())
	{
		bool ret = imageFile.copy(dstFile);
		if (m_directTransCount++ >= m_sortieStep)
		{
			m_directTransSize++;
			m_directTransCount = 0;

			emit signalDirectProcess(m_directTransSize / m_directStep);
		}
		return ret;
	}
	else
	{
		return false;
	}
}

QString Transfer::creatDirectDir(QString nickname)
{
	QString dst = m_dstPath + "/" + nickname;
	QDir dir(dst);
	if (!dir.exists())
	{
		dir.mkpath(dst);
	}
	return dst;
}

int Transfer::getDirectTotalSize()
{
	int total = 0;
	Q_FOREACH(QString id, m_idList)
	{
		if (m_sdInfo.contains(id))
		{
			total += m_sdInfo[id].imageSize;
		}
	}

	return total;
}
