#ifndef PROCESS_H
#define PROCESS_H

#include <QString>
#include <QDebug>
#include <QDateTime>

class Process
{
public:
    Process();

    friend  QDebug& operator <<(QDebug debug, const Process &process);

    bool operator==(const Process &process);
    bool operator==(Process *process);

    QString pid;

    QString process_name;
    QDateTime m_time;
};

#endif // PROCESS_H
