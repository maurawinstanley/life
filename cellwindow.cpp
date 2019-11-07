#include "cellwindow.h"
#include "cell.h"
#include "ui_cellwindow.h"

#include <vector>
#include <string>
#include <QColor>
#include <QGraphicsItem>
#include <QDebug>

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
            QColor color = QColor(255,255,255);
            bool alive = false;
            if (random > 5) {
                color = QColor(255,20,147);
                alive = true;
                population_++;
            }
            Cell *c = new Cell(color, i*20, j*20, alive);

            AddCell(c);

            // the two lines below this took me so long to figure out. i'm pissed. why is this the only way it works
            row.push_back(c);
        }
        cells_.push_back(row);
    }
    std::string s = "Population: " + std::to_string(population_);
    ui->populationLabel->setText(s.c_str());
}

CellWindow::~CellWindow()
{
    delete ui;
}

void CellWindow::AddCell(Cell* c) {
    connect(c, &Cell::CellSelected, this, &CellWindow::CellClickedSlot);
    scene->addItem(c);
}

void CellWindow::CellClickedSlot(Cell *c) {
    qDebug() << "I'm here";
    if (c->is_alive()) {
        population_++;
    } else {
        population_--;
    }

    std::string s = "Population: " + std::to_string(population_);
    ui->populationLabel->setText(s.c_str());
}
