#ifndef POLYGON_H
#define POLYGON_H

#include <iostream>
using namespace std;


class Polygon{
public:
Polygon(int d = 10);
Polygon(const int& d,const Point* points_list);
~Polygon();
double perimeter();
friend istream& operator>>(istream &in,  Polygon& otherP);
friend ostream& operator<<(ostream &out, const Polygon& otherP);


private:
int vert;
Point* points;

};

#endif