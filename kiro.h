#pragma once




class sommet{
    double x,y;
    bool type_sommet; // 0 si distribution, 1 si antenne
    int indice;
public :
    void set(double x1, double y1) {x=x1; y=y1;}
    double get_x() {return x;}
    double get_y() {return y;}
    sommet (double x1, double y1, bool t, int i) { x=x1; y=y1; type_sommet =t; indice =i;}


};
