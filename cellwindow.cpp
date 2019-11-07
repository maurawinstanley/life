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
    cells_ = {};

    for (int row = 0; row < 10; row++) {
        std::vector<Cell*> cell_column = {};
        for (int col = 0; col < 20; col++) {
            int random = rand() % 10 + 1;
            QColor color = QColor(255,255,255);
            bool alive = false;
            if (random > 5) {
                color = QColor(255,20,147);
                alive = true;
                population_++;
            }
            Cell *c = new Cell(color, col*20, row*20, alive);

            AddCell(c);
            cell_column.push_back(c);
        }
        //qDebug() << col[0]->is_alive() << ", " << col[1]->is_alive() << ", " << col[2]->is_alive() << ", " << col[3]->is_alive() << ", " << col[4]->is_alive() << ", " << col[5]->is_alive() << ", " << col[6]->is_alive() << ", " << col[7]->is_alive() << ", " << col[8]->is_alive() << ", " << col[9]->is_alive();
        cells_.push_back(cell_column);
    }
    //qDebug() << get_cell(0,19)->is_alive();

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
    if (c->is_alive()) {
        population_++;
    } else {
        population_--;
    }

    std::string s = "Population: " + std::to_string(population_);
    ui->populationLabel->setText(s.c_str());
    SimulateTurn();
}


void CellWindow::SimulateTurn(){
    turn_ct_ += 1;

    std::vector<std::pair<int, int>> kill;
    std::vector<std::pair<int, int>> revive;

    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 20; col++) {
            int surrounding_population = GetNeighbors(row, col);
            Cell* current = get_cell(row, col);

            if (current->is_alive()) {
                if (surrounding_population < 2 || surrounding_population > 3) {
                    kill.push_back({row,col});
                }
            } else {
                if (surrounding_population == 3) {
                    revive.push_back({row,col});
                }
            }
        }
    }

    population_ += (revive.size() - kill.size());

    for (int i = 0; i < revive.size(); i++) {
        cells_[revive[i].first][revive[i].second]->set_color(QColor(255,0,147));
        cells_[revive[i].first][revive[i].second]->flip_vivality();
        scene->update();
    }

    for (int i = 0; i < kill.size(); i++) {
        cells_[kill[i].first][kill[i].second]->set_color(QColor(255,255,255));
        cells_[kill[i].first][kill[i].second]->flip_vivality();
        scene->update();
    }
}


int CellWindow::GetNeighbors(int row, int col) {

    int neighbors = 0;

    int row_above = row-1;
    int row_below = row+1;
    int col_before = col-1;
    int col_after = col+1;

    if (row - 1 < 0) {row_above = cells_.size()-1;}
    if (row + 1 > 9) {row_below = 0;}
    if (col - 1 < 0) {col_before = cells_[0].size()-1;}
    if (col + 1 > 19) {col_after = 0;}

    if (cells_[row_above][col_before]->is_alive()) {neighbors++;}
    if (cells_[row_above][col]->is_alive()) {neighbors++;}
    if (cells_[row_above][col_after]->is_alive()) {neighbors++;}

    if (cells_[row][col_before]->is_alive()) {neighbors++;}
    if (cells_[row][col_after]->is_alive()) {neighbors++;}

    if (cells_[row_below][col_before]->is_alive()) {neighbors++;}
    if (cells_[row_below][col]->is_alive()) {neighbors++;}
    if (cells_[row_below][col_after]->is_alive()) {neighbors++;}

    return neighbors;

}

