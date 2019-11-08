#ifndef CELLWINDOW_H
#define CELLWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColor>
#include <vector>
#include "cell.h"
#include <QSlider>
namespace Ui {
class CellWindow;
}
//del comment
//del comment 2
class CellWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CellWindow(QWidget *parent = nullptr);
    ~CellWindow();

    void NewGame();

    void SimulateTurn(); //will call GetNeighbors and then create it will create
    //two vectors - one that keeps track of cells to flip from alive to dead
    // and one that keep track of cells to flip from dead to alive

    int GetNeighbors(int row, int col); // will return the count of number alive neighbors

    int get_total_pop(){ return population_; }

    int get_turn_ct(){ return turn_ct_; }

    Cell* get_cell(int row, int col) {return cells_[row][col];}

    void AddCell(Cell *c);

    int grid_[20][10];
    //onStartButtonClicked - will start the timer
    //onStopButtonClicked - will sotp the timer
    //onStepButtonClicked - will just call simulateturn
    //slider func - will update the timer so make timer_*= 1000
    //for graph will want to add a new scene

private slots:
     void CellClickedSlot(Cell *c);
     //void onPlayButtonClicked();
     void on_stepButton_clicked();
     void on_playButton_clicked();
     void on_pauseButton_clicked();
     //void on_horizontalSlider_valueChanged();
     //void setValue(int);
     //void PlotWindow::on_randomButton_clicked()

     void on_horizontalSlider_actionTriggered(int action);
     void on_horizontalSlider_valueChanged(int);

     void on_randomizeColorButton_clicked();

     void on_comboBox_currentIndexChanged(int index);

     void on_resetButton_clicked();

private:
    Ui::CellWindow *ui;

    QGraphicsScene *scene;

   int population_;

   int turn_ct_;

   QTimer * time_;

   int speed_;

   std::vector<std::vector<Cell*>> cells_;

   int reproduction_requirement_;
};

#endif // CELLWINDOW_H
