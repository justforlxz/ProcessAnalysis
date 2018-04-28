#include <QCoreApplication>

#include "readprocessinfo.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ReadProcessInfo info;

    return a.exec();
}
