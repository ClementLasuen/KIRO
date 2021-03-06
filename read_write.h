#pragma once

#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include "kiro.h"
using namespace std;

// return n le nombre de nodes
int read_file(string distances_file, string nodes_file, vector<int> &D, vector<node> &distribution, vector<node> &terminal){
    string path_distance = string("../Data/")+distances_file;
    string path_nodes = string("../Data/")+nodes_file;
    ifstream distances(path_distance);
    ifstream nodes(path_nodes);
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
        n-=2;
        for(int i=0; i<n*n-n-2;i++){
            int d_;
            distances >> d_;
            D.push_back(d_);
        }
        for(int i=0;i<n;i++){
            double x;
            double y;
            string type;
            nodes >> x >> y >> type;
            bool t = false;
            if(type==string("terminal")) t = true;
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
    else cerr << "pb ouverture" << endl;
}



// V est un chemin ou un circuit, cette fonction ecrit ligne par ligne
void write(vector<vector<node> > V, string name){
    string file_name = string("../")+name; //+string<int>(nb_iterations)+string("_")+string<int>(h);
    ofstream fichier(file_name.c_str(), ios::out|ios::trunc); // On va ecrire a la fin du fichier
    if (fichier){
        for(int i=0;i<V.size();i++){
            if(V[i].size() < 30){
                fichier << "b";
                for(int j=0;j<V[i].size();j++)
                    fichier << " " << V[i][j].get_indice();
                fichier << endl;
            }
            else{
                fichier << "b";
                for(int j=0;j<30;j++)
                    fichier << " " << V[i][j].get_indice() ;
                fichier << endl;

                int k=30;
                while(k+4 < V[i].size()){
                    fichier << "c" ;
                    fichier<<" " << V[i][0].get_indice();
                    // raccorder au point de la boucle le plus proche plutot qu'a la distribution
                    for(int l=0;l<4;l++)
                        fichier<< " " << V[i][k+l].get_indice() ;
                    fichier<< endl;
                    k+=4;
                }
                fichier << "c" ;
                fichier<<" " << V[i][0].get_indice();
                while(k<V[i].size()){
                    fichier << " "<< V[i][k].get_indice() ;
                    k++;
                }
                fichier << endl;
            }
        }

    }
    fichier.close();
}
