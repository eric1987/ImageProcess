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
	void init();
	void connects();			//
	void slotOpenPos();			//选择pos文件
	void slotChangePosType(int index);	//设置pos类型
	void slotStartSort();		//开始排序
	void receivePosData(QMap<int, QList<PosInfo>> posData);	//接收排序好的pos数据
	bool isDataPrepared();		//数据是否准备好
	void receiveImageData(QMap<int, QList<ImageInfo>> imageData);
	void sortiesDisplay();		//展示架次信息
	void sortieSelectChanged(int state, int sortie);
	void showSortieDetail(int sortie);
	void imageAdd(bool state);
	void addCamTab(int index);
	QMap<QString, QList<ImageInfo>> getSortieImage(int sortie);
	QMap<int, QMap<int, QList<ImageInfo>>> needToGenBlockSorts();
	void createBlockFile();
	void slotGenBlockFinished(bool b);
	void blockPathChanged();
	void blockPathSelected();
	void sortTransfer(bool checked);		//传输架次
	void unableOperation();
	void enableOperation();
	void rePosAndSave(QList<int> sorties);	//修改pos并保存
	QString getImageName(QMap<int, QList<ImageInfo>> &images);
	QString setSaveReposFileName(QString cam, int sortie, QString path);//命名转存pos文件名称
	void refreshProcess(int process);
	void transFinished();
	void onclosed(int index);
	void setCheckSorties(int state);
private:
	Ui::LocalSort ui;
	QStringList m_posFiles;
	int m_posType = DAJ;
	PosSorting *m_posSort = nullptr;
	const int m_camSize = 6;
	QMap<int, QList<PosInfo>> m_posData;
	QMap<int, QList<ImageInfo>> m_image1;
	QMap<int, QList<ImageInfo>> m_image2;
	QMap<int, QList<ImageInfo>> m_image3;
	QMap<int, QList<ImageInfo>> m_image4;
	QMap<int, QList<ImageInfo>> m_image5;
	QMap<int, QList<ImageInfo>> m_image6;
	QMap<int, QMap<int, QList<ImageInfo>>> m_images;
	QMap<QString, QMap<int, QList<ImageInfo>>> m_localImages;
	QMap<int, bool> m_imageisReady;
	QVBoxLayout *m_sortLayout = nullptr;
	QMap<int, bool> m_sortieSelectStatus;	//选择的架次
	QMap<int, int> m_sortieIndex;		//架次结果界面，key为架次，value为widget的index值
	QMap<int, bool> m_cams;				//添加影像的相机状态，true代表添加了要分类的影像。
	QString m_savePath;
	QString m_basePath;				//父目录

	int m_addIndex = 1;
	QList<int> m_transferSortie;	//要传输的架次
	Transfer *m_transfer = nullptr;
};
