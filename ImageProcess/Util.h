#pragma once

#include <QObject>

class Util : public QObject
{
	Q_OBJECT

public:
	Util(QObject *parent);
	~Util();

	//��ȡӰ�������ĸ��ļ�����
	static QString getParentFloder(QString srcFile);
};
