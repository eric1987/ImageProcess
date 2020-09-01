#include "Image.h"

int Image::m_fightGap = 200;
int Image::m_sizeDiff = 5000000;
int Image::m_minFightImages = 30;

//����һ��ExifKey����û�е�Key�����δ���
std::string FindExifKey(Exiv2::ExifData &ed, std::string key) {
	Exiv2::ExifKey tmp = Exiv2::ExifKey(key);
	Exiv2::ExifData::iterator pos = ed.findKey(tmp);
	if (pos == ed.end()) {
		return "Unknow";
	}

	return pos->value().toString();
}

Image::Image(QObject *parent)
	: QObject(parent)
{
	QString pattern = "\\d{5}";
	m_reg = new QRegExp;
	m_reg->setPattern(pattern);
}

Image::~Image()
{
}

void Image::setPath(QString path)
{
	m_path = path;
}

void Image::setUDiskInfo(UDisk *udisk)
{
	m_disk = udisk;
}

bool lessThan(const QString &s1, const QString &s2)
{
	if (s1.size() < 3 && s2.size() < 3)
	{
		Log::INFO(QStringLiteral("�Ƿ����ļ���·�����ļ���%1�� %2 ").arg(s1).arg(s2));
	}
	return s1 < s2;
}

void Image::readInfo()
{
	QDir dir(m_disk->path);
	QStringList dirList = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);

	qSort(dirList.begin(), dirList.end(), lessThan);

	for each (QString subDir in dirList)
	{
		if (subDir.contains("MSDCF"))
		{
			QString dirPath = dir.absolutePath();
			QDir dirMSDCF(dirPath + "/" + subDir);

			QStringList fileList = dirMSDCF.entryList(QDir::Files);
			QString subPath = dirMSDCF.absolutePath();
			if (m_disk->exifStatus.value(subPath) == true)
			{
				continue;
			}

			qSort(fileList.begin(), fileList.end(), lessThan);
			for each (QString var in fileList)
			{
				ImageInfo info;
				int ret = readSingleInfo((subPath + "/" + var).toStdString(), info);
				if (ret < 0)
				{
					Log::INFO(QStringLiteral("��ȡͼƬ��Ϣ����."));
					return;
				}
				//m_exifImages.append(info);
				sortie(info);
			}

			m_disk->exifStatus[subPath] = true;
		}
	}

	if (m_sortieImageSize < m_minFightImages)
	{
		for each (ImageInfo var in m_fightData)
		{
			var.type = 0;
		}
	}
	else
	{
		m_disk->imageData.insert(m_sortie++, m_fightData);
	}
	m_fightData.clear();
	m_sortieImageSize = 0;
	m_sortie = 0;

	m_disk->sortieStatus = true;
}

void Image::readAndSortLocalImage()
{
	getImageInfo();
	sortieLocal();
}

void Image::setImages(QStringList images)
{
	m_images = images;
}

void Image::setConfig(int gap, int sizeDiff, int minImages)
{
	m_fightGap = gap;
	m_sizeDiff = sizeDiff;
	m_minFightImages = minImages;
}

void Image::nameContinuous(QString &name)
{
	int pos = m_reg->indexIn(name);
	if (pos > -1)
	{
		QString str = m_reg->cap(0);
		int index = str.toInt();
		if (fabs(index - m_lastIndex) != 1 && m_lastIndex != -1)
		{
			index = m_lastIndex + 1;
			QString numb = QString("%1").arg(index, 5, 10, QChar('0'));
			name.replace(str, numb);
		}
		m_lastIndex = index;
	}
}

void Image::sortie(ImageInfo info)
{
	//�����ܴ�
	if (fabs(info.timestamp - m_info.timestamp) > m_fightGap || 
		fabs(info.size - m_info.size) > m_sizeDiff)
	{
		Log::INFO(QStringLiteral("ʱ������ %1�� ��Ƭ��С� %2").arg(m_fightGap).arg(m_sizeDiff));

		//�жϼܴ��е�image����������50��Ϊ���������㡣
		if (m_sortieImageSize < m_minFightImages)
		{
			/*for (int i = 0; i < m_fightData.size(); i++)
			{
				m_fightData[i].type = 0;
			}*/
			Log::INFO(QStringLiteral("�л��ܴ�,��ʼӰ��Ϊ��%1,֮ǰ��Ӱ������̫�٣���������").arg(info.fileName));
			m_fightData.clear();
		}
		else
		{
			m_disk->imageData.insert(m_sortie++, m_fightData);
			m_fightData.clear();
			m_sortieImageSize = 0;
			Log::INFO(QStringLiteral("�л��ܴ�,��ʼӰ��Ϊ��%1����һ�ܴεĽ�ֹΪ��%2").arg(info.fileName).arg(m_info.fileName));
		}
	}

	info.type = 1;
	m_sortieImageSize++;
	m_fightData.append(info);
	m_info = info;
}

void Image::getImageInfo()
{
	Q_FOREACH(QString file, m_images)
	{
		ImageInfo info;
		int ret = readSingleInfo(file.toStdString(), info);
		if (ret < 0)
		{
			Log::INFO(QStringLiteral("��ȡͼƬ��Ϣ����."));
			return;
		}
		m_infos.append(info);
		m_info = info;
	}
}

