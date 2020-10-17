#pragma once

#include <QObject>
#include <QMap>

class Util : public QObject
{
	Q_OBJECT

public:
	Util(QObject *parent);
	~Util();

	//获取影像是在哪个文件夹下
	static QString getParentFloder(QString srcFile);
	//获取pos文件
	static QStringList getPosFile();
	//比较pos文件与影像文件数量
	static bool compareImageAndPos(QMap<QString, int> imageSize, int posSize);
};
