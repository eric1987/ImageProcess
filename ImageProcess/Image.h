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
	
	//��ȡ·���µ�ͼƬ��Ϣ
	void readInfo();

signals:
	void signalInfos(QMap<int, QList<ImageInfo>> m_sortedImage);
	void signalSortFinish(UDisk *);

private:
	//����������
	void nameContinuous(QString &name);
	
	//�ܴη���
	void sortie(ImageInfo info);

	//��ȡ�����ļ�
	int readSingleInfo(std::string file, ImageInfo &info);

	QString m_path;
	UDisk *m_disk = nullptr;
	QString m_imageName;						//last image name
	int m_lastIndex = -1;						//last image index
	int m_timestamp = -1;						//last image time
	QRegExp *m_reg;								//regexp
	ImageInfo m_info;							//last info
	QList<ImageInfo> m_fightData;				//�ܴ�Ӱ��
	QMap<int, QList<ImageInfo>> m_sortedImage;	//sorted images

	int m_sortieImageSize = 0;
	int m_sortie = 0;
	float m_averageGap = 2.0;
	const int m_sortieTime = 100;			///<�ܴ�ʱ�����������Ƭ����ı���
	const int m_fightImageMinNum = 30;		///<�ܴ�����������Ƭ����
};