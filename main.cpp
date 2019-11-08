#include "cellwindow.h"
#include <QApplication>
/**
Maura Winstanley
Matthew Donovan
Homework 4
Program to play Conway's game of life.
To run: QMake and then play button.
Extra features:
    choose button random color to create a new random color
    drop down to select the ratio of neighbors at which the cell will die at

*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CellWindow w;
    w.show();

    return a.exec();
}
