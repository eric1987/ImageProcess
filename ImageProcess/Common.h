#pragma once

#include <QDateTime>
#include <QMetaType>
#include <QMap>

struct ImageInfo;

typedef	QList<ImageInfo> LineData;
typedef QList<LineData> SortData;

struct PosInfo
{
	int index = -1;				///<序号
	int timestamp = 0;			///<时间戳
	QString time = "";			///<时间
	float latitude = 0.0;		///<纬度
	float longitude = 0.0;		///<经度
	float altitude = 0.0;		///<高度
	QString strLat = "";		///<纬度
	QString strLong = "";		///<经度
	QString strAlt = "";		///<高度
	float pitching = 0.0;		///<俯仰
	float rolling = 0.0;		///<滚转
	float driftAngle = 0.0;		///<航偏角
	float heading = 0.0;		///<航向
	float groundSpeed = 0.0;	///<地速
	int type = -1;				///<数据类型，-1：初始值，0：舍弃，1：留用

	QString date;				///<日期
	QString id;					///<id
};

struct ImageInfo
{
	int timestamp = 0;			///<时间戳
	QDateTime time;				///<时间
	QString stime;				///<时间
	QString aperture;			///<光圈
	QString shutterspeed;		///<快门速度
	QString iso;				///<iso
	int size;					///<大小
	QString imageName;			///<用于影像存储时的名称
	QString fileName;			///<用于影像转存使用：影像绝对路径
	int type = -1;				///<数据类型，-1：初始值，0：舍弃，1：留用
	bool transfer = false;			///<传输状态：-1：初始值，0：未传输，1：传输完成
	bool directTrans = false;
};

struct Block
{
	QString imageName;			///<影像名称
	QString groupName;			///<group名
	QString nickName;			///<相机名称
	QString latitude;			///<纬度
	QString longitude;			///<经度
	QString height;				///<高度
};

struct ImageSD
{
	int imageSize;			///<影像大小
	float total;			///<总大小
	float available;		///<可用大小
	QString path;			///<路径
};

struct SortInfo
{
	int index;		//架次编号
	int imageSize;	//影像数
};

struct UDisk
{
	QString diskID;							//u disk serial number
	bool connected = false;					//true:have connected false:not connected
	QString cam;							//cam nickname
	//float capacity;						//capacity
	//float available;						//available
	//float used;							//used
	QString path;							//path
	//int imageNum;							//image number
	QMap<int, QList<ImageInfo>> imageData;	//sortie images
	QMap<int, bool> transImage;				//sorties need to transfer. key: sortie number; value: status
	bool sortieTransStatus = false;			//sortie trans status
	bool directTransStatus = false;
	bool sortieStatus = false;				//sortie image status. false: not finish, true finish
	QMap<QString, bool> exifStatus;			//get image exif status. key: path; value status.	
};

struct SDInfo
{
	QString deviceID = "";				//sd卡唯一ID号
	QString nickname = "";				//cam nickname
	float collective = 0;				//capacity
	float available = 0;				//available				
	QString path = "";					//path
	int imageSize = 0;					//image size
	bool connected = false;				//true:have connected false:not connected
	bool sortie = false;				//true: get image exif, and sortied.
	QMap<QString, bool> directTransStatus;	//直接传输状态，QString：DCIM下子路径
};

struct SortieStatus
{
	QString deviceID = "";					//sd卡唯一ID号
	QList<SortData> imagedata;				//分架次的影像
	QMap<int, bool> sortieTransStatus;		//架次传输状态：int：架次；bool：传输状态
	QMap<QString, bool> directTransStatus;	//直接传输状态：qstring：路径, bool：传输状态
};


Q_DECLARE_METATYPE(PosInfo);
Q_DECLARE_METATYPE(ImageInfo);
Q_DECLARE_METATYPE(ImageSD);
Q_DECLARE_METATYPE(SortInfo);
Q_DECLARE_METATYPE(UDisk*);
Q_DECLARE_METATYPE(SDInfo);