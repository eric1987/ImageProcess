#pragma once

#include <QWidget>
#include "ui_BlockInterface.h"
#include "Common.h"

class BlockInterface : public QWidget
{
	Q_OBJECT

public:
	BlockInterface(QWidget *parent = Q_NULLPTR);
	~BlockInterface();

private:
	void init();	//初始化
	void selectPos();	//选择重新生成的pos文件
	void blockPathChanged();	//block生成路径变化
	void blockPathSelected();	//block生成路径选择
	void basePathChanged();		//base路径变化
	void basePathSelected();	//base路径选择
	void clearPos();			//清空pos
	void genBlockFile();		//生成block文件
	void readPosFile();			//读取pos文件
	void readSignalFile(QString file);		//读取单个文件
	/**
	* @brief 解析文件名
	* @file: 文件名称 
	* @groupName, excel中对应文件的groupName
	* @sort：架次信息
	* @nickname：相机名称：eg：A，S，D，W
	*/
	void decodeFileName(QString file, int &groupName, int &sort, QString &nickname);
	/**
	* @brief block文件生成完毕
	* @b: 生成成功与否
	*/
	void slotGenBlockFinished(bool b);	
	void onTemplateFile();		//打开模板文件
	/**
	* @brief 模板文件变化
	* @ text：模板文件名称
	*/
	void onTemplateChanged(QString text);	

private:
	Ui::BlockInterface ui;
	QStringList m_posFiles;		//pos文件
	QString m_blockPath;		//block生成路径
	QString m_basePath;			//base路径
	QMap<int, QMap<int, QList<Block>>> m_blocks;	//要生成block用到的数据
	QString m_templateFile;		//模板文件
};
