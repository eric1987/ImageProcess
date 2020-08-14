#include "Log.h"

#include <QString>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>

Logger *Log::_pLogger = NULL;

QList<QString> _lLogContent;
QMutex _LogMutex;

void Log::Start()
{
	_pLogger = new Logger;
	_pLogger->Start();
}

void Log::DEBUG(QString &content)
{
#ifdef _DEBUG
    QString t = GetTime();
    QString sHead = GetLineHead(IDEBUG, t);
    QString l = sHead + content + "\n";
    QMutexLocker locker(&_LogMutex);
    _lLogContent.append(l);
#endif
}

void Log::INFO(QString &content)
{
    QString t = GetTime();
    QString sHead = GetLineHead(IINFO, t);
    QString l = sHead + content + "\n";
    QMutexLocker locker(&_LogMutex);
    _lLogContent.append(l);
}

void Log::FAIL(QString &content)
{
    QString t = GetTime();
    QString sHead = GetLineHead(IERROR, t);
    QString l = sHead + content + "\n";
    QMutexLocker locker(&_LogMutex);
    _lLogContent.append(l);
}

QString Log::GetTime()
{
    QDateTime now = QDateTime::currentDateTime();
    QString ret = now.toString("yyyy-MM-dd hh:mm:ss zzz");
    return ret;
}

QString Log::GetLineHead(LOG_LEVEL ll, QString t)
{
    QString ret;
    switch (ll) {
        case IDEBUG:
        {
            ret = QString("[ DEBUG - ");
            ret += t;
            ret += QString(" ] : ");
        }
        break;
        case IINFO:
        {
            ret = QString("[ INFO  - ");
            ret += t;
            ret += QString(" ] : ");
        }
        break;
        case IERROR:
        {
            ret = QString("[ ERROR - ");
            ret += t;
            ret += QString(" ] : ");
        }
        break;
		default:
		break;
    }
	return ret;
}
