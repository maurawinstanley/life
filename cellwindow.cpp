#include "cellwindow.h"
#include "cell.h"
#include "ui_cellwindow.h"

#include <vector>
#include <string>
#include <QColor>
#include <QGraphicsItem>
#include <QDebug>
#include <QTimer>

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QChart>

#include <QtCharts/QChartView>
CellWindow::CellWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CellWindow)
{
    ui->setupUi(this);

    ui->comboBox->addItem("0");
    ui->comboBox->addItem("1");
    ui->comboBox->addItem("2");
    ui->comboBox->addItem("3");
    ui->comboBox->addItem("4");
    ui->comboBox->addItem("5");
    ui->comboBox->addItem("6");
    ui->comboBox->addItem("7");
    ui->comboBox->addItem("8");

    // scene is a pointer field of plot window
    scene = new QGraphicsScene;
    graph_scene = new QGraphicsScene;
    time_ = new QTimer(this);
    speed_ = 1000; //start at 1000 ms intervals
    connect(time_, SIGNAL(timeout()), this, SLOT(on_playButton_clicked()));


    // QGraphicsView is a container for a QGraphicsScene
    QGraphicsView * view = ui->graphicsView;
    view2 = ui->graphicsView_2;
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());
    view2->setScene(graph_scene);
    view2->setSceneRect(0,0,view2->frameSize().width(),view2->frameSize().height());

    srand(time(0));
    //view2->show();

    NewGame();
    //connect(ui->stepButton, &QAbstractButton::pressed, this, &CellWindow::on_playButton_clicked);
    //connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(setValue(int)));
    //connect(ui->horizontalSlider, SIGNAL)
    UpdateGraph();
}

CellWindow::~CellWindow()
{
    delete ui;
}

void CellWindow::NewGame() {
    reproduction_requirement_ = 3;
    ui->comboBox->setCurrentIndex(3);

    cells_ = {};
    Cell::set_alive_color(QColor(255,0,147));
    for (int col = 0; col < 10; col++) {
        std::vector<Cell*> cell_column = {};
        for (int row = 0; row < 20; row++) {
            int random = rand() % 10 + 1;
            QColor color = Cell::get_dead_color();
            bool alive = false;
            if (random > 5) {
                color = Cell::get_alive_color();
                alive = true;
                population_++;
            }
            Cell *c = new Cell(color, row*20, col*20, alive);

            AddCell(c);
            cell_column.push_back(c);
        }
        cells_.push_back(cell_column);
    }

    std::string s = "Pop: " + std::to_string(population_) + "(" + std::to_string(population_/200) + ")";

    ui->populationLabel->setText(s.c_str());

    std::string speed = "Speed: " + std::to_string(1);

    ui->speedLabel->setText(speed.c_str());
    scene->update();

}

void CellWindow::AddCell(Cell* c) {
    connect(c, &Cell::CellSelected, this, &CellWindow::CellClickedSlot);
    scene->addItem(c);

}

void CellWindow::on_stepButton_clicked(){

    //qDebug()<<"step button clicked";
    SimulateTurn();
}

void CellWindow::on_playButton_clicked(){
    //Debug()<<"play button clicked";
    time_->start(speed_);
    SimulateTurn();
    scene->update();

}
void CellWindow::on_pauseButton_clicked(){
    //qDebug()<<"pause button clicked";
    time_->stop();
}


void CellWindow::CellClickedSlot(Cell *c) {

    if (c->is_alive()) {
        c->now_this_is_the_story_all_about_how_my_life_got_flipped_turned_upside_down();
        c->set_color(Cell::get_dead_color());
        population_--;
    } else {
        c->now_this_is_the_story_all_about_how_my_life_got_flipped_turned_upside_down();
        c->set_color(Cell::get_alive_color());
        population_++;
    }


    scene->update();

    std::string s = "Pop: " + std::to_string(population_)+ "(" + std::to_string(population_/200) + ")";
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
                if (surrounding_population == reproduction_requirement_) {
                    revive.push_back({row,col});

                }
            }
        }
    }

    //population_ += (revive.size() - kill.size());
    //population_ += accum;

    for (int i = 0; i < revive.size(); i++) {
        cells_[revive[i].first][revive[i].second]->set_color(Cell::get_alive_color());
        cells_[revive[i].first][revive[i].second]->now_this_is_the_story_all_about_how_my_life_got_flipped_turned_upside_down();
        //scene->update();
    }

    for (int i = 0; i < kill.size(); i++) {
        cells_[kill[i].first][kill[i].second]->set_color(Cell::get_dead_color());
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
    if (pops_.size()>40){
        pops_.erase(pops_.begin());
    }
    pops_.push_back(population_);
    qDebug()<< "pop: ";
    qDebug() <<population_ ;
    double ratio =population_/200.0;
    double rounded = (int)(ratio * 1000.0)/1000.0;
    qDebug()<<"ratio";
    qDebug()<<rounded;
    std::string s = "Pop: " + std::to_string(population_)+ "(" + std::to_string(rounded) + ")";
    ui->populationLabel->setText(s.c_str());

    std::string turn = "Turn: " + std::to_string(turn_ct_);
    ui->turnLabel->setText(turn.c_str());

    scene->update();


    UpdateGraph();

    //this will call a function update graph
}
void CellWindow::UpdateGraph(){
    graph_scene->clear();
    for (int i = 0; i<pops_.size(); i++){
        //qDebug()<<"pop size: ";
        //qDebug()<<pops_[i];
        QGraphicsRectItem* item3 = new QGraphicsRectItem((i+1)*10,85,10,-pops_[i]);
        graph_scene->addItem(item3);
    }
    graph_scene->update();
    view2->update();

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


void CellWindow::on_horizontalSlider_actionTriggered(int action)
{

}

void CellWindow::on_horizontalSlider_valueChanged(int value)
{
    speed_ = 1000-(value*4);

    std::string speed = "Speed: " + std::to_string(1000-speed_);

    ui->speedLabel->setText(speed.c_str());
    scene->update();
    qDebug()<<"in sliding";
    qDebug()<<speed_;
} //slider goes from 1-100

void CellWindow::on_randomizeColorButton_clicked()
{
    int r = rand() % 255;
    int g = rand() % 255;
    int b = rand() % 255;

    Cell::set_alive_color(QColor(r, g, b));
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 20; col++) {
            Cell *c = get_cell(row, col);

            if (c->is_alive()) {
                c->set_color(Cell::get_alive_color());
                scene->update();
            }
        }
    }
}

void CellWindow::on_comboBox_currentIndexChanged(int index)
{
    reproduction_requirement_ = index;
}

void CellWindow::on_resetButton_clicked()
{
   NewGame();
}
