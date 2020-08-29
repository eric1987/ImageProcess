#pragma once

#include <QDateTime>
#include <QMetaType>
#include <QMap>

struct ImageInfo;

typedef	QList<ImageInfo> LineData;
typedef QList<LineData> SortData;

struct PosInfo
{
	int index = -1;				///<���
	int timestamp = 0;			///<ʱ���
	QString time = "";			///<ʱ��
	float latitude = 0.0;		///<γ��
	float longitude = 0.0;		///<����
	float altitude = 0.0;		///<�߶�
	float pitching = 0.0;		///<����
	float rolling = 0.0;		///<��ת
	float driftAngle = 0.0;		///<��ƫ��
	float heading = 0.0;		///<����
	float groundSpeed = 0.0;	///<����
	int type = -1;				///<�������ͣ�-1����ʼֵ��0��������1������

	QString date;
	QString id;
};

struct ImageInfo
{
	int timestamp = 0;
	QDateTime time;
	QString stime;
	QString aperture;
	QString shutterspeed;
	QString iso;
	int size;
	QString imageName;			///<����Ӱ��洢ʱ������
	QString fileName;			///<����Ӱ��ת��ʹ�ã�Ӱ�����·��
	int type = -1;				///<�������ͣ�-1����ʼֵ��0��������1������
	bool transfer = false;			///<����״̬��-1����ʼֵ��0��δ���䣬1���������
	bool directTrans = false;
};

struct ImageSD
{
	int imageSize;
	float total;
	float available;
	QString path;
};

struct SortInfo
{
	int index;		//�ܴα��
	int imageSize;	//Ӱ����
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
	QString deviceID = "";				//sd��ΨһID��
	QString nickname = "";				//cam nickname
	float collective = 0;				//capacity
	float available = 0;				//available				
	QString path = "";					//path
	int imageSize = 0;					//image size
	bool connected = false;				//true:have connected false:not connected
	bool sortie = false;				//true: get image exif, and sortied.
	QMap<QString, bool> directTransStatus;	//ֱ�Ӵ���״̬��QString��DCIM����·��
};

struct SortieStatus
{
	QString deviceID = "";					//sd��ΨһID��
	QList<SortData> imagedata;				//�ּܴε�Ӱ��
	QMap<int, bool> sortieTransStatus;		//�ܴδ���״̬��int���ܴΣ�bool������״̬
	QMap<QString, bool> directTransStatus;	//ֱ�Ӵ���״̬��qstring��·��, bool������״̬
};


Q_DECLARE_METATYPE(PosInfo);
Q_DECLARE_METATYPE(ImageInfo);
Q_DECLARE_METATYPE(ImageSD);
Q_DECLARE_METATYPE(SortInfo);
Q_DECLARE_METATYPE(UDisk*);
Q_DECLARE_METATYPE(SDInfo);