#include "process.h"

#include <QFile>

Process::Process()
{
    m_time = QDateTime::currentDateTime();
}

bool Process::operator==(const Process &process)
{
    return pid == process.pid;
}

bool Process::operator==(Process *process)
{
    return pid == process->pid;
}

QDebug &operator <<(QDebug debug, const Process &process)
{
    debug << QString("{pid=%1,name=%2,startTime=%3}").arg(process.pid).arg(process.process_name).arg(process.m_time.toString("yyyy-MM-dd hh:mm:ss"));
    return debug;
}
