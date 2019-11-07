#include "cellwindow.h"
#include "cell.h"
#include "ui_cellwindow.h"

#include <vector>
#include <string>
#include <QColor>
#include <QGraphicsItem>
#include <QDebug>
#include <QTimer>

CellWindow::CellWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CellWindow)
{
    ui->setupUi(this);

    // scene is a pointer field of plot window
    scene = new QGraphicsScene;

    //time_ = new QTimer(this);
    //speed_ = 1;
    //connect(time_, SIGNAL(timeout()), this, SLOT(onPlayButtonClicked()));
    //time_->start();

    // QGraphicsView is a container for a QGraphicsScene
    QGraphicsView * view = ui->graphicsView;
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());


    srand(time(0));
    cells_ = {};

    for (int col = 0; col < 10; col++) {
        std::vector<Cell*> cell_column = {};
        for (int row = 0; row < 20; row++) {
            int random = rand() % 10 + 1;
            QColor color = QColor(255,255,255);
            bool alive = false;
            if (random > 5) {
                color = QColor(255,20,147);
                alive = true;
                population_++;
            }
            Cell *c = new Cell(color, row*20, col*20, alive);

            AddCell(c);
            cell_column.push_back(c);
        }
        cells_.push_back(cell_column);
    }

    std::string s = "Population: " + std::to_string(population_);
    ui->populationLabel->setText(s.c_str());

    //connect(ui->stepButton, &QAbstractButton::pressed, this, &CellWindow::onPlayButtonClicked);
}

CellWindow::~CellWindow()
{
    delete ui;
}

void CellWindow::AddCell(Cell* c) {
    connect(c, &Cell::CellSelected, this, &CellWindow::CellClickedSlot);
    scene->addItem(c);
}

void CellWindow::on_stepButton_clicked(){
    //SimulateTurn();
    qDebug()<<"step button clicked";
    SimulateTurn();
}

void CellWindow::on_playButton_clicked(){
    qDebug()<<"play button clicked";
}
void CellWindow::on_pauseButton_clicked(){
    qDebug()<<"pause button clicked";
}


void CellWindow::CellClickedSlot(Cell *c) {
    if (c->is_alive()) {
        population_++;
    } else {
        population_--;
    }

    std::string s = "Population: " + std::to_string(population_);
    ui->populationLabel->setText(s.c_str());

}


void CellWindow::SimulateTurn(){
    turn_ct_ += 1;
    int accum = 0;
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

    //population_ += (revive.size() - kill.size());
    //population_ += accum;

    for (int i = 0; i < revive.size(); i++) {
        cells_[revive[i].first][revive[i].second]->set_color(QColor(255,0,147));
        cells_[revive[i].first][revive[i].second]->now_this_is_the_story_all_about_how_my_life_got_flipped_turned_upside_down();
        //scene->update();
    }

    for (int i = 0; i < kill.size(); i++) {
        cells_[kill[i].first][kill[i].second]->set_color(QColor(255,255,255));
        cells_[kill[i].first][kill[i].second]->now_this_is_the_story_all_about_how_my_life_got_flipped_turned_upside_down();
        //scene->update();
    }
    for (int row = 0; row<10; row++){
        for (int col = 0; col<20; col++){
            Cell* current = get_cell(row, col);
            if (current->is_alive()){
                accum ++;
            }
        }
    }
    population_ = accum;
    qDebug()<< "pop: ";
    qDebug() <<population_ ;
    scene->update();


    //this will call a function update graph
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

