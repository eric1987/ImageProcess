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

	//初始化model
	void initModel(QMap<QString, QList<ImageInfo>> images, QList<PosInfo> pos);

private:
	//初始化一行
	QList<QStandardItem*> initOneRow(QMap<QString, ImageInfo> images, PosInfo pos);
	
	Qt::Alignment m_defaultAlign;	//默认排列方式
	QStringList m_headList;			//表格标题栏

};
