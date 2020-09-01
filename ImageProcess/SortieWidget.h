#pragma once

#include <QWidget>
#include <QTableView>

#include "ui_SortieWidget.h"
#include "TableModel.h"
#include "Log/Log.h"

class SortieWidget : public QWidget
{
	Q_OBJECT

public:
	SortieWidget(QWidget *parent = Q_NULLPTR);
	~SortieWidget();

	/**
	* @brief 显示架次信息. disks 为影像数据， pos为pos数据，num：架次编号
	* @return true：影响与pos数据一致，false，影像与pos数据不一致
	*/
	bool showDetail(QMap<QString, UDisk *> *disks, QList<PosInfo> pos, int num);

	void setProgress(int percent);

private:
	//判断架次影像与pos数量是否一致
	bool comparisonImageAndPos(QMap<QString, QList<ImageInfo>> images, QList<PosInfo> pos);

	//显示点击的图片
	void slotShowPic(QModelIndex index);

	//初始化
	void init();

private:
	Ui::SortieWidget ui;

	TableModel *m_model = nullptr;
};
