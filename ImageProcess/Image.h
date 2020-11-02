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
	
	//读取SD卡下的图片信息
	void readInfo();

	//读取本地路径下的图片信息
	void readAndSortLocalImage();

	//设置要读取与分类的图片信息
	void setImages(QStringList images);

	//设置配置文件
	static void setConfig(int gap, int sizeDiff, int minImages);

signals:
	void signalInfos(QMap<int, QList<ImageInfo>> m_sortedImage); //
	void signalSortFinish(UDisk *);	//分类完成

private:
	//名字连续性
	void nameContinuous(QString &name);
	
	//架次分批
	void sortie(ImageInfo info);

	//获取图片info
	void getImageInfo();

	//本地架次分批
	void sortieLocal();

	//读取单个文件
	int readSingleInfo(std::string file, ImageInfo &info);

	QString m_path;								//路径
	UDisk *m_disk = nullptr;					//sd卡对象
	QString m_imageName;						//last image name
	int m_lastIndex = -1;						//last image index
	int m_timestamp = -1;						//last image time
	QRegExp *m_reg;								//regexp
	ImageInfo m_info;							//last info
	QList<ImageInfo> m_exifImages;				//所有待排序影像，均已获取exif信息
	QList<ImageInfo> m_fightData;				//架次影像

	int m_sortieImageSize = 0;					//架次影像大小
	int m_sortie = 0;							//架次

	static int m_fightGap;						//架次时间间隔：可在配置文件设置
	static int m_sizeDiff;						//架次影像大小差值：用于架次分类，可在配置文件设置
	static int m_minFightImages;				//架次最小影像数：可在配置文件设置

	QStringList m_images;						//本地绝对路径下影像名称：用于影像排序
	QList<ImageInfo> m_infos;					//影像对应的info
	QMap<int, QList<ImageInfo>> m_sortedImage;	//sorted images
};
