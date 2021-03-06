#ifndef __LOGEMITTER_H__
#define __LOGEMITTER_H__


#include <QObject>
#include <QString>
#include <stdarg.h>

class LogEmitter: public QObject
{
    Q_OBJECT
public:
    void log(const char *type, const char *tag, char *fmt, va_list ap);
signals:
    void logstring(QString);
};

#endif
