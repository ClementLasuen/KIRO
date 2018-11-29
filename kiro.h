#pragma once


#include <time.h>
#include <math.h>
#include <vector>
using namespace std;

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
    int get_indice() {return indice;}
    bool is_in(vector<node> V);
    bool is_terminal() {return node_type;}
};

class arc{
    node u,v;
    double l;
public :
    double get_l() {return l;}
    arc (node u1, node v1, double l1) {u=u1 ; v=v1; l =l1;}

};

double cost(vector<node> C, vector<double> lenght);


// Assume that the first element is the distribution
void change_one_node(vector<node> circuit, vector<node> nodes, vector<double> lenght);
