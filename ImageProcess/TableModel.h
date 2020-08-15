#pragma once

#include <QStandardItemModel>

#include "Common.h"

class TableModel : public QStandardItemModel
{
	Q_OBJECT

public:
	TableModel(QObject *parent = Q_NULLPTR);
	~TableModel();

	void initModel(QMap<QString, QList<ImageInfo>> images, QList<PosInfo> pos);

private:
	QList<QStandardItem*> initOneRow(QMap<QString, ImageInfo> images, PosInfo pos);
	
	Qt::Alignment m_defaultAlign;
	QStringList m_headList;			//表格标题栏
	int m_index = 0;

};
