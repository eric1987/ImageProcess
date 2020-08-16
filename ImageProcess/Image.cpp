#include "Image.h"

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

void Image::readInfo()
{
	QDir dir(m_disk->path);
	QStringList dirList = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
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

			for each (QString var in fileList)
			{
				ImageInfo info;
				int ret = readSingleInfo((subPath + "/" + var).toStdString(), info);
				if (ret < 0)
				{
					Log::INFO(QStringLiteral("��ȡͼƬ��Ϣ����."));
					return;
				}

				sortie(info);
				m_info = info;
			}

			m_disk->exifStatus[subPath] = true;
		}
	}

	if (m_sortieImageSize < m_fightImageMinNum)
	{
		for each (ImageInfo var in m_fightData)
		{
			var.type = 0;
		}
	}
	m_disk->imageData.insert(m_sortie++, m_fightData);
	m_sortieImageSize = 0;
	m_sortie = 0;

	m_disk->sortieStatus = true;
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
	if (fabs(info.timestamp - m_info.timestamp) > m_averageGap * m_sortieTime)
	{
		//�жϼܴ��е�image����������50��Ϊ���������㡣
		if (m_sortieImageSize < m_fightImageMinNum)
		{
			for each (ImageInfo var in m_fightData)
			{
				var.type = 0;
			}
		}
		else
		{
			m_disk->imageData.insert(m_sortie++, m_fightData);
			m_fightData.clear();
			m_sortieImageSize = 0;
		}
	}

	info.type = 1;
	m_sortieImageSize++;
	m_fightData.append(info);
	m_info = info;
}

int Image::readSingleInfo(std::string file, ImageInfo &info)
{
	//�ж��ļ��Ƿ���Ч  
	if (!Exiv2::fileExists(file, true)) {
		Log::INFO(QStringLiteral("��ȡ�ļ�exifʱ���ļ������ڡ�"));
		return -1;
	}

	//��ȡ�ļ���С
	//FILE *FP = fopen(file.c_str(), "rb");
	//fseek(FP, 0, SEEK_END);			//��λ���ļ�ĩ 
	//int FileSize = ftell(FP);       //�ļ�����
	//fclose(FP);

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