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

    int j = rand()%nodes.size();
    int compteur=0;
    if(i < circuit.size()-1){
        while( !nodes[j].is_in(circuit) && nodes[j].is_terminal() && compteur<100){
            circuit[i] = nodes[j];
            j = rand()%nodes.size();
            compteur++;
        }
    }
}

// Prend une distribution

void easy_way(vector<node> C, vector<double> lenght , vector<node> &solution, bool hugo = false){
    int k =0;
    while( C[k].is_terminal() || C[k].is_in(solution)) k+=1;
    solution.push_back(C[k]);
    int i=0;
    while( k <30){
        if (i!=k) solution.push_back(C[i]);
        i++;
    }
}







// tEMPERAYITR

void temperature(vector<node> &C, vector<node> nodes, vector<double> lenght, double T){

    vector<node> new_circuit = change_one_node(circuit, nodes, lenght);

    double p = (rand()%100)/100;
    if(p<= min(1, exp(cost(C,lenght)-cost(new_circuit,lenght)))){
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
