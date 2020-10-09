#pragma once

#include <QObject>
#include <QMap>

class Util : public QObject
{
	Q_OBJECT

public:
	Util(QObject *parent);
	~Util();

	//��ȡӰ�������ĸ��ļ�����
	static QString getParentFloder(QString srcFile);

	static QStringList getPosFile();
	static bool compareImageAndPos(QMap<QString, int> imageSize, int posSize);
};
