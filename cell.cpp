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

Cell::Cell(QColor color, const int x, const int y) {
  this->color_ = color;
  x_ = x;
  y_ = y;
}

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
    painter->setBrush(QBrush(color_.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}


//in point.cpp
void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Day 2, Task 3
    qDebug() << x_;
    qDebug() << y_;
    qDebug() << "point clicked!";
    emit CellSelected(x_, y_);

    // Day 2, Task 4
    int red = rand() % 255;
    int green = rand() % 255;
    int blue = rand() % 255;
    QColor c(red, green, blue);
    color_ = c;

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
