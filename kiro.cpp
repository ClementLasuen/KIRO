#include "kiro.h"


bool node::is_in(vector<node> V){
    for(int i=0; i<V.size();i++){
        if(indice== V[i].get_indice()) return true;
    }
    return false;
}


double cost(vector<node> C, vector<double> lenght){
    double result =0;
    int n = sqrt(lenght.size());
    for(int i=1; i< C.size(); i++){
        result += lenght[ C[i].get_indice()*n + C[i-1].get_indice() ];
    }

    return result;
}

//-------------------------------------------------------
//----------- Heuristique -------------------------------


vector<node> change_one_node(vector<node> circuit, vector<node> nodes, vector<double> lenght, int i){
    double c = cost(circuit, lenght);
    srand(clock());
    vector<node> circuit2(circuit);
    int j = rand()%nodes.size();
    int compteur=0;
    if(i < circuit.size()-1){
        while( !nodes[j].is_in(circuit2) && nodes[j].is_terminal() && compteur<100){
            circuit2[i] = nodes[j];
            j = rand()%nodes.size();
            compteur++;
        }
    }
    return circuit2;
}


vector<vector<node>> clustering(vector<node> nodes_d, vector<node> nodes_t, vector<int> distances) {
//    vector<node> clusters;
//    for (int i=0; i<nodes.size(); i++) {
//        if (nodes[i].is_terminal() == false) {
//            clusters.push_back(nodes[i]);
//        }
//    }

    int n = nodes_d.size() + nodes_t.size();

    vector<int> index_clusters;
    for (int i=0; i<nodes_t.size(); i++) {
        int index = 0;
        int min_distance = distances[n*nodes_t[i].get_indice() + nodes_d[0].get_indice()];
        for (int j=0; j<nodes_d.size(); j++) {
            if (distances[n*nodes_t[i].get_indice() + nodes_d[j].get_indice()] < min_distance) {
                min_distance = distances[n*nodes_t[i].get_indice() + nodes_d[j].get_indice()];
                index = j;
            }
        }
        index_clusters.push_back(index);
    }

    vector<vector<node>> data(nodes_d.size());
    for (int i=0; i<nodes_d.size(); i++) {
        data[i].push_back(nodes_d[i]);
    }
    for (int i=0; i<nodes_t.size(); i++) {
        data[index_clusters[i]].push_back(nodes_t[i]);
    }

    return data;
}


// Prend une distribution

void easy_way(vector<vector<int>> &C, vector<node> distributions, vector<node> terminal){
    /*
    int n_terminal = terminal.size();
    int n_distributions = distributions.size();
    while( C[k].is_terminal() || C[k].is_in(solution)) k+=1;
    solution.push_back(C[k]);
    int i=0;
    while( k <30){
        if (i!=k) solution.push_back(C[i]);
        i++;
    }*/
}


// tEMPERAYITR

void temperature(vector<node> &C, vector<node> nodes, vector<double> lenght, double T){

    int i =rand()%C.size();
    vector<node> new_circuit = change_one_node(C, nodes, lenght,i);

    double p = (rand()%100)/100;
    if(p<= min(1.0, exp(cost(C,lenght)-cost(new_circuit,lenght)))){
        C = new_circuit;
    }
}

void change_T(vector<node> &C, vector<node> nodes, vector<double> lenght){
    double beta = 0.9;
    double T0 = 10000;
    for(int k=0;k<1000;k++){
        beta*=beta;
        temperature(C,nodes,lenght, T0*beta );
    }
}
