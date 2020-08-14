#include "LogWriter.h"

#include <QString>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QtWidgets/QApplication>

extern QList<QString> _lLogContent;
extern QMutex _LogMutex;

LogWriter::LogWriter()
{

}

LogWriter::~LogWriter()
{

}

void LogWriter::run()
{
    QString path = CreateLogFileIfNotExists();

	QFile f(path);
	if (f.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		QMutexLocker locker(&_LogMutex);
		QTextStream ts(&f);
		for each(QString s in _lLogContent)
		{
			ts << QString(s.toStdString().c_str()) << endl;
			ts.flush();
		}
		_lLogContent.clear();
		f.close();
	}
}

QString LogWriter::CreateLogFileIfNotExists()
{
    //创建日志根目录
    QString sYMD = QDate::currentDate().toString("yyyyMMdd");
    QString path = AppDir();
    path.append("\\Log");
    CreateDirIfNotExists(path);

    path.append("\\");
    path.append(sYMD);
    path.append(".log");

    return path;
}

QString LogWriter::AppDir()
{
	QString ret = QApplication::applicationDirPath();
	return ret;
}

void LogWriter::CreateDirIfNotExists(QString &path)
{
	QDir dir(path);
	if (!dir.exists())
	{
		dir.mkpath(path);
	}
}
