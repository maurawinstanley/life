#include "cellwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CellWindow w;
    w.show();

    return a.exec();
}
