#include "SortieWidget.h"

SortieWidget::SortieWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
}

SortieWidget::~SortieWidget()
{
}

bool SortieWidget::showDetail(QMap<QString, UDisk *> *disks, QList<PosInfo> pos, int num)
{
	QMap<QString, QList<ImageInfo>> images;
	for (QMap<QString, UDisk *>::const_iterator it = disks->begin(); it != disks->end(); it++)
	{
		images.insert(it.value()->cam, (*it)->imageData.value(num));
	}

	bool same = comparisonImageAndPos(images, pos);

	m_model->initModel(images, pos);
	ui.tableView->setModel(m_model);

	ui.tableView->setAlternatingRowColors(true);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	return same;
}

bool SortieWidget::showDetail(QMap<QString, QList<ImageInfo>> cams, QList<PosInfo> pos, int num)
{
	bool same = comparisonImageAndPos(cams, pos);
	m_model->initModel(cams, pos);
	ui.tableView->setModel(m_model);

	ui.tableView->setAlternatingRowColors(true);
	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	return same;
}

bool SortieWidget::comparisonImageAndPos(QMap<QString, QList<ImageInfo>> images, QList<PosInfo> pos)
{
	QMapIterator<QString, QList<ImageInfo>> iter(images);
	while (iter.hasNext())
	{
		iter.next();
		if (iter.value().size() != pos.size())
		{
			return false;
		}
	}
	return true;
}

void SortieWidget::slotShowPic(QModelIndex index)
{
	QModelIndex ind = m_model->index(index.row(), index.column());
	QString pic = m_model->data(ind, Qt::UserRole).value<QString>();
	Log::INFO(QStringLiteral("打开影像文件路径：%1").arg(pic));
	QPixmap img(pic);
	QPixmap scaledImg = img.scaledToWidth(ui.imageLabel->width());

	ui.imageLabel->clear();
	ui.imageLabel->setPixmap(scaledImg);
}

void SortieWidget::init()
{
	m_model = new TableModel;

	connect(ui.tableView, &QTableView::clicked, this, &SortieWidget::slotShowPic);
}
