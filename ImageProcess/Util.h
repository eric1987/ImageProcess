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
	//��ȡpos�ļ�
	static QStringList getPosFile();
	//�Ƚ�pos�ļ���Ӱ���ļ�����
	static bool compareImageAndPos(QMap<QString, int> imageSize, int posSize);
};
