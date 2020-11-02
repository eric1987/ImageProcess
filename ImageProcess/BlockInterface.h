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
	void init();	//��ʼ��
	void selectPos();	//ѡ���������ɵ�pos�ļ�
	void blockPathChanged();	//block����·���仯
	void blockPathSelected();	//block����·��ѡ��
	void basePathChanged();		//base·���仯
	void basePathSelected();	//base·��ѡ��
	void clearPos();			//���pos
	void genBlockFile();		//����block�ļ�
	void readPosFile();			//��ȡpos�ļ�
	void readSignalFile(QString file);		//��ȡ�����ļ�
	/**
	* @brief �����ļ���
	* @file: �ļ����� 
	* @groupName, excel�ж�Ӧ�ļ���groupName
	* @sort���ܴ���Ϣ
	* @nickname��������ƣ�eg��A��S��D��W
	*/
	void decodeFileName(QString file, int &groupName, int &sort, QString &nickname);
	/**
	* @brief block�ļ��������
	* @b: ���ɳɹ����
	*/
	void slotGenBlockFinished(bool b);	
	void onTemplateFile();		//��ģ���ļ�
	/**
	* @brief ģ���ļ��仯
	* @ text��ģ���ļ�����
	*/
	void onTemplateChanged(QString text);	

private:
	Ui::BlockInterface ui;
	QStringList m_posFiles;		//pos�ļ�
	QString m_blockPath;		//block����·��
	QString m_basePath;			//base·��
	QMap<int, QMap<int, QList<Block>>> m_blocks;	//Ҫ����block�õ�������
	QString m_templateFile;		//ģ���ļ�
};
