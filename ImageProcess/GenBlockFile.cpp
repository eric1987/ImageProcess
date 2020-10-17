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
	changeBlocksIntoExcelData();
#if 0
	if (m_genWay == LOCALWAY)
	{
	}
	else if(m_genWay == REPOSWAY)
	{
		createBlockByRepos();
	}
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
	emit signalFinished(true);
#endif
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

void GenBlockFile::setImages(QMap<int, QStringList> images)
{

}

void GenBlockFile::setPosFile(QStringList files)
{

}

void GenBlockFile::setGenWay(int type)
{
	m_genWay = type;
}

void GenBlockFile::setBlocks(QMap<int, QMap<int, QList<Block>>> blocks)
{
	m_blocks = blocks;
}

void GenBlockFile::setBasePath(QString path)
{
	m_basePath = path;
}

void GenBlockFile::setFile(QString file)
{
	m_file = file;
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

bool GenBlockFile::writeExcel(QString file, int row, int column, QVariant &var, QString basePath)
{
	QFile temp(m_template);
	bool copy = temp.copy(file);
	if (copy)
	{
		Log::INFO(QStringLiteral("生成文件成功。"));
	}
	else
	{
		Log::INFO(QStringLiteral("block文件复制失败。"));
		QMessageBox::warning(new QWidget, QStringLiteral("警告"), QStringLiteral("block文件复制失败, 确保生成文件路径下无block.xls文件。"));
		return false;
	}
	temp.close();

	auto excel = new QAxObject(this);
	excel->setControl("Excel.Application");
	excel->dynamicCall("SetVisible(boo Visible)", "false");
	excel->setProperty("DisplayAlerts", false);

	auto workbooks = excel->querySubObject("WorkBooks");
	if (nullptr == workbooks)
	{
		int x = 0;
	}
	workbooks->dynamicCall("Open (const QString&)", file); //filename
	//workbooks->dynamicCall("Add");
	//auto workbook = excel->querySubObject("Open(QString, QVariant)", file);
	QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
	QAxObject *worksheets = workbook->querySubObject("Sheets");
	QAxObject *pFirstSheet = worksheets->querySubObject("Item(QString)", "Photos");
	pFirstSheet->dynamicCall("delete");
	QAxObject *pSecSheet = worksheets->querySubObject("Item(QString)", "Options");
	pSecSheet->dynamicCall("delete");
	workbook->dynamicCall("Save(void)");
	//workbook->dynamicCall("Close (Boolean)", false);
	//excel->setProperty("DisplayAlerts", 1);
	//return;

	QAxObject *firstSheet = worksheets->querySubObject("Item(int)", 1);
	QAxObject *photoSheet = worksheets->querySubObject("Add(QVariant)", firstSheet->asVariant());
	firstSheet->dynamicCall("Move(QVariant)", photoSheet->asVariant());
	photoSheet->setProperty("Name", "Photos");  //设置工作表名称

	//worksheet->setProperty("Name","Photos");
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
	auto user_range = photoSheet->querySubObject("Range(const QString&)", range);
	//写入存储值
	user_range->setProperty("Value", var);
	/*workbook->dynamicCall("Save(void)");
	workbook->dynamicCall("Close(Boolean)", false);
	excel->dynamicCall("Quit(void)");
	delete excel;
	return;*/
	
	int count = worksheets->property("Count").toInt();  //获取工作表数目
	QAxObject *pLastSheet = worksheets->querySubObject("Item(int)", count);
	auto optionSheet = worksheets->querySubObject("Add(QVariant)", pLastSheet->asVariant());
	pLastSheet->dynamicCall("Move(QVariant)", optionSheet->asVariant());
	optionSheet->setProperty("Name", "Options");  //设置工作表名称
	writeOptions(optionSheet, basePath);
	//workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(file));
	workbook->dynamicCall("Save(void)");
	workbook->dynamicCall("Close(Boolean)", false);
	excel->dynamicCall("Quit(void)");
	delete excel;

	return true;
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

void GenBlockFile::changeBlocksIntoExcelData()
{
	int column = 6;	//由ImageInfo的数据种类决定
	QMapIterator<int, QMap<int, QList<Block>>> iter(m_blocks);
	int row = 0;
	QVariantList vars;
	QList<QList<QVariant>> v;
	QList<QVariant> head;
	head << QStringLiteral("Name") << QStringLiteral("PhotogroupName") << QStringLiteral("Directory")
		<< QStringLiteral("Latitude") << QStringLiteral("Longitude") << QStringLiteral("Height");
	v.append(head);
	while (iter.hasNext())
	{
		iter.next();
		QMapIterator<int, QList<Block>> iterSignal(iter.value());
		while (iterSignal.hasNext())
		{
			iterSignal.next();
			row += iterSignal.value().size();
			Q_FOREACH(Block block, iterSignal.value())
			{
				QList<QVariant> rowData;
				rowData.append(block.imageName);
				rowData.append(block.groupName);
				rowData.append(block.nickName);
				rowData.append(block.latitude);
				rowData.append(block.longitude);
				rowData.append(block.height);
				v.append(rowData);
			}
		}
	}
	if (row == 0)
	{
		emit signalFinished(false);
		return;
	}
	for (int i = 0;i < row + 1; ++i)
	{
		vars.append(QVariant(v[i]));
	}
	QVariant res = QVariant(vars);
	if (!writeExcel(m_file, row + 1, column, res, m_basePath))
	{
		return;
	}
	emit signalFinished(true);
}
