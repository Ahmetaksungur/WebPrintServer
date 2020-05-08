#include "server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Server s;
    s.run();

    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
