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
#include <QTextCodec>
void SortieWidget::slotShowPic(QModelIndex index)
{
	//QModelIndex ind = m_model->index(index.row(), index.column());
	//QVariant pic = m_model->data(ind, Qt::UserRole+1).value<QVariant>();

	//QStandardItem *item = m_model->item(index.row(), index.column());
	QString strPic = index.data(Qt::UserRole+1).toString();
	QPixmap pix;
	QImageReader r(strPic);
	r.setDecideFormatFromContent(true);
	QImage i = r.read();
	if (!i.isNull())
		pix = QPixmap::fromImage(i);
	/*bool ret = pix.load(strPic);
	if (!ret)
	{
		Log::INFO(QStringLiteral("使用pixmap打开影像文件失败：%1").arg(strPic));
		QImage img;
		ret = img.load(strPic);
		if (!ret)
		{
			Log::INFO(QStringLiteral("使用image打开影像文件失败：%1").arg(strPic));
		}
		else
		{
			pix = QPixmap::fromImage(img);
		}
	}*/
	
	QPixmap scaledImg = pix.scaledToWidth(ui.imageLabel->width());

	ui.imageLabel->clear();
	ui.imageLabel->setPixmap(scaledImg);
}

void SortieWidget::init()
{
	m_model = new TableModel;

	connect(ui.tableView, &QTableView::clicked, this, &SortieWidget::slotShowPic);
}
