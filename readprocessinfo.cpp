#include "readprocessinfo.h"

#include <QDir>
#include <QDebug>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QJsonDocument>

ReadProcessInfo::ReadProcessInfo(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000 * 1);
    connect(m_timer, &QTimer::timeout, this, &ReadProcessInfo::onTimeOut);
    m_timer->setSingleShot(true);
    m_timer->start();
}

void ReadProcessInfo::onTimeOut()
{
    QDir Dir("/proc/");
    QFileInfoList fileInfoListInSplDir = Dir.entryInfoList(QDir::Dirs);
    QFileInfo tempFileInfo;
    QStringList pids;
    std::list<Process*> currentList;

    for (Process *process : m_processList) {
        pids << process->pid;
    }

    foreach (tempFileInfo, fileInfoListInSplDir) {
        const QString &f = tempFileInfo.completeBaseName();

        QRegularExpression reg("^\\d+$");

        if (reg.match(f).hasMatch()) {
            QFile file(tempFileInfo.absoluteFilePath() + QDir::separator() + "comm");
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                const QString s = file.readAll();
                if (!s.isEmpty()) {
                    Process *p = new Process;
                    p->process_name = s.simplified();
                    p->pid = f;

                    // 追加新增进程
                    if (!pids.contains(f)) {
                        qDebug() << "进程: " << p->process_name << " 已被添加!";
                        m_processList.push_back(p);
                        pids << f;
                        QJsonObject obj;
                        obj["pid"] = p->pid;
                        obj["low"] = p->m_time.currentMSecsSinceEpoch();
                        obj["x"] = QString("start: %1, end: %2, name: %3").arg(p->m_time.toString("hh:mm:ss")).arg("").arg(p->process_name);
                        m_jsonObj[f] = obj;
                    }

                    currentList.push_back(p);
                }
            }
        }
    }

    pids.clear();

    for (Process *process : currentList) {
        pids << process->pid;
    }

    //计算已经不存在的进程
    for (std::list<Process*>::iterator it = m_processList.begin(); it != m_processList.end();) {
        const QString &pid = (*it)->pid;

        // 更新所有的进程退出时间
        QJsonObject obj = m_jsonObj[pid].toObject();
        obj["high"] = QDateTime::currentDateTime().currentMSecsSinceEpoch();
        obj["x"] = QString("start: %1, end: %2, name: %3").arg((*it)->m_time.toString("hh:mm:ss")).arg(QDateTime::currentDateTime().toString("hh::mm::ss")).arg((*it)->process_name);
        m_jsonObj[pid] = obj;
        m_jsonArray << obj;

        if (!pids.contains(pid)) {
            Process *p = *it;
            m_processList.erase(it++);
            currentList.remove(p);
            qDebug() << "进程: " << p->process_name << "已被删除!";
            delete p;
        } else {
            it++;
        }
    }

    const QString path = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first() + QDir::separator();

    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << dir.mkpath(path);
    }

    QFile file(path + "ProcessAnalysis.json");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(m_jsonObj).toJson());
        file.close();
    } else {
        Q_UNREACHABLE();
    }

    file.setFileName(path + "anychart.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(m_jsonArray).toJson());
        file.close();
    } else {
        Q_UNREACHABLE();
    }

    m_timer->start();

}
