#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QTimer>

class Logger : public QObject
{
    Q_OBJECT
public:
    Logger();
    ~Logger();
    
    void Start();
protected:
private slots:
    void slotStore();

private:
    QTimer *_pTimer;
};

#endif
