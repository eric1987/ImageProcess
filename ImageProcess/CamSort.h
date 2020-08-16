#pragma once

#include <QWidget>
#include <QStandardPaths>
#include <windows.h>
#include <dbt.h>
#include <QStorageInfo>
#include <QMapIterator>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QThread>
#include <QMessageBox>

#include "ui_CamSort.h"
#include "CamSD.h"
#include "Common.h"
#include "Image.h"
#include "Log/Log.h"
#include "PosSorting.h"
#include "SortieWidget.h"
#include "Transfer.h"

//TODO��ֱ��ת�棬��ʽ��
//TODO: ����excel�ļ�

class CustomSortie : public QWidget
{
	Q_OBJECT
public:
	CustomSortie(int sortie, int size, QWidget *parent = Q_NULLPTR)
		: QWidget(parent)
	{
		m_sortie = sortie;
		box = new QCheckBox(QStringLiteral("�ܴ�%1  Ӱ��%2��").arg(sortie).arg(size));
		btn = new QPushButton(QStringLiteral("�鿴����"));
		btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		btn->setBackgroundRole(QPalette::Highlight);

		layout = new QHBoxLayout(this);
		layout->setSpacing(5);
		layout->setContentsMargins(0, 0, 0, 0);
		this->setLayout(layout);
		layout->addWidget(box);
		layout->addWidget(btn);

		connect(box, &QCheckBox::stateChanged, this, &CustomSortie::checkChanged);
		connect(btn, &QPushButton::clicked, this, &CustomSortie::showSortie);
	}
	~CustomSortie(){}

signals:
	void signalCheckChanged(int state, int sortie);
	void signalSortieDetail(int sortie);

private:
	void checkChanged(int state)
	{
		emit signalCheckChanged(state, m_sortie);
	}

	void showSortie()
	{
		emit signalSortieDetail(m_sortie);
	}

	int m_sortie = -1;
	QCheckBox *box = nullptr;
	QPushButton *btn = nullptr;
	QHBoxLayout *layout = nullptr;
};

class CamSort : public QWidget
{
	Q_OBJECT

		enum POSTYPE{DAJ = 0, ZONGHENG};

public:
	CamSort(QWidget *parent = Q_NULLPTR);
	~CamSort();

	//������sd��Ϣ������
	void addCamSD(QString id, QString name, float available, float collective);
	
	//ˢ��sd����Ϣ
	void refreshSDInfo();
signals:
	void signalStatus(QString str);
	void signalSDInfo(QMap<QString, SDInfo> sdInfo);

private:
	void test();

	//��ʼ��
	void init();

	//����
	void connects();

	//ѡ��pos�ļ�
	void selectPosFiles();

	//�ı�pos����
	void changePosType(int index);

	

	//��ȡ����sd��Ϣ
	void getSDInfo(QString path);

	//��ȡsd�����ǳ���Ӱ������
	void getSDNicknameAndImageSize(SDInfo &sd, QString path);

	//Ӱ���ļ��ܴη���
	void getSorties();

	//�յ�pos�ּܴ���Ϣ����׼����ʾ�ܴθ���
	void prepareShowSorties(QMap<int, QList<PosInfo>> posData);

	//��ʾ�ܴθ���: sorties, key:�ܴΣ�value��Ӱ������
	void showSorties(QMap<int, int> sorties);

	//��ʾ�ܴ�����
	void showSortieDetail(int sortie);

	//�ܴ�ѡ��仯
	void sortieSelectChanged(int state, int sortie);

	//�ܴδ���
	void sortTransfer();

	//����Ӱ��ܴδ洢·��
	void setSavePath();
	void changeSavePath(QString path);

	//���ò���
	void unableOperation();

	//���ò���
	void enableOperation();
	
	//��ȡӰ��exif��Ϣ
	void getImageExif();
 
	//ˢ�¼ܴδ������
	void refreshProcess(int value);

	//�ܴδ������
	void transFinished();

private:
	Ui::CamSort ui;
	QVBoxLayout *m_sortLayout = nullptr;

	QMap<QString, CamSD*> m_sdList;	//sd��list��key��id������չʾsd��
	QMap<QString, UDisk*> m_disks;	//sd����Ϣ������Ӱ����Ϣ�����ڼܴη���
	QMap<QString, SDInfo> m_sdInfo;	//sd��Ϣ������sd����״̬���洢������
	QMap<int, QList<PosInfo>> m_posData;	//�ּܴκ��pos����

	int m_posType = DAJ;			//pos����
	QString m_savePath;				//�洢·��
	QStringList m_posFiles;			//pos�ļ�
	QList<int> m_transferSortie;	//Ҫ����ļܴ�
	int m_camNum = 0;				//

	QMap<int, bool> m_sortieSelectStatus;	//�ܴ�ѡ��״̬
	PosSorting *m_posSort = nullptr;
	QMap<int, SortieWidget*> m_sortieWidgets;	//�ܴν������

	Transfer *m_transfer = nullptr;
};
