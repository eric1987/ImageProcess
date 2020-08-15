#include "TableModel.h"

TableModel::TableModel(QObject *parent)
	: QStandardItemModel(parent)
{
	m_defaultAlign = Qt::AlignLeft | Qt::AlignVCenter;		//左对齐。重置居中
}

TableModel::~TableModel()
{
}

void TableModel::initModel(QMap<QString, QList<ImageInfo>> images, QList<PosInfo> pos)
{
	clear();
	if (pos.isEmpty())
	{
		return;
	}

	int size = images.size();
	m_headList.clear();
	for (QMap<QString, QList<ImageInfo>>::const_iterator it = images.begin(); it != images.end(); it++)
	{
		m_headList << QStringLiteral("相机%1").arg(it.key());
	}
	m_headList << QStringLiteral("纬度") << QStringLiteral("经度") 
		<< QStringLiteral("高度") << QStringLiteral("地速");
	setHorizontalHeaderLabels(m_headList);

	QMap<QString, ImageInfo> rowInfo;
	for (int m = 0; m < pos.size(); m++)
	{
		rowInfo.clear();
		for (QMap<QString, QList<ImageInfo>>::const_iterator it = images.begin(); it != images.end(); it++)
		{
			if (it.value().size() > m + 1)
			{
				rowInfo.insert(it.key(), it.value().at(m));
			}
			else
			{
				continue;
			}
		}

		QList<QStandardItem*> items = initOneRow(rowInfo, pos.at(m));

		if (!items.isEmpty())
		{
			appendRow(items);       //添加一行
		}
	}
}

QList<QStandardItem*> TableModel::initOneRow(QMap<QString, ImageInfo> images, PosInfo pos)
{
	QList<QStandardItem*> items;

	for (int n = 0; n < m_headList.size(); n++)
	{
		QString headStr = m_headList.at(n);

		QStandardItem *item = new QStandardItem;
		item->setTextAlignment(m_defaultAlign);

		if (QStringLiteral("纬度") == headStr)
		{
			item->setData(pos.latitude, Qt::DisplayRole);
		}
		else if (QStringLiteral("经度") == headStr)
		{
			item->setData(pos.longitude, Qt::DisplayRole);
		}
		else if (QStringLiteral("高度") == headStr)
		{
			item->setData(pos.altitude, Qt::DisplayRole);
		}
		else if (QStringLiteral("地速") == headStr)
		{
			item->setData(pos.groundSpeed, Qt::DisplayRole);
		}
		for (QMap<QString, ImageInfo>::const_iterator it = images.begin(); it != images.end(); it++)
		{
			if (QStringLiteral("相机%1").arg(it.key()) == headStr)
			{
				item->setData(it->imageName, Qt::DisplayRole);	//显示照片名称
				item->setData(it->fileName, Qt::UserRole);		//照片路径名
			}
		}

		item->setToolTip(item->data(Qt::EditRole).toString());

		items << item;
	}

	return items;
}
