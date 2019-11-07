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

//will call GetNeighbors and then create it will create
//two vectors - one that keeps track of cells to flip from alive to dead
// and one that keep track of cells to flip from dead to alive
void CellWindow::SimulateTurn(){
    this->turn_ct_ +=1;
    std::vector<std::pair<int,int>> alive_dead;
    std::vector<std::pair<int,int>> dead_alive;
    for (int i = 0; i<20; i++){
        for (int j = 0; j<10; j++){
            int alive_neighbors = GetNeighbors(i,j);
            if (cells_[i][j]->is_alive()){ //alive
                if (alive_neighbors<2){
                    std::pair <int, int> alive_to_dead;
                    alive_to_dead.first = i;
                    alive_to_dead.second = j;
                    alive_dead.push_back(alive_to_dead);
                } else if (alive_neighbors>3){
                    std::pair <int, int> alive_to_dead;
                    alive_to_dead.first = i;
                    alive_to_dead.second = j;
                    alive_dead.push_back(alive_to_dead);
                } //else: has 2 or three neighbors exactly and stays alive
            } else { //dead
                if (alive_neighbors == 3){
                    std::pair <int, int> dead_to_alive;
                    dead_to_alive.first = i;
                    dead_to_alive.second = j;
                    dead_alive.push_back(dead_to_alive);
                }
            }
        }
    }
    population_ += dead_alive.size();
    population_ -= alive_dead.size();


}

int CellWindow::GetNeighbors(int row, int col){ // will return the count of number alive neighbors
    std::vector<int> rows = {-1, 0, 1, -1, 1, -1, 0, 1};
    std::vector<int> cols = {-1, -1, -1, 0, 0, 1, 1, 1};

    int neighbors_alive = 0;

    for (int i = 0; i<8; i++){
        if (cells_[row + rows[i]][col + cols[i]]){
            if (cells_[row + rows[i]][col + cols[i]]->is_alive()){
                neighbors_alive += 1;
            }
        }
    }
    return neighbors_alive;
}

