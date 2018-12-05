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
    for(int i=1; i< C.size(); i++){
        result += lenght[ C[i].get_indice() + C[i-1].get_indice()*n ];
    }

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






vector<vector<vector<node>>> clustering(vector<node> nodes_d, vector<node> nodes_t, vector<int> distances) {

    // Nombre total de noeuds
    int n = nodes_d.size() + nodes_t.size();

    // On associe a chaque noeud terminal un index correspondant au numero du point de distribution auquel il est rattache (le plus proche)
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

    // Ce qu'on va renvoyer a la fin
    vector<vector<vector<node>>> data_new(nodes_d.size());

    // Stocke tous les noeuds associes a chaque distribution
    vector<vector<node>> data(nodes_d.size());

    // On met les distributeurs dans data_new et data
    for (int i=0; i<nodes_d.size(); i++) {
        data[i].push_back(nodes_d[i]);
        data_new[i].push_back(data[i]);
    }

    // On remplit data avec les bons noeuds pour les distributeurs
    for (int i=0; i<nodes_t.size(); i++) {
        data[index_clusters[i]].push_back(nodes_t[i]);
    }

    // On complete data_new colonne par colonne
    for (int i=0; i<nodes_d.size(); i++) {

        // On ordonne data pour le ieme distributeur de facon a avoir le (j+1)eme element qui est le proche voisin
        // du jeme element parmi ceux qu'on a pas encore parcouru pour tout j
        for (int j=0; j<data[i].size()-1; j++) {
            int distance_min = distances[n*data[i][j].get_indice() + data[i][j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<data[i].size()-1; k++) {
                if (distances[n*data[i][j].get_indice() + data[i][k].get_indice()] < distance_min) {
                    distance_min = distances[n*data[i][j].get_indice() + data[i][k].get_indice()];
                    index = k;
                }
            }
            data[i].insert(data[i].begin()+j+1, data[i][index]);
            data[i].erase(data[i].begin()+index+1);
        }

        // Si la taille de data[i] est plus petite que 30, on ne fait pas de chaine et il n'y a qu'une boucle
        if (data[i].size() <= 30) {
            for (int j=0; j<data[i].size()-1; j++) {
                data_new[i][0].push_back(data[i][j+1]);
            }
        }

        // Sinon, on va faire des chaines
        else {

            // On commence par remplir la premiere colonne pour la boucle b
            for (int j=0; j<29 ; j++) {
                data_new[i][0].push_back(data[i][j+1]);
            }

            // On cree les chaines jusqu'a ce que tous les noeuds soient dans une chaine
            for (int j=30; j<data[i].size(); j++) {

                // Si la chaine precedente est de longueur 5, on en cree une nouvelle
                // On cherche le point de la boucle le plus proche pour commencer la chaine
                if (data_new[i][data_new[i].size()-1].size() >= 5) {
                    vector<node> new_chain;
                    int distance_min = distances[n*data_new[i][0][0].get_indice() + data[i][j].get_indice()];
                    int index = 0;
                    for (int k=1; k<30; k++) {
                        if (distances[n*data_new[i][0][k].get_indice() + data[i][j].get_indice()] < distance_min) {
                            distance_min = distances[n*data_new[i][0][k].get_indice() + data[i][j].get_indice()];
                            index = k;
                        }
                    }
                    new_chain.push_back(data_new[i][0][index]);
                    new_chain.push_back(data[i][j]);
                    data_new[i].push_back(new_chain);
                }

                // Sinon, on regarde le cout pour passer du dernier node de la derniere chaine au point que l'on considere (cost1)
                // On compare au cout en creant une nouvelle chaine avec un point de la boucle et ce point (cost2)
                // On effectue l'action de plus petit cout
                else {

                    // Calcul du cout 1
                    int cost1 = distances[n*data_new[i][data_new[i].size()-1][data_new[i][data_new[i].size()-1].size()-1].get_indice() + data[i][j].get_indice()];

                    // Calcul du cout 2
                    int cost2 = distances[n*data_new[i][0][0].get_indice() + data[i][j]];
                    int index = 0;
                    for (int k=1; k<data_new[i][0].size(); k++) {
                        if (distances[n*data_new[i][0][k].get_indice() + data[i][j].get_indice()] < cost2) {
                            cost2 = distances[n*data_new[i][0][k].get_indice() + data[i][j].get_indice()];
                            index = k;
                        }
                    }

                    // Si rajouter le noeud a la fin de la derniere chaine est le meilleur choix
                    if (cost1 < cost2) {
                        data_new[i][data_new[i].size()-1].push_back(data[i][j]);
                    }

                    // Sinon, si creer une nouvelle chaine est le meilleur choix
                    else {
                        vector<node> new_chain;
                        new_chain.push_back(data_new[i][0][index]);
                        new_chain.push_back(data[i][j]);
                        data_new[i].push_back(new_chain);
                    }

                }

            }

        }

    }
    return data_new;
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

