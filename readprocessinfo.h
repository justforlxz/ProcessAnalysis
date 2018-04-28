#ifndef READPROCESSINFO_H
#define READPROCESSINFO_H

#include "process.h"

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>

class ReadProcessInfo : public QObject
{
    Q_OBJECT
public:
    explicit ReadProcessInfo(QObject *parent = nullptr);

public:
    void onTimeOut();

    QStringList currentList;
    std::list<Process*> m_processList;
    QTimer *m_timer;
    QJsonObject m_jsonObj;
    QJsonArray m_jsonArray;
};

#endif // READPROCESSINFO_H
