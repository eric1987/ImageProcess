//TODO����ɱ���·����Ӱ��exif��Ϣ��ȡ
#pragma once

#include "Common.h"
#include "Log/Log.h"

#include <QObject>
#include <exiv2/exiv2.hpp>
#include <QRegExp>
#include <QDateTime>
#include <QDir>
#include <QMap>
#include <QList>

class Image : public QObject
{
	Q_OBJECT

public:
	Image(QObject *parent = Q_NULLPTR);
	~Image();

	//�޸�·��
	void setPath(QString path);

	//����Ҫ��ȡ��u����Ϣ
	void setUDiskInfo(UDisk *udisk);
	
	//��ȡSD���µ�ͼƬ��Ϣ
	void readInfo();

	//��ȡ����·���µ�ͼƬ��Ϣ
	void readAndSortLocalImage();

	//����Ҫ��ȡ������ͼƬ��Ϣ
	void setImages(QStringList images);

	//���������ļ�
	static void setConfig(int gap, int sizeDiff, int minImages);

signals:
	void signalInfos(QMap<int, QList<ImageInfo>> m_sortedImage); //
	void signalSortFinish(UDisk *);	//�������

private:
	//����������
	void nameContinuous(QString &name);
	
	//�ܴη���
	void sortie(ImageInfo info);

	//��ȡͼƬinfo
	void getImageInfo();

	//���ؼܴη���
	void sortieLocal();

	//��ȡ�����ļ�
	int readSingleInfo(std::string file, ImageInfo &info);

	QString m_path;								//·��
	UDisk *m_disk = nullptr;					//sd������
	QString m_imageName;						//last image name
	int m_lastIndex = -1;						//last image index
	int m_timestamp = -1;						//last image time
	QRegExp *m_reg;								//regexp
	ImageInfo m_info;							//last info
	QList<ImageInfo> m_exifImages;				//���д�����Ӱ�񣬾��ѻ�ȡexif��Ϣ
	QList<ImageInfo> m_fightData;				//�ܴ�Ӱ��

	int m_sortieImageSize = 0;					//�ܴ�Ӱ���С
	int m_sortie = 0;							//�ܴ�

	static int m_fightGap;						//�ܴ�ʱ���������������ļ�����
	static int m_sizeDiff;						//�ܴ�Ӱ���С��ֵ�����ڼܴη��࣬���������ļ�����
	static int m_minFightImages;				//�ܴ���СӰ���������������ļ�����

	QStringList m_images;						//���ؾ���·����Ӱ�����ƣ�����Ӱ������
	QList<ImageInfo> m_infos;					//Ӱ���Ӧ��info
	QMap<int, QList<ImageInfo>> m_sortedImage;	//sorted images
};
