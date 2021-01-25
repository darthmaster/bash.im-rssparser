#include <QCoreApplication>
#include "rss.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    RSS worker;
    worker.getRssPage("https://bash.im/rss/");
    return a.exec();
}
