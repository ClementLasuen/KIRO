#pragma once




class node{
    double x,y;
    bool node_type; // 0 si distribution, 1 si antenne
    int indice;
public :
    void set(double x1, double y1) {x=x1; y=y1;}
    double get_x() {return x;}
    double get_y() {return y;}
    node (double x1, double y1, bool t, int i) { x=x1; y=y1; node_type =t; indice =i;}
    node () {}
};

class arc{
    node u,v;
    double l;
public :
    double get_l() {return l;}
    arc (node u1, node v1, double l1) {u=u1 ; v=v1; l =l1;}

};
