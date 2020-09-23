#include "GenBlockFile.h"

GenBlockFile::GenBlockFile(QObject *parent)
	: QThread(parent)
{
}

GenBlockFile::~GenBlockFile()
{
}

void GenBlockFile::run()
{
	getBasePath();
	prePareData();

	int column = 6;	//由ImageInfo的数据种类决定
	int row = m_content.size();
	if (row == 0)
	{
		emit signalFinished(false);
		return;
	}
	QList<QVariant> head;
	head << QStringLiteral("Name") << QStringLiteral("PhotogroupName") << QStringLiteral("Directory")
		<< QStringLiteral("Latitude") << QStringLiteral("Longitude") << QStringLiteral("Height");

	QVariantList vars;
	QList<QList<QVariant>> v;
	v.append(head);

	Q_FOREACH(Block content, m_content)
	{
		QList<QVariant> rowData;
		rowData.append(content.imageName);
		rowData.append(content.groupName);
		rowData.append(content.groupName);
		rowData.append(content.latitude);
		rowData.append(content.longitude);
		rowData.append(content.height);
		v.append(rowData);
	}
	for (int i = 0;i < row + 1; ++i)
	{
		vars.append(QVariant(v[i]));
	}
	QVariant res = QVariant(vars);

	writeExcel(m_file, row + 1, column, res, m_basePath);

#if 0
	ExcelOperator excel;
	excel.open(m_file);
	//[1]
	QAxObject *photoGroup = excel.addSheet("Photogroups");
	
	//[2]
	QAxObject *photos = excel.addSheet("Photos");
	int row = 1;
	int column = 1;
	excel.setCell(photos, row, column++, "Name");
	excel.setCell(photos, row, column++, "PhotogroupName");
	excel.setCell(photos, row, column++, "Directory");
	excel.setCell(photos, row, column++, "Latitude");
	excel.setCell(photos, row, column++, "Longitude");
	excel.setCell(photos, row, column++, "Height");

	Q_FOREACH(Block content, m_content)
	{
		row++;
		column = 1;
		excel.setCell(photos, row, column++, content.imageName);
		excel.setCell(photos, row, column++, content.groupName);
		excel.setCell(photos, row, column++, content.groupName);
		excel.setCell(photos, row, column++, content.latitude);
		excel.setCell(photos, row, column++, content.longitude);
		excel.setCell(photos, row, column++, content.height);
	}


	//[3]
	QAxObject *controlPoint = excel.addSheet("ControlPoint");
	excel.setCell(controlPoint, 1, 1, "Name");
	excel.setCell(controlPoint, 1, 2, "Easting");
	excel.setCell(controlPoint, 1, 3, "Northing");
	excel.setCell(controlPoint, 1, 4, "Height");

	//[4]
	QAxObject *options = excel.addSheet("Options");
	excel.setCell(options, 1, 1, "OptionName");
	excel.setCell(options, 1, 2, "Value");
	excel.setCell(options, 3, 1, "SRS");
	excel.setCell(options, 3, 2, "WGS84");
	excel.setCell(options, 4, 1, "InRadians");
	excel.setCell(options, 4, 2, "FALSE");
	excel.setCell(options, 5, 1, "BaseImagePath");
	//excel.setCell(options, 4, 1, "WGS84");

	excel.close();
#endif

	emit signalFinished(true);
}

void GenBlockFile::setFileAndImages(QString file, QMap<int, QMap<int, QList<ImageInfo>>> images)
{
	m_file = file;
	m_images = images;
}

void GenBlockFile::setPosAndSorties(QMap<int, QList<PosInfo>> posData, QList<int> sorties)
{
	m_posData = posData;
	m_sorties = sorties;
}

QList<int> GenBlockFile::getEqualSortie()
{
	QList<int> equals;
	Q_FOREACH(int i, m_sorties)
	{
		bool same = true;
		QMapIterator<int, QMap<int, QList<ImageInfo>>> iter(m_images);
		while (iter.hasNext())
		{
			iter.next();
			if (iter.value().value(i).size() != m_posData.value(i).size())
			{
				same = false;
			}
		}
		if (same)
		{
			equals.append(i);
		}
	}
	return equals;
}

void GenBlockFile::prePareData()
{
	QList<int> equals = getEqualSortie();
	int camIndex = 0;
	QMapIterator<int, QMap<int, QList<ImageInfo>>> iter(m_images);
	while (iter.hasNext())
	{
		iter.next();
		camIndex++;
		QString strCamIndex = QString::number(camIndex);
		QMap<int, QList<ImageInfo>> cam = iter.value();
		Q_FOREACH(int sort, equals)
		{
			QList<ImageInfo> images = cam[sort];
			QList<PosInfo> pos = m_posData[sort];
			for (size_t k = 0; k < pos.size(); k++)
			{
				Block block;
				block.imageName = images.at(k).imageName;
				block.groupName = strCamIndex;
				block.latitude = QString::number(pos.at(k).latitude);
				block.longitude = QString::number(pos.at(k).longitude);
				block.height = QString::number(pos.at(k).altitude);
				m_content.append(block);
			}
		}
	}
}

void GenBlockFile::writeExcel(QString file, int row, int column, QVariant &var, QString basePath)
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
	worksheet->setProperty("Name","Photos");
	//worksheet->setObjectName("Photos");
	//设置输出范围
	QString range = "A1:";

	char firstLetter = ' ';
	if (column / 26)
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

	int count = worksheets->property("Count").toInt();  //获取工作表数目
	QAxObject *pLastSheet = worksheets->querySubObject("Item(int)", count);
	auto optionSheet = worksheets->querySubObject("Add(QVariant)", pLastSheet->asVariant());
	pLastSheet->dynamicCall("Move(QVariant)", optionSheet->asVariant());
	optionSheet->setProperty("Name", "Options");  //设置工作表名称

	writeOptions(optionSheet, basePath);

	workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(file));

	workbook->dynamicCall("Close(Boolean)", false);
	excel->dynamicCall("Quit(void)");
}

void GenBlockFile::writeOptions(QAxObject *optionSheet, QString path)
{
	writeCell(optionSheet, 1, 1, "OptionName");
	writeCell(optionSheet, 1, 2, "Value");
	writeCell(optionSheet, 3, 1, "SRS");
	writeCell(optionSheet, 3, 2, "WGS84");
	writeCell(optionSheet, 4, 1, "InRadians");
	writeCell(optionSheet, 4, 2, "FALSE");
	writeCell(optionSheet, 5, 1, "BaseImagePath");
	writeCell(optionSheet, 5, 2, path);
}

void GenBlockFile::writeCell(QAxObject *sheet, int row, int column, QString value)
{
	QAxObject* pCell = sheet->querySubObject("Cells(int, int)", row, column);
	pCell->setProperty("Value", value);
}

void GenBlockFile::getBasePath()
{
	if (!m_images.isEmpty())
	{
		if (!m_images.first().isEmpty())
		{
			if (!m_images.first().first().isEmpty())
			{
				ImageInfo info = m_images.first().first().first();
 				if (info.fileName.contains("MSDCF"))
 				{
 					int index = info.fileName.lastIndexOf("MSDCF");
 					QString path = info.fileName.left(index);
 					index = path.lastIndexOf('/');
 					path = path.left(index);
 					index = path.lastIndexOf('/');
 					m_basePath = path.left(index);
 				}
 				else
				{
					int index = info.fileName.lastIndexOf('/');
					QString path = info.fileName.left(index);
					index = path.lastIndexOf('/');
					m_basePath = path.left(index);
				}
			}
		}
	}
}
