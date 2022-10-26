#include <iostream>
#include <sstream>
#include "Point.h"
#include "Polygon.h"
#include <math.h>


// Point Class definitions
Point::Point(){
    x = 0;
    y = 0;
}
// constructor
Point::Point(const double& x1,const double& y1){
    x = x1;
    y = y1;
}

// x value getter
double Point::getXval() const {
    return x;
}

// y value getter
double Point::getYval() const {
    return y;
}

//distance function
double Point::distanceTo(const Point& otherP){
    double x1,y1,xsquare,ysquare,total;
    x1 = otherP.getXval();
    y1 = otherP.getYval();
    xsquare = pow((x1-x),2);
    ysquare = pow((y1-y),2);
    total = sqrt((xsquare + ysquare));
    return total;
}

// cin operator overload
istream& operator>>(istream &in, Point& otherP){
    in >> otherP.x >> otherP.y;
    return in;
} 

// cout operator overload
ostream& operator<<(ostream &out, const Point& otherP){
    out << "P(" << otherP.x << ", " << otherP.y << ")" ;
    return out;
}

// Polygon Class definitions

//default constructor
Polygon::Polygon(int d){
    vert = d;  
    points = new Point[d];  
}

//constructor 2
Polygon::Polygon(const int& d,const Point* points_list){
    vert = d;
    points = new Point[d];
    for(int i=0;i<d;i++){
        points[i] = points_list[i];
    }

}

//destructor
Polygon::~Polygon(){
    delete [] points;
}

//perimeter calculation
double Polygon::perimeter(){
    int i; int j = 1; double peri=0; int final = 0;
    for (i=0;i<vert;i++){
        if(j!=vert){
            peri += points[i].distanceTo(points[j]);
            j++;
       }       
    }
    if(vert!=2){
        peri += points[vert-1].distanceTo(points[0]);
    }
    return peri ;
}

// cout operator overload
ostream& operator<<(ostream &out, const Polygon& otherP){
    for(int i=0;i<(otherP.vert);i++){
        out << otherP.points[i] << " " ;
    }
    
    return out;
}

// cin operator overload
istream& operator>>(istream &in, Polygon& otherP){
    for(int i=0;i<(otherP.vert);i++){
         in >> otherP.points[i] ;
    }
   
    return in;
} 

int main() {
    /*
    Expected output:

    P(0, 0), P(5, 12)
    5, 12
    13, 13
    P(9, 15)
    P(0, 0) P(0, 0) P(0, 0) P(0, 0) P(0, 0)
    P(0, 0) P(0, 1) P(1, 1) P(1, 0)
    4, 0
    P(1, 4) P(5, 1) P(10, 13) P(6, 10) P(1, 10)
    34
    */

    Point p1;  // (2) no arg constructor
    std::cout << p1 << ", ";  

    Point p2(5, 12);  // (3) two arg constructor
    std::cout << p2 << "\n";

    std::cout << p2.getXval() << ", ";  // (4) getters
    std::cout << p2.getYval() << "\n";

    std::cout << p1.distanceTo(p2) << ", ";  // (5) distanceTo
    std::cout << p2.distanceTo(p1) << "\n";

    std::istringstream input1("9 15");  // (6) input
    input1 >> p1;
    std::cout << p1 << "\n";

    Polygon poly(5);  // (9) size-only constructor
    std::cout << poly << "\n";

    Point* points_list = new Point[4];  // (10) array constructor
    points_list[0] = Point(0, 0);
    points_list[1] = Point(0, 1);
    points_list[2] = Point(1, 1);
    points_list[3] = Point(1, 0);
    Polygon rectangle(4, points_list);
    std::cout << rectangle << "\n";


    std::cout << rectangle.perimeter() << ", "; // (12) perimeter
    std::cout << poly.perimeter() << "\n";


    std::istringstream input2("1 4 5 1 10 13 6 10 1 10");  // (13) input
    input2 >> poly;
    std::cout << poly << "\n";
    std::cout << poly.perimeter() << "\n";
    
    return 0;
}