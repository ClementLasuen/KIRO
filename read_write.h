#pragma once

#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include "kiro.h"
using namespace std;

// return n le nombre de nodes
int read_file(string distances_file, string nodes_file, vector<int> D, vector<node> distribution, vector<node> terminal){
    ifstream distances(string("../KIRO/Data/")+distances_file);
    ifstream nodes(string("../KIRO/Data/")+nodes_file);
    if(distances && nodes){
        int test = 0;
        int n=0;
        while(test!=2){
            int d_;
            distances >> d_;
            D.push_back(d_);
            n++;
            if (d_==0) test++;
        }
        n--;
        for(int i=0; i<n*n-n-1;i++){
            int d_;
            distances >> d_;
            D.push_back(d_);
        }
        for(int i=0;i<n;i++){
            string contenu;
            getline(nodes,contenu);
            double x;
            double y;
            string type;
            scanf(contenu,"%d %d %s", x, y, type);
            bool t = false;
            if(contenu==string("terminal")) t = true;
            if (t){
                node N(x,y,t,i);
                terminal.push_back(N);
            }
            else {
                node N(x,y,t,i);
                distribution.push_back(N);
            }
        }
        distances.close();
        nodes.close();
        return(n);
    }
    else cout << "pb ouverture" << endl;
}
