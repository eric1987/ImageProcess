#include "ExcelOperator.h"

#include <QAxObject>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

ExcelOperator::ExcelOperator(QObject *parent)
	: QObject(parent)
{
}

ExcelOperator::~ExcelOperator()
{
}

void ExcelOperator::ReadExcel(QString file)
{
	auto excel = new QAxObject("Excel.Application");
	excel->dynamicCall("SetVisible(bool Visible)", "false");
	excel->setProperty("DisplayAlerts", false);
	//excel->dynamicCall("Quit(void)");

	//获取工作薄(excel文件)集合
	auto workbooks = excel->querySubObject("Workbooks");

	workbooks->dynamicCall("Add");

	auto workbook = workbooks->querySubObject("Open(const QString&)", file);
	auto sheets = workbook->querySubObject("Worksheets");
	auto sheet = sheets->querySubObject("Item(int)", 1);

	// read the first cells in row 1..5
	for (int r = 1; (r <= 5); ++r)
	{
		auto cCell = sheet->querySubObject("Cells(int,int)", r, 1);
		qDebug() << cCell->dynamicCall("Value()").toInt();
	}
}

void ExcelOperator::WritePhotoInfo(QString file, QList<ImageInfo> &datas)
{
	int column = 4;	//由ImageInfo的数据种类决定
	int row = datas.size();

	QList<QVariant> head;
	head << QStringLiteral("时间") << QStringLiteral("光圈") << QStringLiteral("快门速度")
		<< QStringLiteral("ISO");


	QVariantList vars;
	QList<QList<QVariant>> v;
	v.append(head);

	for (int i = 0; i < row; i++)
	{
		QList<QVariant> rowData;
		rowData.append(datas[i].time);
		rowData.append(datas[i].aperture);
		rowData.append(datas[i].shutterspeed);
		rowData.append(datas[i].iso);

		v.append(rowData);
	}

	for (int i = 0;i < row + 1; ++i)
	{
		vars.append(QVariant(v[i]));
	}
	QVariant res = QVariant(vars);

	WriteExcel(file, row + 1, column, res);
}

void ExcelOperator::WritePosInfo(QString file, QList<PosInfo> &datas)
{
	int column = 10;		//由PosInfo的数据种类决定
	int row = datas.size();
	
	QList<QVariant> head;
	head << QStringLiteral("序号") << QStringLiteral("时间") << QStringLiteral("纬度")
		<< QStringLiteral("经度") << QStringLiteral("高度") << QStringLiteral("俯仰")
		<< QStringLiteral("滚转") << QStringLiteral("航偏角") << QStringLiteral("航向")
		<< QStringLiteral("地速");

	QVariantList vars;
	QList<QList<QVariant>> v;
	v.append(head);
	
	for (int i = 0; i < row; i++)
	{
		QList<QVariant> rowData;
		rowData.append(QString::number(datas[i].index));
		//rowData.append(datas[i].time);
		rowData.append(QString::number(datas[i].timestamp));
		rowData.append(QString::number(datas[i].latitude));
		rowData.append(QString::number(datas[i].longitude));
		rowData.append(QString::number(datas[i].altitude));
		rowData.append(QString::number(datas[i].pitching));
		rowData.append(QString::number(datas[i].rolling));
		rowData.append(QString::number(datas[i].driftAngle));
		rowData.append(QString::number(datas[i].heading));
		rowData.append(QString::number(datas[i].groundSpeed));
		
		v.append(rowData);
	}

	for (int i = 0;i < row+1; ++i)
	{
		vars.append(QVariant(v[i]));
	}
	QVariant res = QVariant(vars);

	WriteExcel(file, row+1, column, res);
}

void ExcelOperator::writePosAndImage(QString file, QList<ImageInfo> &imageData, QList<PosInfo> &posData)
{
	int column = 4;	//由ImageInfo的数据种类决定
	int row = posData.size();

	/*QList<QVariant> head;
	head << QStringLiteral("时间") << QStringLiteral("光圈") << QStringLiteral("快门速度")
		<< QStringLiteral("ISO");


	QVariantList vars;
	QList<QList<QVariant>> v;
	v.append(head);

	for (int i = 0; i < row; i++)
	{
		QList<QVariant> rowData;
		rowData.append(datas[i].time);
		rowData.append(datas[i].aperture);
		rowData.append(datas[i].shutterspeed);
		rowData.append(datas[i].iso);

		v.append(rowData);
	}

	for (int i = 0;i < row + 1; ++i)
	{
		vars.append(QVariant(v[i]));
	}
	QVariant res = QVariant(vars);

*/
}

void ExcelOperator::WriteExcel(QString file, int row, int column, QVariant &var)
{
	auto excel = new QAxObject(this);
	excel->setControl("Excel.Application");
	excel->dynamicCall("SetVisible(boo Visible)", "false");
	excel->setProperty("DisplayAlerts", false);

	auto workbooks = excel->querySubObject("WorkBooks");
	if (nullptr == workbooks)
	{
		int x = 0;
	}
	workbooks->dynamicCall("Add");
	auto workbook = excel->querySubObject("ActiveWorkBook");

	auto worksheets = workbook->querySubObject("Sheets");
	auto worksheet = worksheets->querySubObject("Item(int)", 1);

	//设置输出范围
	QString range = "A1:";
	
	char firstLetter = ' ';
	if (column/26)
	{
		firstLetter = 'A' + column / 26 - 1;
	}
	if (firstLetter == ' ')
	{
		QChar secLetter = column % 26 - 1 + 'A';
		range += QString(firstLetter) + QString(secLetter) + QString::number(row);
	}
	else
	{
		QChar letter = column % 26 - 1 + 'A';
		range += QString(letter) + QString::number(row);
	}
	auto user_range = worksheet->querySubObject("Range(const QString&)", range);


	//写入存储值
	user_range->setProperty("Value", var);
	workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(file));

	workbook->dynamicCall("Close(Boolean)", false);
	excel->dynamicCall("Quit(void)");
}

void ExcelOperator::CastListListVariant2Variant(QList<QList<QVariant>> &cells, QVariant &res)
{
	QVariantList vars;
	const int rows = cells.size();
	for (int i = 0;i < rows; ++i)
	{
		vars.append(QVariant(cells[i]));
	}
	res = QVariant(vars);
}
