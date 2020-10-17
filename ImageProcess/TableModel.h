#pragma once

#include <QStandardItemModel>
#include <QTableWidgetItem>

#include "Common.h"

class TableModel : public QStandardItemModel
{
	Q_OBJECT

public:
	TableModel(QObject *parent = Q_NULLPTR);
	~TableModel();

	//��ʼ��model
	void initModel(QMap<QString, QList<ImageInfo>> images, QList<PosInfo> pos);

private:
	//��ʼ��һ��
	QList<QStandardItem*> initOneRow(QMap<QString, ImageInfo> images, PosInfo pos);
	
	Qt::Alignment m_defaultAlign;	//Ĭ�����з�ʽ
	QStringList m_headList;			//��������

};
