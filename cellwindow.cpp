#include "cellwindow.h"
#include "ui_cellwindow.h"

CellWindow::CellWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CellWindow)
{
    ui->setupUi(this);
}

CellWindow::~CellWindow()
{
    delete ui;
}
