#pragma once

#include <QThread>
#include <QDir>
#include <ActiveQt/QAxObject>
#include <QFile>
#include <QMessageBox>
#include "Common.h"
#include "Log/Log.h"

//����block�ļ���
class GenBlockFile : public QThread
{
	Q_OBJECT

public:
	GenBlockFile(QObject *parent = nullptr);
	~GenBlockFile();
	void run();
	//�����ļ���Ӱ�� ����ʹ��
	void setFileAndImages(QString file, QMap<int, QMap<int, QList<ImageInfo>>> images);
	//����pos��ܴ���Ϣ�� ����ʹ��
	void setPosAndSorties(QMap<int, QList<PosInfo>> posData, QList<int> sorties);
	//����Ӱ���ļ��� ����ʹ��
	void setImages(QMap<int, QStringList> images);
	//����pos�ļ��� ����ʹ��
	void setPosFile(QStringList files);
	//�������ɷ�ʽ�� ����ʹ��
	void setGenWay(int type);
	//����block���ݣ���������block�ļ���
	void setBlocks(QMap<int, QMap<int, QList<Block>>> blocks);
	//����base·��
	void setBasePath(QString path);
	//��������block�ļ�����
	void setFile(QString file);
	//����ģ���ļ�
	void setTemplateFile(QString file) { m_template = file; }

signals:
	//��������ź�
	void signalFinished(bool b);

private:
	//
	QList<int> getEqualSortie();
	//
	void prePareData();
	//дexcel�ļ�
	bool writeExcel(QString file, int row, int column, QVariant &var, QString basePath);
	//дoption sheetҳ
	void writeOptions(QAxObject *optionSheet, QString path);
	//д���Ԫ��
	void writeCell(QAxObject *sheet, int row, int column, QString value);
	//��ȡbase·��
	void getBasePath();
	//��block���ݱ�Ϊ��д��excel�����ݣ� �����block�ļ�д��
	void changeBlocksIntoExcelData();

private:
	QString m_file;			//block�ļ���
	QMap<int, QMap<int, QList<ImageInfo>>> m_images;	//Ӱ�����ݣ�����ʹ��
	QMap<int, QList<PosInfo>> m_posData;	//pos���ݣ�����ʹ��
	QList<int> m_sorties;		//�ܴη���
	QList<Block> m_content;		//blockת��������
	QString m_basePath;	//cam�ĸ�Ŀ¼
	int m_genWay;	//���ɷ�ʽ������ʹ��
	QMap<int, QMap<int, QList<Block>>> m_blocks;	//block����
	QString m_template;	//ģ���ļ�
};
