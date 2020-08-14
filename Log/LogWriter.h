#ifndef LOG_WRITER_H
#define LOG_WRITER_H

#include <QRunnable>

class LogWriter : public QRunnable
{
public:
    LogWriter();
    ~LogWriter();

protected:
    void run();

private:
    QString CreateLogFileIfNotExists();
	QString AppDir();
	void CreateDirIfNotExists(QString &path);

private:
};

#endif