void Image::sortieLocal()
{
	Q_FOREACH(ImageInfo info, m_infos)
	{

	}

	//if (fabs(info.timestamp - m_info.timestamp) > m_averageGap * m_sortieTime)
	//{
	//	//�жϼܴ��е�image����������50��Ϊ���������㡣
	//	if (m_sortieImageSize < m_fightImageMinNum)
	//	{
	//		for each (ImageInfo var in m_fightData)
	//		{
	//			var.type = 0;
	//		}
	//	}
	//	else
	//	{
	//		m_disk->imageData.insert(m_sortie++, m_fightData);
	//		m_fightData.clear();
	//		m_sortieImageSize = 0;
	//	}
	//}

	//info.type = 1;
	//m_sortieImageSize++;
	//m_fightData.append(info);
	//m_info = info;
}

int Image::readSingleInfo(std::string file, ImageInfo &info)
{
	//�ж��ļ��Ƿ���Ч  
	if (!Exiv2::fileExists(file, true)) {
		Log::INFO(QStringLiteral("��ȡ�ļ�exifʱ���ļ������ڡ�"));
		return -1;
	}

	//��ȡ�ļ���С
	FILE *FP = fopen(file.c_str(), "rb");
	fseek(FP, 0, SEEK_END);			//��λ���ļ�ĩ 
	int fileSize = ftell(FP);       //�ļ�����
	fclose(FP);

	try
	{
		Exiv2::Image::AutoPtr &image = Exiv2::ImageFactory::open(file, false);
		if (image.get() == 0)
		{
			Log::INFO(QStringLiteral("��ȡexifʧ�ܡ�"));
			return -2;
		}
		//��ȡ��Ƭ��exif��Ϣ  
		image->readMetadata();
		Exiv2::ExifData ed = image->exifData();//�õ�exif��Ϣ  

		if (ed.empty())
		{
			Log::INFO(QStringLiteral("δ����exif��Ϣ��"));
			return -3;
		}

		info.fileName = QString::fromStdString(file);
		QStringList list = info.fileName.split('/');
		if (!list.isEmpty())
		{
			QString name = list.last();
			nameContinuous(name);
			info.imageName = name;
		}
		info.iso = QString::fromStdString(FindExifKey(ed, "Exif.Photo.ISOSpeedRatings"));
		info.aperture = QString::fromStdString(FindExifKey(ed, "Exif.Photo.FNumber"));
		info.shutterspeed = QString::fromStdString(FindExifKey(ed, "Exif.Photo.ExposureTime"));
		info.stime = QString::fromStdString(FindExifKey(ed, "Exif.Image.DateTime"));
		QDateTime dateTime = QDateTime::fromString(info.stime, "yyyy:MM:dd HH:mm:ss");
		info.timestamp = dateTime.toSecsSinceEpoch();
		info.size = fileSize;

		Log::INFO(QStringLiteral("Ӱ�����ƣ�%1�� Ӱ������ʱ�䣺 %2, ʱ���Ϊ�� %3").arg(info.fileName).arg(info.stime).arg(info.timestamp));

		return 0;
	}
	catch (Exiv2::AnyError& e)
	{
		//cout << "An exception occurred. Exception Nr. " << e << '\n';
		return -1;
	}
	
	//std::cout << "��Ƭ�ߴ�	:" << FindExifKey(ed, "Exif.Photo.PixelYDimension") << " x " << FindExifKey(ed, "Exif.Photo.PixelXDimension") << std::endl;

	//std::cout << "�ļ�����	:" << argv[1] << std::endl;
	//std::cout << "�ļ���С	:" << FileSize << std::endl;
	//std::cout << "��Ȩ	:" << FindExifKey(ed, "Exif.Image.Copyright") << std::endl;
	//std::cout << "������	:" << FindExifKey(ed, "Exif.Image.Artist") << std::endl;
	//std::cout << "���Ʒ��	:" << FindExifKey(ed, "Exif.Image.Make") << std::endl;
	//std::cout << "����ͺ�	:" << FindExifKey(ed, "Exif.Image.Model") << std::endl;
	//std::cout << "��ͷ�ͺ�	:" << FindExifKey(ed, "Exif.Photo.LensModel") << std::endl;
	//std::cout << "��ͷ���к�	:" << FindExifKey(ed, "Exif.Photo.LensSerialNumber") << std::endl;
	//std::cout << "ISO	:" << FindExifKey(ed, "Exif.Photo.ISOSpeedRatings") << std::endl;
	//std::cout << "��Ȧ	:" << FindExifKey(ed, "Exif.Photo.FNumber") << std::endl;
	//std::cout << "����	:" << FindExifKey(ed, "Exif.Photo.FocalLength") << std::endl;
	//std::cout << "����	:" << FindExifKey(ed, "Exif.Photo.ExposureTime") << std::endl;
	//std::cout << "����ʱ��	:" << FindExifKey(ed, "Exif.Image.DateTime") << std::endl;
	//std::cout << "�����	:" << FindExifKey(ed, "Exif.CanonCs.FlashMode") << std::endl;
	//std::cout << "ˮƽ�ֱ���	:" << FindExifKey(ed, "Exif.Image.XResolution") << std::endl;
	//std::cout << "��ֱ�ֱ���	:" << FindExifKey(ed, "Exif.Image.YResolution") << std::endl;
	//std::cout << "��Ƭ�ߴ�	:" << FindExifKey(ed, "Exif.Photo.PixelYDimension") << " x " << FindExifKey(ed, "Exif.Photo.PixelXDimension") << std::endl;

	/*std::cout << "\n������ԭʼ��Ϣ:\n" << std::endl;
	for (Exiv2::ExifData::iterator tmp = ed.begin(); tmp != ed.end(); tmp++) {
	std::cout << tmp->tagName() << " (" << tmp->key() << ")       " << tmp->value() << std::endl;
	}*/

	
}
