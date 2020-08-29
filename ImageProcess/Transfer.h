#pragma once

#include <QObject>
#include <QDir>
#include <QRegExp>
#include <QThread>

#include "Common.h"
#include "Log/Log.h"
#include "Util.h"

enum TRANSTYPE { SORTIE = 1, DIRECT };	//SORTIE�����ܴδ��䣬 DIRECT��ֱ�Ӵ���

class Transfer : public QThread
{
	Q_OBJECT

public:	
	Transfer(QThread *parent = Q_NULLPTR);
	~Transfer();

	//���ô�������
	void setTransType(int type);

	//����Ҫ�����·��
	void setDstPath(QString path);

	//����Ҫ����ļܴ���Ϣ
	void setTransSortie(QList<int> num, QMap<QString, UDisk*> *disks);

	//����Ҫ�����sd��
	void setSDCards(QStringList idList, QMap<QString, SDInfo> &sdInfo);
	
	//ֹͣ����
	void stopTransfer();

	//��ʼ����
	void run();

signals:
	void signalProcess(int value);
	void signalSortieTransFailed();
	void signalSortieTransFinished();
	
	void signalDirectProcess(int value);
	void signalDirectTransFailed();
	void signalDirectTransFinished();

private:
	//�ܴδ��䷽��
	void sortiesTransfer();

	//��ȡ�ܴ�Ӱ������
	int getSortieTotalSize();

	//�����洢·�� sortie:�ܴ���ţ�nickname:sd���ǳ�
	QString createDir(int sortie, QString nickname);

	//���ļ��ܴδ��䷽�� srcFile��Դ�ļ� dstPath������Ŀ��·��  sortie������ļܴ� dstName��Ŀ���ļ�����
	bool sortieTransferSingleFile(QString srcFile, QString dstPath, int sortie, QString dstName);

	//��ȡӰ�������ĸ��ļ�����
	//QString getParentFloder(QString srcFile);

	//ֱ�Ӵ��䷽��
	void directTransfer();

	//ֱ�Ӵ��䵥���ļ�����
	bool directTransferSingleFile(QString path, QString srcFile, QString dstPath);

	//�����洢·���� nickname��sd���ǳ�
	QString creatDirectDir(QString nickname);

	//��ȡֱ�Ӵ���Ӱ������
	int getDirectTotalSize();

	

private:
	int m_transType;		//��������
	QString m_dstPath;					//����Ŀ��·��
	QList<int> m_sorties;				//����ļܴ�
	QStringList m_idList;				//ֱ�Ӵ����sd��id
	QMap<QString, SDInfo> m_sdInfo;		//sd����Ϣ������sd��·��
	QMap<QString, UDisk*> *m_disks = nullptr;		//sd����Ϣ�������зּܴε�Ӱ����Ϣ
	int m_transferCount = 0;		//�Ѵ�������
	int m_transferSize = 0;			//�����¼
	int m_sortieTotal = 0;
	int m_sortieStep = 1;

	int m_directTransCount = 0;
	int m_directTransSize = 0;
	int m_directTotal = 0;
	int m_directStep = 1;

	bool m_stop = false;
};
