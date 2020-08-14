#include "Logger.h"
#include "LogWriter.h"

#include <QThreadPool>
#include <QMutex>
#include <QMutexLocker>

extern QList<QString> _lLogContent;
extern QMutex _LogMutex;

Logger::Logger() : _pTimer(NULL)
{

}

Logger::~Logger()
{

}

void Logger::Start()
{
    _pTimer = new QTimer;
    _pTimer->setInterval(200);
    connect(_pTimer, SIGNAL(timeout()), this, SLOT(slotStore()));
    _pTimer->start();
}

void Logger::slotStore()
{
    QMutexLocker locker(&_LogMutex);
    int sz = _lLogContent.size();
    if (sz > 0)
	{
        LogWriter *pLogWriter = new LogWriter;
    	pLogWriter->setAutoDelete(true);
    	QThreadPool::globalInstance()->start(pLogWriter);
    }
}
