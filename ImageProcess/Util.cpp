#include <QFileDialog>
#include <QStandardPaths>
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

QStringList Util::getPosFile()
{
	QFileDialog *fileDialog = new QFileDialog;
	fileDialog->setWindowTitle(QStringLiteral("ѡ��pos�ļ�"));
	fileDialog->setNameFilter(tr("Images(*.txt)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	fileDialog->setViewMode(QFileDialog::Detail);
	fileDialog->setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
	QStringList fileNames;
	if (fileDialog->exec())
	{
		fileNames = fileDialog->selectedFiles();
	}
	return fileNames;
}

bool Util::compareImageAndPos(QMap<QString, int> imageSize, int posSize)
{
	QMapIterator<QString, int> iter(imageSize);
	while (iter.hasNext())
	{
		iter.next();
		if (iter.value() != posSize)
		{
			return false;
		}
	}
	return true;
}
