#pragma once
#include <QObject>

#include "Common.h"

class ExcelOperator : public QObject
{
	Q_OBJECT

public:
	ExcelOperator(QObject *parent = 0);
	~ExcelOperator();

	void ReadExcel(QString file);
	
	void WritePhotoInfo(QString file, QList<ImageInfo> &datas);
	void WritePosInfo(QString file, QList<PosInfo> &datas);

	void writePosAndImage(QString file, QList<ImageInfo> &imageData, QList<PosInfo> &posData);

private:
	void WriteExcel(QString file, int row, int column, QVariant &var);

	void CastListListVariant2Variant(QList<QList<QVariant>> &cells, QVariant &res);
};
