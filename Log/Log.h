#ifndef LOG_H
#define LOG_H

#include "log_global.h"

#include "Logger.h"

#include <QObject>

enum LOG_LEVEL
{
	IDEBUG,
	IINFO,
	IERROR
};

class LOG_EXPORT Log : public QObject
{
public:
	static void Start();

	static void DEBUG(QString &content);
	static void INFO(QString &content);
	static void FAIL(QString &content);

private:

	static QString GetTime();
	static QString GetLineHead(LOG_LEVEL ll, QString t);


protected:

private:
	static Logger *_pLogger;
};

#endif
