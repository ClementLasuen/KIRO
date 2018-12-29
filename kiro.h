#pragma once

#include <cstdlib>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <random>
#include <map>
#include <set>
#include <climits>
#include <fstream>
#include <string>

using namespace std;

class node{
    double x,y;
    bool node_type; // 0 si distribution, 1 si antenne
    int indice;
public :
    void set(double x1, double y1) {x=x1; y=y1;}
    void set_indice(int ind) {indice =ind;}
    double get_x() {return x;}
    double get_y() {return y;}
    node (double x1, double y1, bool t, int i) { x=x1; y=y1; node_type =t; indice =i;}
    node () {}
    int get_indice() {return indice;}
    bool get_node_type() {return node_type;}
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


int read_file(string distances_file, string nodes_file, vector<int> &D, vector<node> &distribution, vector<node> &terminal);

void write(vector<vector<vector<node>>> V, string name);

vector<vector<vector<node>>> read_solution(string name, vector<node> distributions, vector<node> terminaux);

int cost(vector<vector<node> > C, vector<int> lenght);

int cost_solution(vector<vector<vector<node> > > C, vector<int> lenght);

// Assume that the first element is the distribution
vector<node> change_one_node(vector<node> circuit, vector<node> nodes, vector<double> lenght);

void echange(vector<vector<node>> &C, int n1, int n2, int i1, int i2);

void echange_aleat(vector<vector<vector<node>>> &data, vector<node> distributions, vector<node> terminal);

void switch_chain(vector<vector<node> > &sol, vector<int> lenght);

vector<vector<vector<node>>> clustering(vector<node> nodes_d, vector<node> nodes_t, vector<int> distances);

vector<vector<vector<node>>> change_loop_randomly(vector<vector<vector<node>>> best_data, int n, vector<int> distances, int indice);

vector<vector<vector<node>>> heuristic_change_loop_randomly(vector<node> distribution, vector<node> terminal, vector<int> D, int rep, string name);

vector<vector<vector<node>>> neighbor(vector<vector<vector<node>>> best_data, int n, vector<int> distances, int indice);

vector<vector<vector<node>>> heuristic_neighbor(vector<vector<vector<node>>> initial_solution, vector<node> distribution, vector<node> terminal, vector<int> D, int rep, string name);

vector<vector<vector<node>>> heuristic_neighbor_multiple_times(vector<vector<vector<node>>> initial_solution, vector<node> distribution, vector<node> terminal, vector<int> D, int rep, int k, string name);

