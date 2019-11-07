#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>
#include <QColor>

#include "cell.h"
#include <string>




/**
  Creates a new Cell object with coordinates x and y
  @param x int x coordinate
  @param y int y coordinate
*/

Cell::Cell(QColor color, const int x, const int y, bool alive) {
  this->color_ = color;
  x_ = x;
  y_ = y;
  alive_ = alive;
}

//Cell* Cell::Clone() {
//    Cell *c = new Cell(this->color_, this->x_, this->y_);
//    return c;
//}

QRectF Cell::boundingRect() const
{
    return QRectF(x_, y_, width_, width_);
}

QPainterPath Cell::shape() const
{
    QPainterPath path;
    path.addEllipse(x_, y_, width_, width_);
    return path;
}


void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));

    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}


//in point.cpp
void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    emit CellSelected(this);

    if (alive_) {
        alive_ = false;
        color_ = QColor(255,20,147);
        //TODO decrement population
    } else {
        alive_ = true;
        color_ = QColor(255,255,255);
        //TODO increment population
    }

    // update this QGraphicsItem (force it to re-draw)
    update();
}




/**
  Makes it so the == operator will have the behavior that you
  expect when comparing cells.
  You can overload pretty much any operator in c++
  @param first Point left hand side of the expression
  @param other Point right hand side of the expression
*/
bool operator==(const Cell &first, const Cell &other) {
  return first.x_ == other.x_ && first.y_ == other.y_;
}
