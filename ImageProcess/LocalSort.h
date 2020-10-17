#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QVBoxLayout>

#include "PosSorting.h"
#include "CustomSortie.h"
#include "SortieWidget.h"
#include "ExcelOperator.h"
#include "GenBlockFile.h"
#include "ui_LocalSort.h"
#include "Transfer.h"

class LocalSort : public QWidget
{
	Q_OBJECT
		enum POSTYPE { DAJ = 0, ZONGHENG };
public:
	LocalSort(QWidget *parent = Q_NULLPTR);
	~LocalSort();

signals:
	void signalStatus(QString mes);

private:
	void init();				//初始化
	void connects();			//连接
	void slotOpenPos();			//选择pos文件
	void slotChangePosType(int index);	//设置pos类型
	void slotStartSort();		//开始排序
	void receivePosData(QMap<int, QList<PosInfo>> posData);	//接收排序好的pos数据
	bool isDataPrepared();		//数据是否准备好
	void receiveImageData(QMap<int, QList<ImageInfo>> imageData);	//接收分类影像数据
	void sortiesDisplay();		//展示架次信息
	void sortieSelectChanged(int state, int sortie);	//影像架次选择变化
	void showSortieDetail(int sortie);		//显示架次详细信息
	void imageAdd(bool state);		//增加添加了影像的相机
	void addCamTab(int index);		//添加影像架次显示
	QMap<QString, QList<ImageInfo>> getSortieImage(int sortie);	//获取架次分类的影像
	QMap<int, QMap<int, QList<ImageInfo>>> needToGenBlockSorts();	//需要生成block的影像文件：不再使用
	void createBlockFile();	//生成block文件： 不再使用
	void slotGenBlockFinished(bool b);	//生成block文件完成函数： 不再使用
	void blockPathChanged();	//block路径变化：不再使用
	void blockPathSelected();	//block路径选择：不再使用
	void sortTransfer(bool checked);		//传输架次
	void unableOperation();	//禁用操作
	void enableOperation();	//启用操作
	void rePosAndSave(QList<int> sorties);	//修改pos并保存
	QString getImageName(QMap<int, QList<ImageInfo>> &images);	//获取影像名称：ASDWX
	QString setSaveReposFileName(QString cam, int sortie, QString path);//命名转存pos文件名称
	void refreshProcess(int process);	//刷新传输进度
	void transFinished();	//传输完成
	void onclosed(int index);	//关闭架次
	void setCheckSorties(int state);	//设置架次选中状态
private:
	Ui::LocalSort ui;
	QStringList m_posFiles;		//pos文件
	int m_posType = DAJ;		//pos类型
	PosSorting *m_posSort = nullptr;		//pos分类对象
	const int m_camSize = 6;		//cam数据
	QMap<int, QList<PosInfo>> m_posData;	//pos分架次后数据
	QMap<int, QList<ImageInfo>> m_image1;	//image1分架次后数据
	QMap<int, QList<ImageInfo>> m_image2;	//image2分架次后数据
	QMap<int, QList<ImageInfo>> m_image3;	//image3分架次后数据
	QMap<int, QList<ImageInfo>> m_image4;	//image4分架次后数据
	QMap<int, QList<ImageInfo>> m_image5;	//image5分架次后数据
	QMap<int, QList<ImageInfo>> m_image6;	//image6分架次后数据
	QMap<int, QMap<int, QList<ImageInfo>>> m_images;	//分架次后数据：不再使用
	QMap<QString, QMap<int, QList<ImageInfo>>> m_localImages;	//分架次后本地数据
	QMap<int, bool> m_imageisReady;	//影像数据准备好与否状态
	QVBoxLayout *m_sortLayout = nullptr;
	QMap<int, bool> m_sortieSelectStatus;	//选择的架次
	QMap<int, int> m_sortieIndex;		//架次结果界面，key为架次，value为widget的index值
	QMap<int, bool> m_cams;				//添加影像的相机状态，true代表添加了要分类的影像。
	QString m_savePath;				//保存路径
	QString m_basePath;				//父目录

	int m_addIndex = 1;				//架次展示index
	QList<int> m_transferSortie;	//要传输的架次
	Transfer *m_transfer = nullptr;	//传输对象
};
