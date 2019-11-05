#ifndef CELL_H
#define CELL_H

#include <QColor>
#include <QGraphicsItem>

class Cell : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    Cell();  // constructor
    Cell(QColor color, const int x, const int y);

    int get_x() const { return x_; }  // inline member function
    int get_y() const { return y_; }  // inline member function

    void set_color(QColor color) {color_ = color;}

    double Distance(const Cell &other) const;  // member function

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static int get_width() { return width_; }

// in point.h
signals:
    // for distance
    void CellSelected(int x, int y);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;



private:
  int x_;
  int y_;
  QColor color_;
  //bool alive_; // maybe, probably not


  // all our Points will be 10 wide and tall (they are circles)
  static const int width_ = 20;

  // if you declare the operator as a friend, then it isn't
  // part of the object
  // friend will let this method access the private fields
  friend bool operator==(const Cell &first, const Cell &other);
};  // class Point



#endif // CELL_H
