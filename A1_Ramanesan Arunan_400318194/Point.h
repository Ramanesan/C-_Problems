#ifndef POINT_H
#define POINT_H

#include <iostream>
using namespace std;


class Point{
public:

Point();
Point(const double& x1,const double& y1);
double getXval() const;
double getYval() const;
double distanceTo(const Point& otherP);
friend istream& operator>>(istream &in,  Point& otherP);
friend ostream& operator<<(ostream &out, const Point& otherP);

private:

double x;
double y;


};

#endif