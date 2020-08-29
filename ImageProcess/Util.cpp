#include "Util.h"

Util::Util(QObject *parent)
	: QObject(parent)
{
}

Util::~Util()
{
}

QString Util::getParentFloder(QString srcFile)
{
	QString pattern = "\\d{3}MSDCF";
	QRegExp reg;
	reg.setPattern(pattern);
	reg.indexIn(srcFile);
	QString folder = reg.cap(0);

	return folder.at(2);
}
