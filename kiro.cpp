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


void change_one_node(vector<node> circuit, vector<node> nodes, vector<double> lenght){
    double c = cost(circuit, lenght);
    srand(clock());
    int i = rand()%circuit.size() +1;
    int j = rand()%nodes.size();
    int compteur=0;
    if(i < circuit.size()-1){
        while( !nodes[j].is_in(circuit) && nodes[j].is_terminal() && cost(circuit,lenght)>= c && compteur<100){
            circuit[i] = nodes[j];
            j = rand()%nodes.size();
            compteur++;
        }

    }
}
