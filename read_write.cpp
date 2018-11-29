#pragma once

#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include "kiro.h"
using namespace std;

// return n le nombre de nodes
int read_file(string distances_file, string nodes_file, vector<int> D, vector<node> distribution, vector<node> terminal){
    ifstream distances(string("../KIRO/Datas/")+distances_file);
    ifstream nodes(string("../KIRO/Datas/")+distances_file);
    if(distances && nodes){
        int test = 0;
        int n=0;
        while(test!=2){
            string contenu;
            getline(distances,contenu);
            int d_ = int(contenu);
            D.push_back(d_);
            n++;
            if (d_==0) test++;
        }
        n--;
        for(int i=0; i<n*n-n-1;i++){
            string contenu;
            getline(distances,contenu);
            int d_ = int(contenu);
            D.push_back(d_);
        }
        for(int i=0;i<n;i++){
            string contenu;
            getline(nodes,contenu,";");
            float x = float(contenu);
            getline(nodes,contenu,";");
            float y = float(contenu);


        }
        distances.close();
        nodes.close();
    }
    else cout << "pb ouverture" << endl;

    return 0;
}
