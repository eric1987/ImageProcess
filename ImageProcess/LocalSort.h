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
	void init();				//��ʼ��
	void connects();			//����
	void slotOpenPos();			//ѡ��pos�ļ�
	void slotChangePosType(int index);	//����pos����
	void slotStartSort();		//��ʼ����
	void receivePosData(QMap<int, QList<PosInfo>> posData);	//��������õ�pos����
	bool isDataPrepared();		//�����Ƿ�׼����
	void receiveImageData(QMap<int, QList<ImageInfo>> imageData);	//���շ���Ӱ������
	void sortiesDisplay();		//չʾ�ܴ���Ϣ
	void sortieSelectChanged(int state, int sortie);	//Ӱ��ܴ�ѡ��仯
	void showSortieDetail(int sortie);		//��ʾ�ܴ���ϸ��Ϣ
	void imageAdd(bool state);		//���������Ӱ������
	void addCamTab(int index);		//���Ӱ��ܴ���ʾ
	QMap<QString, QList<ImageInfo>> getSortieImage(int sortie);	//��ȡ�ܴη����Ӱ��
	QMap<int, QMap<int, QList<ImageInfo>>> needToGenBlockSorts();	//��Ҫ����block��Ӱ���ļ�������ʹ��
	void createBlockFile();	//����block�ļ��� ����ʹ��
	void slotGenBlockFinished(bool b);	//����block�ļ���ɺ����� ����ʹ��
	void blockPathChanged();	//block·���仯������ʹ��
	void blockPathSelected();	//block·��ѡ�񣺲���ʹ��
	void sortTransfer(bool checked);		//����ܴ�
	void unableOperation();	//���ò���
	void enableOperation();	//���ò���
	void rePosAndSave(QList<int> sorties);	//�޸�pos������
	QString getImageName(QMap<int, QList<ImageInfo>> &images);	//��ȡӰ�����ƣ�ASDWX
	QString setSaveReposFileName(QString cam, int sortie, QString path);//����ת��pos�ļ�����
	void refreshProcess(int process);	//ˢ�´������
	void transFinished();	//�������
	void onclosed(int index);	//�رռܴ�
	void setCheckSorties(int state);	//���üܴ�ѡ��״̬
private:
	Ui::LocalSort ui;
	QStringList m_posFiles;		//pos�ļ�
	int m_posType = DAJ;		//pos����
	PosSorting *m_posSort = nullptr;		//pos�������
	const int m_camSize = 6;		//cam����
	QMap<int, QList<PosInfo>> m_posData;	//pos�ּܴκ�����
	QMap<int, QList<ImageInfo>> m_image1;	//image1�ּܴκ�����
	QMap<int, QList<ImageInfo>> m_image2;	//image2�ּܴκ�����
	QMap<int, QList<ImageInfo>> m_image3;	//image3�ּܴκ�����
	QMap<int, QList<ImageInfo>> m_image4;	//image4�ּܴκ�����
	QMap<int, QList<ImageInfo>> m_image5;	//image5�ּܴκ�����
	QMap<int, QList<ImageInfo>> m_image6;	//image6�ּܴκ�����
	QMap<int, QMap<int, QList<ImageInfo>>> m_images;	//�ּܴκ����ݣ�����ʹ��
	QMap<QString, QMap<int, QList<ImageInfo>>> m_localImages;	//�ּܴκ󱾵�����
	QMap<int, bool> m_imageisReady;	//Ӱ������׼�������״̬
	QVBoxLayout *m_sortLayout = nullptr;
	QMap<int, bool> m_sortieSelectStatus;	//ѡ��ļܴ�
	QMap<int, int> m_sortieIndex;		//�ܴν�����棬keyΪ�ܴΣ�valueΪwidget��indexֵ
	QMap<int, bool> m_cams;				//���Ӱ������״̬��true���������Ҫ�����Ӱ��
	QString m_savePath;				//����·��
	QString m_basePath;				//��Ŀ¼

	int m_addIndex = 1;				//�ܴ�չʾindex
	QList<int> m_transferSortie;	//Ҫ����ļܴ�
	Transfer *m_transfer = nullptr;	//�������
};
