#include "kiro.h"


bool node::is_in(vector<node> V){
    for(int i=0; i<V.size();i++){
        if(indice== V[i].get_indice()) return true;
    }
    return false;
}


int cost_solution(vector<vector<node> > C, vector<int> lenght){
    int result =0;

    for(int i=0;i<C.size();i++) result += cost(C[i],lenght);
    return result;
}

int cost(vector<node> C, vector<int> lenght){
    int result =0;
    int n = sqrt(lenght.size());
    int m = C.size();
    // COUT DE LA BOUCLE PRINCIPALE

    for(int i=1; i< min(m, 30) ; i++)  result += lenght[ C[i].get_indice() + C[i-1].get_indice()*n ];
    if(C.size() >= 30) result += lenght[ C[29].get_indice()*n + C[0].get_indice()];

    // COUT DES BRANCHES ASSOCIEES
    // ELLES COMMENTCENT TOUTES A LA DISTRIBUTION

    int k=30;


    while(k+4< C.size()){

        // ON PART DE LA DISTRIBUTION
        result += lenght[ C[0].get_indice()*n + C[k].get_indice()];
        for(int i =0;i<3;i++)
        {
            result += lenght[ C[k+i].get_indice()*n + C[k+i+1].get_indice()];
        }

        k += 4;
    }

    while(k+1<C.size()){
        result += lenght[ C[k].get_indice()*n + C[k+1].get_indice()];
        k++;
    }


    /*for(int i=1; i< C.size(); i++){
        result += lenght[ C[i].get_indice() + C[i-1].get_indice()*n ];
    }
    if(C.size() > 5) result += lenght[ C[C.size()].get_indice()*n + C[0].get_indice()];
    */


    return result;
}

//-------------------------------------------------------
//----------- Heuristique -------------------------------


vector<node> change_one_node(vector<node> circuit, vector<node> nodes, vector<int> lenght, int i){
    int c = cost(circuit, lenght);
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
    for (int i=0; i<nodes_d.size(); i++) {
        for (int j=0; j<data[i].size()-2; j++) {
            int distance_min = distances[n*data[i][j].get_indice() + data[i][j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<data[i].size()-1; k++) {
                if (distances[n*data[i][j].get_indice() + data[i][k].get_indice()] < distance_min) {
                    distance_min = distances[n*data[i][j].get_indice() + data[i][k].get_indice()];
                    index = k;
                }
            }
            data[i].insert(data[i].begin()+ j+1, data[i][index]);
            data[i].erase(data[i].begin()+index+1);

        }
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
    }
    for(int i=0;i<distributions.size();i++){
        vector<int> d ;
        d.push_back(distributions[i]);
        for(int j=0;j<29;j++){
            if(i*30 +j < terminal.size) d.push_back(terminal[i*30 +j].get_indice());
        }
        C.push_back(d);
    }
*/

}


// tEMPERAYITR

/*
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
*/

void echange(vector<node> &C, int i, int j){
    node temp(C[i].get_x(),C[i].get_y(), C[i].get_node_type(),C[i].get_indice());
    C[i].set(C[j].get_x(), C[j].get_y());
    C[i].set_indice(C[j].get_indice());
    C[j].set(temp.get_x(), temp.get_y());
    C[j].set_indice(temp.get_indice());
}

void echange_aleat(vector<vector<node>> &data, vector<node> distributions, vector<node> terminal){
    int c = 0;
    int s = 0;
    while (s<31){
        c = rand()%distributions.size();
        s = data[c].size();
    }
    int d = data[c].size()-30;
    int i = rand()%(d-1) + 31;
    int j = i;
    while (i==j)
        j = rand()%(d-1) + 31;
    echange(data[c],i,j);
}

void switch_chain(vector<vector<node> > &sol, vector<int> lenght){

    vector<vector<node> > sol1 (sol);

    int n = sqrt(lenght.size());
    for(int i=0; i<sol.size();i++){
        if(sol[i].size()>30){


            // Les chaines commencent à 30 + 4*j
            int j=30;
            while(j+4 < sol[i].size()){

                // trouver le plsu proche de sol[i][30+4*i]
                for(int k=0;k<sol[i].size();k++){
                    if(  lenght[ sol[i][k].get_indice()*n +  sol[i][j].get_indice()] <=
                         lenght[sol[i][0].get_indice()*n +  sol[i][j].get_indice()]){
                        node p (sol[i][0]);
                        sol[i][0] = sol[i][k];
                        sol[i][k] = p;
                    }
                }
                j+=4;
            }

        }
    }


}

