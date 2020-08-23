//TODO：完成本地路径下影像exif信息获取
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

	//修改路径
	void setPath(QString path);

	//设置要读取的u盘信息
	void setUDiskInfo(UDisk *udisk);
	
	//读取路径下的图片信息
	void readInfo();

	//读取本地路径下的图片信息
	void readLocalInfo();

signals:
	void signalInfos(QMap<int, QList<ImageInfo>> m_sortedImage);
	void signalSortFinish(UDisk *);

private:
	//名字连续性
	void nameContinuous(QString &name);
	
	//架次分批
	void sortie(ImageInfo info);

	//读取单个文件
	int readSingleInfo(std::string file, ImageInfo &info);

	QString m_path;
	UDisk *m_disk = nullptr;
	QString m_imageName;						//last image name
	int m_lastIndex = -1;						//last image index
	int m_timestamp = -1;						//last image time
	QRegExp *m_reg;								//regexp
	ImageInfo m_info;							//last info
	QList<ImageInfo> m_fightData;				//架次影像
	QMap<int, QList<ImageInfo>> m_sortedImage;	//sorted images

	int m_sortieImageSize = 0;
	int m_sortie = 0;
	float m_averageGap = 2.0;
	const int m_sortieTime = 100;			///<架次时间间隔相对于照片间隔的倍率
	const int m_fightImageMinNum = 30;		///<架次最少拍摄照片数量
};
