#include "cellwindow.h"
#include "cell.h"
#include "ui_cellwindow.h"

#include <vector>
#include <QColor>
#include <QGraphicsItem>

CellWindow::CellWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CellWindow)
{
    ui->setupUi(this);

    // scene is a pointer field of plot window
    scene = new QGraphicsScene;



    // QGraphicsView is a container for a QGraphicsScene
    QGraphicsView * view = ui->graphicsView;
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());


    srand(time(0));

    for (int i = 0; i < 20; i++) {
        std::vector<Cell*> row;
        for (int j = 0; j < 10; j++) {
            int random = rand() % 10 + 1;
            QColor color(255, 255, 255);
            if (random > 5) {
                color = QColor(255,20,147);
            }
            Cell *c = new Cell(color, i*20, j*20);
            scene->addItem(c);

            // the two lines below this took me so long to figure out. i'm pissed. why is this the only way it works
            row.push_back(c);
        }
        cells_.push_back(row);
    }
}

CellWindow::~CellWindow()
{
    delete ui;
}

