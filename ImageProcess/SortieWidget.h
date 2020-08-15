#pragma once

#include <QWidget>
#include <QTableView>

#include "ui_SortieWidget.h"
#include "TableModel.h"

class SortieWidget : public QWidget
{
	Q_OBJECT

public:
	SortieWidget(QWidget *parent = Q_NULLPTR);
	~SortieWidget();

	/**
	* @brief ��ʾ�ܴ���Ϣ. disks ΪӰ�����ݣ� posΪpos���ݣ�num���ܴα��
	* @return true��Ӱ����pos����һ�£�false��Ӱ����pos���ݲ�һ��
	*/
	bool showDetail(QMap<QString, UDisk *> *disks, QList<PosInfo> pos, int num);

	void setProgress(int percent);

private:
	//�жϼܴ�Ӱ����pos�����Ƿ�һ��
	bool comparisonImageAndPos(QMap<QString, QList<ImageInfo>> images, QList<PosInfo> pos);

	//��ʾ�����ͼƬ
	void slotShowPic(QModelIndex index);

	//��ʼ��
	void init();

private:
	Ui::SortieWidget ui;

	TableModel *m_model = nullptr;
};
