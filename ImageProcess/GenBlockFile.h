//TODO:block文件补全
//TODO：影像图片名称改变。

#pragma once

#include <QThread>
#include <QDir>
#include <ActiveQt/QAxObject>
#include <QFile>
#include <QMessageBox>
#include "Common.h"
#include "Log/Log.h"

class GenBlockFile : public QThread
{
	Q_OBJECT

	enum GENWAY{LOCALWAY = 0, REPOSWAY};

public:
	GenBlockFile(QObject *parent = nullptr);
	~GenBlockFile();
	void run();
	void setFileAndImages(QString file, QMap<int, QMap<int, QList<ImageInfo>>> images);
	void setPosAndSorties(QMap<int, QList<PosInfo>> posData, QList<int> sorties);
	void setImages(QMap<int, QStringList> images);
	void setPosFile(QStringList files);
	void setGenWay(int type);
	void setBlocks(QMap<int, QMap<int, QList<Block>>> blocks);
	void setBasePath(QString path);
	void setFile(QString file);
	void setTemplateFile(QString file) { m_template = file; }

signals:
	void signalFinished(bool b);

private:
	QList<int> getEqualSortie();
	void prePareData();
	bool writeExcel(QString file, int row, int column, QVariant &var, QString basePath);
	void writeOptions(QAxObject *optionSheet, QString path);
	void writeCell(QAxObject *sheet, int row, int column, QString value);
	void getBasePath();
	void createBlockByRepos();
	void changeBlocksIntoExcelData();

private:
	QString m_file;
	QMap<int, QMap<int, QList<ImageInfo>>> m_images;
	QMap<int, QList<PosInfo>> m_posData;
	QList<int> m_sorties;
	QList<Block> m_content;
	QString m_basePath;	//cam的父目录
	int m_genWay;
	QMap<int, QMap<int, QList<Block>>> m_blocks;
	QString m_template;
};
