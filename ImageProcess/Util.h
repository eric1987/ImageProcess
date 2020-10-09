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

	static QStringList getPosFile();
	static bool compareImageAndPos(QMap<QString, int> imageSize, int posSize);
};
