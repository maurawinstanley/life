#ifndef CELLWINDOW_H
#define CELLWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColor>
#include <vector>
#include "cell.h"

namespace Ui {
class CellWindow;
}

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



    void AddCell(Cell *c);

    Cell* get_cell(int row, int col){ return cells_[row][col]; };

    int grid_[20][10];

private slots:
     void CellClickedSlot(Cell *c);

private:
    Ui::CellWindow *ui;

    QGraphicsScene *scene;

   int population_;

   int turn_ct_;

   std::vector<std::vector<Cell*>> cells_;
};

#endif // CELLWINDOW_H
