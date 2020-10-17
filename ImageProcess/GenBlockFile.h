#pragma once

#include <QThread>
#include <QDir>
#include <ActiveQt/QAxObject>
#include <QFile>
#include <QMessageBox>
#include "Common.h"
#include "Log/Log.h"

//生成block文件类
class GenBlockFile : public QThread
{
	Q_OBJECT

public:
	GenBlockFile(QObject *parent = nullptr);
	~GenBlockFile();
	void run();
	//设置文件与影像： 不再使用
	void setFileAndImages(QString file, QMap<int, QMap<int, QList<ImageInfo>>> images);
	//设置pos与架次信息： 不再使用
	void setPosAndSorties(QMap<int, QList<PosInfo>> posData, QList<int> sorties);
	//设置影像文件： 不再使用
	void setImages(QMap<int, QStringList> images);
	//设置pos文件： 不再使用
	void setPosFile(QStringList files);
	//设置生成方式： 不再使用
	void setGenWay(int type);
	//设置block数据（用于生成block文件）
	void setBlocks(QMap<int, QMap<int, QList<Block>>> blocks);
	//设置base路径
	void setBasePath(QString path);
	//设置生成block文件名称
	void setFile(QString file);
	//设置模板文件
	void setTemplateFile(QString file) { m_template = file; }

signals:
	//生成完毕信号
	void signalFinished(bool b);

private:
	//
	QList<int> getEqualSortie();
	//
	void prePareData();
	//写excel文件
	bool writeExcel(QString file, int row, int column, QVariant &var, QString basePath);
	//写option sheet页
	void writeOptions(QAxObject *optionSheet, QString path);
	//写表格单元格
	void writeCell(QAxObject *sheet, int row, int column, QString value);
	//获取base路径
	void getBasePath();
	//将block数据变为可写入excel的数据， 并完成block文件写入
	void changeBlocksIntoExcelData();

private:
	QString m_file;			//block文件名
	QMap<int, QMap<int, QList<ImageInfo>>> m_images;	//影像数据，不再使用
	QMap<int, QList<PosInfo>> m_posData;	//pos数据，不再使用
	QList<int> m_sorties;		//架次分类
	QList<Block> m_content;		//block转化后数据
	QString m_basePath;	//cam的父目录
	int m_genWay;	//生成方式：不再使用
	QMap<int, QMap<int, QList<Block>>> m_blocks;	//block数据
	QString m_template;	//模板文件
};
