#include "kiro.h"


bool node::is_in(vector<node> V){
    for(int i=0; i<V.size();i++){
        if(indice== V[i].get_indice()) return true;
    }
    return false;
}


int cost_solution(vector<vector<vector<node>>> C, vector<int> lenght){
    int result =0;

    for(int i=0;i<C.size();i++) result += cost(C[i],lenght);
    return result;
}

int cost(vector<vector<node>> C, vector<int> lenght){ // Pour une distribution
    int result =0;
    int n = sqrt(lenght.size());
    for (int j = 0; j<C.size(); j++){
        if (j==0)
            result+=lenght[ C[j][0].get_indice() + C[j][C[j].size()-1].get_indice()*n ];
        for(int i=1; i< C[j].size(); i++){
            result += lenght[ C[j][i].get_indice() + C[j][i-1].get_indice()*n ];
        }
    }

    return result;
}

//-------------------------------------------------------
//----------- Heuristique -------------------------------

/*
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
*/





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
            for (int k=j+2; k<data[i].size(); k++) {
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

                // Sinon, on regarde le cout pour passer du dernier noeud de la derniere chaine au point que l'on considere (cost1)
                // On compare au cout en creant une nouvelle chaine avec un point de la boucle et ce point (cost2)
                // On effectue l'action de plus petit cout
                else {

                    // Calcul du cout 1
                    int cost1 = distances[n*data_new[i][data_new[i].size()-1][data_new[i][data_new[i].size()-1].size()-1].get_indice() + data[i][j].get_indice()];

                    // Calcul du cout 2
                    int cost2 = distances[n*data_new[i][0][0].get_indice() + data[i][j].get_indice()];
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

void echange(vector<vector<node>> &C, int n1, int n2, int i1, int i2){
    node temp(C[n1][i1].get_x(),C[n1][i1].get_y(), C[n1][i1].get_node_type(),C[n1][i1].get_indice());
    C[n1][i1].set(C[n2][i2].get_x(), C[n2][i2].get_y());
    C[n1][i1].set_indice(C[n2][i2].get_indice());
    C[n2][i2].set(temp.get_x(), temp.get_y());
    C[n2][i2].set_indice(temp.get_indice());
}

void echange_aleat(vector<vector<vector<node>>> &data, vector<node> distributions, vector<node> terminal){
    map<int,vector<vector<int>>> encrage_chaine; // vecteur listant les noeuds auquels des chaines sont encrees : a un noeud, donne le vecteur des (numero distribution, numero chaine)
    for(int i = 0; i<data.size(); i++){
        if (data[i].size()>1){
            for(int j=1;j<data[i].size();j++){
                vector<vector<int>> v;
                encrage_chaine[data[i][j][0].get_indice()] = v;
            }
        }
    }
    for(int i = 0; i<data.size(); i++){
        if (data[i].size()>1){
            for(int j=1;j<data[i].size();j++){
                vector<int> v = {i,j};
                encrage_chaine[data[i][j][0].get_indice()].push_back(v);
            }
        }
    }
    int n_distrib = rand()%data.size();
    if (data[n_distrib].size()==1){
        int b_size = data[n_distrib][0].size();
        int i1 = rand()%(b_size-1)+1;
        int i2 = i1;
        while (i1==i2)
            i2 = rand()%(b_size-1)+1;
        echange(data[n_distrib],0,0,i1,i2);
    }
    else if(data[n_distrib].size()>1){
        int n1 = rand()%data[n_distrib].size();
        int n2 = rand()%data[n_distrib].size();
        if (n2<n1){     // on fait en sorte que n1<n2
            int a = n2;
            n2 = n1;
            n1 = a;
        }
        if (n1 > 0){
            int c1_size = data[n_distrib][n1].size();
            int c2_size = data[n_distrib][n2].size();
            if (c1_size>1 && c2_size>1){
                int i1 = rand()%(c1_size-1)+1;
                int i2 = rand()%(c2_size-1)+1;
                echange(data[n_distrib],0,0,i1,i2);
            }
        }
        else if (n1 == 0){
            int c1_size = data[n_distrib][n1].size();
            int c2_size = data[n_distrib][n2].size();
            if (c1_size>1 && c2_size>1){
                int i1 = rand()%(c1_size-1)+1;
                int i2 = rand()%(c2_size-1)+1;
                int indice = data[n_distrib][n1][i1].get_indice();
                auto it = encrage_chaine.find(indice);
                if (it!=encrage_chaine.end()){
                    for(int i = 0; i<(it->second).size(); i++){
                        data[it->second[i][0]][it->second[i][1]][0] = data[n_distrib][n2][i2];
                    }
                    echange(data[n_distrib],n1,n2,i1,i2);
                    encrage_chaine[data[n_distrib][n2][i2].get_indice()] = it->second;
                    encrage_chaine.erase(indice);
                }
            }
        }
    }
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
































// n est le nombre de noeuds
// best_data est le meilleur data que l'on a et que l'on veut ameliorer sur la colonne indice
vector<vector<vector<node>>> heuristic_loop(vector<vector<vector<node>>> best_data, int n, vector<int> distances, int indice) {

    // Ce qu'on va renvoyer a la fin
    vector<vector<vector<node>>> data_new = best_data;

    // On modifie best_data seulement dans la colonne indice

    // Taille de nouvelle boucle
    default_random_engine generator(random_device{}());
    int loop_size = uniform_int_distribution<>(2,30)(generator);

    // Tous les elements de best_data[indice]
    vector<node> all_elements;
    for (int i=0; i<best_data[indice].size(); i++) {
        if (i == 0) {
            for (int j=0; j<best_data[indice][i].size(); j++) {
                all_elements.push_back(best_data[indice][i][j]);
            }
        }
        else {
            for (int j=1; j<best_data[indice][i].size(); j++) {
                all_elements.push_back(best_data[indice][i][j]);
            }
        }
    }

    // Si la taille de la boucle est plus grande que le nombre total d'elements de best_data[indice], on met tout dans la boucle
    if (loop_size >= all_elements.size()) {
        vector<vector<node>> new_column;
        vector<node> loop;
        for (int i=0; i<best_data[indice].size(); i++) {
            for (int j=0; j<best_data[indice][i].size(); j++) {
                if (i == 0) {
                    loop.push_back(best_data[indice][i][j]);
                }
                else {
                    if (j >= 1) {
                        loop.push_back(best_data[indice][i][j]);
                    }
                }
            }
        }
        new_column.push_back(loop);
        data_new[indice] = new_column;
        return data_new;
    }

    // Sinon, on va devoir choisir aleatoirement la boucle principale et creer des chaines
    else {
        vector<vector<node>> new_column;
        //Definition de la boucle principale et de la chaine totale
        vector<node> loop;
        vector<node> chain;

        // Construction de l'ensemble des indices de la boucle principale
        set<int> set_int;
        set_int.insert(0);
        while (set_int.size() < loop_size) {
            default_random_engine generator(random_device{}());
            set_int.insert(uniform_int_distribution<>(1,all_elements.size()-1)(generator));
        }

        // On remplit ce qu'il y aura dans la boucle et dans les chaines
        set<int>::iterator it;
        for (int k=0; k<all_elements.size(); k++) {
            it = set_int.find(k);
            if (it == set_int.end()) {
                chain.push_back(all_elements[k]);
            }
            else {
                loop.push_back(all_elements[k]);
            }
        }

        // On ordonne la boucle et les chaines pour avoir un cout petit
        for (int j=0; j<loop.size()-1; j++) {
            int distance_min = distances[n*loop[j].get_indice() + loop[j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<loop.size(); k++) {
                if (distances[n*loop[j].get_indice() + loop[k].get_indice()] < distance_min) {
                    distance_min = distances[n*loop[j].get_indice() + loop[k].get_indice()];
                    index = k;
                }
            }
            loop.insert(loop.begin()+j+1, loop[index]);
            loop.erase(loop.begin()+index+1);
        }
        for (int j=0; j<chain.size()-1; j++) {
            int distance_min = distances[n*chain[j].get_indice() + chain[j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<chain.size(); k++) {
                if (distances[n*chain[j].get_indice() + chain[k].get_indice()] < distance_min) {
                    distance_min = distances[n*chain[j].get_indice() + chain[k].get_indice()];
                    index = k;
                }
            }
            chain.insert(chain.begin()+j+1, chain[index]);
            chain.erase(chain.begin()+index+1);
        }

        // On met la boucle dans new_column
        new_column.push_back(loop);

        // On cree les chaines jusqu'a ce que tous les noeuds soient dans une chaine
        for (int j=0; j<chain.size(); j++) {

            // Si la chaine precedente est de longueur 6, on en cree une nouvelle
            // On cherche le point de la boucle le plus proche pour commencer la chaine
            if (new_column[new_column.size()-1].size() >= 6) {
                vector<node> new_chain;
                int distance_min = distances[n*new_column[0][0].get_indice() + chain[j].get_indice()];
                int index = 0;
                for (int k=1; k<loop.size(); k++) {
                    if (distances[n*new_column[0][k].get_indice() + chain[j].get_indice()] < distance_min) {
                        distance_min = distances[n*new_column[0][k].get_indice() + chain[j].get_indice()];
                        index = k;
                    }
                }
                new_chain.push_back(new_column[0][index]);
                new_chain.push_back(chain[j]);
                new_column.push_back(new_chain);
            }

            // Sinon, on regarde le cout pour passer du dernier noeud de la derniere chaine au point que l'on considere (cost1)
            // On compare au cout en creant une nouvelle chaine avec un point de la boucle et ce point (cost2)
            // On effectue l'action de plus petit cout
            else {

                // Calcul du cout 1
                int cost1 = distances[n*new_column[new_column.size()-1][new_column[new_column.size()-1].size()-1].get_indice() + chain[j].get_indice()];

                // Calcul du cout 2
                int cost2 = distances[n*new_column[0][0].get_indice() + chain[j].get_indice()];
                int index = 0;
                for (int k=1; k<new_column[0].size(); k++) {
                    if (distances[n*new_column[0][k].get_indice() + chain[j].get_indice()] < cost2) {
                        cost2 = distances[n*new_column[0][k].get_indice() + chain[j].get_indice()];
                        index = k;
                    }
                }

                // Si rajouter le noeud a la fin de la derniere chaine est le meilleur choix
                if (cost1 < cost2) {
                    new_column[new_column.size()-1].push_back(chain[j]);
                }

                // Sinon, si creer une nouvelle chaine est le meilleur choix
                else {
                    vector<node> new_chain;
                    new_chain.push_back(new_column[0][index]);
                    new_chain.push_back(chain[j]);
                    new_column.push_back(new_chain);
                }

            }

        }
        data_new[indice] = new_column;
        return data_new;
    }
}











































// n est le nombre de noeuds
// best_data est le meilleur data que l'on a et que l'on veut ameliorer sur la colonne indice
vector<vector<vector<node>>> heuristic_rs(vector<vector<vector<node>>> best_data, int n, vector<int> distances, int indice) {

    // On modifie best_data seulement dans la colonne indice

    // On modifie un element de la boucle mais on ne change pas la taille
    vector<vector<vector<node>>> data_new_change = best_data;
    int cost_change;

    // On enleve un element de la boucle
    vector<vector<vector<node>>> data_new_remove = best_data;
    int cost_remove;

    // On ajoute un element a la boucle
    vector<vector<vector<node>>> data_new_add = best_data;
    int cost_add;

    // Elements de l'ancienne boucle
    vector<node> previous_loop_elements;
    for (int j=0; j<best_data[indice][0].size(); j++) {
        previous_loop_elements.push_back(best_data[indice][0][j]);
    }

    // Elements des anciennes chaines
    vector<node> previous_chains_elements;
    for (int i=1; i<best_data[indice].size(); i++) {
        for (int j=1; j<best_data[indice][i].size(); j++) {
            previous_chains_elements.push_back(best_data[indice][i][j]);
        }
    }





    // On commence par modifier data_new_change
    // On ne peut le faire que si la taille de la chaine n'est pas initialement nulle
    if (previous_chains_elements.size() >= 1) {
        vector<vector<node>> new_column_change;

        //Definition de la boucle principale et de la chaine totale
        vector<node> loop_change = previous_loop_elements;
        vector<node> chains_change = previous_chains_elements;

        // On choisit aleatoirement l'element de la boucle qui va etre echange avec un element de la chaine
        default_random_engine generator(random_device{}());
        int index_loop_change = uniform_int_distribution<>(1,previous_loop_elements.size()-1)(generator);
        int index_chains_change = uniform_int_distribution<>(0,previous_chains_elements.size()-1)(generator);

        loop_change.push_back(chains_change[index_chains_change]);
        chains_change.push_back(loop_change[index_loop_change]);
        loop_change.erase(loop_change.begin()+index_loop_change);
        chains_change.erase(chains_change.begin()+index_chains_change);

        // On ordonne la boucle et les chaines pour avoir un cout petit
        for (int j=0; j<loop_change.size()-1; j++) {
            int distance_min = distances[n*loop_change[j].get_indice() + loop_change[j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<loop_change.size(); k++) {
                if (distances[n*loop_change[j].get_indice() + loop_change[k].get_indice()] < distance_min) {
                    distance_min = distances[n*loop_change[j].get_indice() + loop_change[k].get_indice()];
                    index = k;
                }
            }
            loop_change.insert(loop_change.begin()+j+1, loop_change[index]);
            loop_change.erase(loop_change.begin()+index+1);
        }
        for (int j=0; j<chains_change.size()-1; j++) {
            int distance_min = distances[n*chains_change[j].get_indice() + chains_change[j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<chains_change.size(); k++) {
                if (distances[n*chains_change[j].get_indice() + chains_change[k].get_indice()] < distance_min) {
                    distance_min = distances[n*chains_change[j].get_indice() + chains_change[k].get_indice()];
                    index = k;
                }
            }
            chains_change.insert(chains_change.begin()+j+1, chains_change[index]);
            chains_change.erase(chains_change.begin()+index+1);
        }

        // On met la boucle dans new_column_change
        new_column_change.push_back(loop_change);

        // On cree les chaines jusqu'a ce que tous les noeuds soient dans une chaine
        for (int j=0; j<chains_change.size(); j++) {

            // Si la chaine precedente est de longueur 6, on en cree une nouvelle
            // On cherche le point de la boucle le plus proche pour commencer la chaine
            if (new_column_change[new_column_change.size()-1].size() >= 6) {
                vector<node> new_chain;
                int distance_min = distances[n*new_column_change[0][0].get_indice() + chains_change[j].get_indice()];
                int index = 0;
                for (int k=1; k<loop_change.size(); k++) {
                    if (distances[n*new_column_change[0][k].get_indice() + chains_change[j].get_indice()] < distance_min) {
                        distance_min = distances[n*new_column_change[0][k].get_indice() + chains_change[j].get_indice()];
                        index = k;
                    }
                }
                new_chain.push_back(new_column_change[0][index]);
                new_chain.push_back(chains_change[j]);
                new_column_change.push_back(new_chain);
            }

            // Sinon, on regarde le cout pour passer du dernier noeud de la derniere chaine au point que l'on considere (cost1)
            // On compare au cout en creant une nouvelle chaine avec un point de la boucle et ce point (cost2)
            // On effectue l'action de plus petit cout
            else {

                // Calcul du cout 1
                int cost1 = distances[n*new_column_change[new_column_change.size()-1][new_column_change[new_column_change.size()-1].size()-1].get_indice() + chains_change[j].get_indice()];

                // Calcul du cout 2
                int cost2 = distances[n*new_column_change[0][0].get_indice() + chains_change[j].get_indice()];
                int index = 0;
                for (int k=1; k<new_column_change[0].size(); k++) {
                    if (distances[n*new_column_change[0][k].get_indice() + chains_change[j].get_indice()] < cost2) {
                        cost2 = distances[n*new_column_change[0][k].get_indice() + chains_change[j].get_indice()];
                        index = k;
                    }
                }

                // Si rajouter le noeud a la fin de la derniere chaine est le meilleur choix
                if (cost1 < cost2) {
                    new_column_change[new_column_change.size()-1].push_back(chains_change[j]);
                }

                // Sinon, si creer une nouvelle chaine est le meilleur choix
                else {
                    vector<node> new_chain;
                    new_chain.push_back(new_column_change[0][index]);
                    new_chain.push_back(chains_change[j]);
                    new_column_change.push_back(new_chain);
                }

            }

        }
        data_new_change[indice] = new_column_change;
        cost_change = cost_solution(data_new_change, distances);
    }
    // Sinon, on met le cout a l'infini pour dire que ce n'est pas possible
    else {
        cost_change = INT_MAX;
    }





    // Ensuite, on modifie data_new_remove
    // On ne peut le faire que si la taille de la boucle initiale est au moins egale a trois
    if (previous_loop_elements.size() >= 3) {
        vector<vector<node>> new_column_remove;

        //Definition de la boucle principale et de la chaine totale
        vector<node> loop_remove = previous_loop_elements;
        vector<node> chains_remove = previous_chains_elements;

        // On choisit aleatoirement l'element de la boucle qui va etre echange avec un element de la chaine
        default_random_engine generator(random_device{}());
        int index_loop_remove = uniform_int_distribution<>(1,previous_loop_elements.size()-1)(generator);

        chains_remove.push_back(loop_remove[index_loop_remove]);
        loop_remove.erase(loop_remove.begin()+index_loop_remove);

        // On ordonne la boucle et les chaines pour avoir un cout petit
        for (int j=0; j<loop_remove.size()-1; j++) {
            int distance_min = distances[n*loop_remove[j].get_indice() + loop_remove[j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<loop_remove.size(); k++) {
                if (distances[n*loop_remove[j].get_indice() + loop_remove[k].get_indice()] < distance_min) {
                    distance_min = distances[n*loop_remove[j].get_indice() + loop_remove[k].get_indice()];
                    index = k;
                }
            }
            loop_remove.insert(loop_remove.begin()+j+1, loop_remove[index]);
            loop_remove.erase(loop_remove.begin()+index+1);
        }
        for (int j=0; j<chains_remove.size()-1; j++) {
            int distance_min = distances[n*chains_remove[j].get_indice() + chains_remove[j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<chains_remove.size(); k++) {
                if (distances[n*chains_remove[j].get_indice() + chains_remove[k].get_indice()] < distance_min) {
                    distance_min = distances[n*chains_remove[j].get_indice() + chains_remove[k].get_indice()];
                    index = k;
                }
            }
            chains_remove.insert(chains_remove.begin()+j+1, chains_remove[index]);
            chains_remove.erase(chains_remove.begin()+index+1);
        }

        // On met la boucle dans new_column_remove
        new_column_remove.push_back(loop_remove);

        // On cree les chaines jusqu'a ce que tous les noeuds soient dans une chaine
        for (int j=0; j<chains_remove.size(); j++) {

            // Si la chaine precedente est de longueur 6, on en cree une nouvelle
            // On cherche le point de la boucle le plus proche pour commencer la chaine
            if (new_column_remove[new_column_remove.size()-1].size() >= 6) {
                vector<node> new_chain;
                int distance_min = distances[n*new_column_remove[0][0].get_indice() + chains_remove[j].get_indice()];
                int index = 0;
                for (int k=1; k<loop_remove.size(); k++) {
                    if (distances[n*new_column_remove[0][k].get_indice() + chains_remove[j].get_indice()] < distance_min) {
                        distance_min = distances[n*new_column_remove[0][k].get_indice() + chains_remove[j].get_indice()];
                        index = k;
                    }
                }
                new_chain.push_back(new_column_remove[0][index]);
                new_chain.push_back(chains_remove[j]);
                new_column_remove.push_back(new_chain);
            }

            // Sinon, on regarde le cout pour passer du dernier noeud de la derniere chaine au point que l'on considere (cost1)
            // On compare au cout en creant une nouvelle chaine avec un point de la boucle et ce point (cost2)
            // On effectue l'action de plus petit cout
            else {

                // Calcul du cout 1
                int cost1 = distances[n*new_column_remove[new_column_remove.size()-1][new_column_remove[new_column_remove.size()-1].size()-1].get_indice() + chains_remove[j].get_indice()];

                // Calcul du cout 2
                int cost2 = distances[n*new_column_remove[0][0].get_indice() + chains_remove[j].get_indice()];
                int index = 0;
                for (int k=1; k<new_column_remove[0].size(); k++) {
                    if (distances[n*new_column_remove[0][k].get_indice() + chains_remove[j].get_indice()] < cost2) {
                        cost2 = distances[n*new_column_remove[0][k].get_indice() + chains_remove[j].get_indice()];
                        index = k;
                    }
                }

                // Si rajouter le noeud a la fin de la derniere chaine est le meilleur choix
                if (cost1 < cost2) {
                    new_column_remove[new_column_remove.size()-1].push_back(chains_remove[j]);
                }

                // Sinon, si creer une nouvelle chaine est le meilleur choix
                else {
                    vector<node> new_chain;
                    new_chain.push_back(new_column_remove[0][index]);
                    new_chain.push_back(chains_remove[j]);
                    new_column_remove.push_back(new_chain);
                }

            }

        }
        data_new_remove[indice] = new_column_remove;
        cost_remove = cost_solution(data_new_remove, distances);
    }
    // Sinon, on met le cout a l'infini pour dire que ce n'est pas possible
    else {
        cost_remove = INT_MAX;
    }





    // Finalement, on modifie data_new_add
    // On ne peut le faire que si la taille de la boucle initiale est inferieur ou egale a 29
    if ((previous_loop_elements.size() <= 29) && (previous_chains_elements.size() >= 2)) {
        vector<vector<node>> new_column_add;

        //Definition de la boucle principale et de la chaine totale
        vector<node> loop_add = previous_loop_elements;
        vector<node> chains_add = previous_chains_elements;

        // On choisit aleatoirement l'element de la boucle qui va etre echange avec un element de la chaine
        default_random_engine generator(random_device{}());
        int index_chains_add = uniform_int_distribution<>(0,previous_chains_elements.size()-1)(generator);

        loop_add.push_back(chains_add[index_chains_add]);
        chains_add.erase(chains_add.begin()+index_chains_add);

        // On ordonne la boucle et les chaines pour avoir un cout petit
        for (int j=0; j<loop_add.size()-1; j++) {
            int distance_min = distances[n*loop_add[j].get_indice() + loop_add[j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<loop_add.size(); k++) {
                if (distances[n*loop_add[j].get_indice() + loop_add[k].get_indice()] < distance_min) {
                    distance_min = distances[n*loop_add[j].get_indice() + loop_add[k].get_indice()];
                    index = k;
                }
            }
            loop_add.insert(loop_add.begin()+j+1, loop_add[index]);
            loop_add.erase(loop_add.begin()+index+1);
        }
        for (int j=0; j<chains_add.size()-1; j++) {
            int distance_min = distances[n*chains_add[j].get_indice() + chains_add[j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<chains_add.size(); k++) {
                if (distances[n*chains_add[j].get_indice() + chains_add[k].get_indice()] < distance_min) {
                    distance_min = distances[n*chains_add[j].get_indice() + chains_add[k].get_indice()];
                    index = k;
                }
            }
            chains_add.insert(chains_add.begin()+j+1, chains_add[index]);
            chains_add.erase(chains_add.begin()+index+1);
        }

        // On met la boucle dans new_column_add
        new_column_add.push_back(loop_add);

        // On cree les chaines jusqu'a ce que tous les noeuds soient dans une chaine
        for (int j=0; j<chains_add.size(); j++) {

            // Si la chaine precedente est de longueur 6, on en cree une nouvelle
            // On cherche le point de la boucle le plus proche pour commencer la chaine
            if (new_column_add[new_column_add.size()-1].size() >= 6) {
                vector<node> new_chain;
                int distance_min = distances[n*new_column_add[0][0].get_indice() + chains_add[j].get_indice()];
                int index = 0;
                for (int k=1; k<loop_add.size(); k++) {
                    if (distances[n*new_column_add[0][k].get_indice() + chains_add[j].get_indice()] < distance_min) {
                        distance_min = distances[n*new_column_add[0][k].get_indice() + chains_add[j].get_indice()];
                        index = k;
                    }
                }
                new_chain.push_back(new_column_add[0][index]);
                new_chain.push_back(chains_add[j]);
                new_column_add.push_back(new_chain);
            }

            // Sinon, on regarde le cout pour passer du dernier noeud de la derniere chaine au point que l'on considere (cost1)
            // On compare au cout en creant une nouvelle chaine avec un point de la boucle et ce point (cost2)
            // On effectue l'action de plus petit cout
            else {

                // Calcul du cout 1
                int cost1 = distances[n*new_column_add[new_column_add.size()-1][new_column_add[new_column_add.size()-1].size()-1].get_indice() + chains_add[j].get_indice()];

                // Calcul du cout 2
                int cost2 = distances[n*new_column_add[0][0].get_indice() + chains_add[j].get_indice()];
                int index = 0;
                for (int k=1; k<new_column_add[0].size(); k++) {
                    if (distances[n*new_column_add[0][k].get_indice() + chains_add[j].get_indice()] < cost2) {
                        cost2 = distances[n*new_column_add[0][k].get_indice() + chains_add[j].get_indice()];
                        index = k;
                    }
                }

                // Si rajouter le noeud a la fin de la derniere chaine est le meilleur choix
                if (cost1 < cost2) {
                    new_column_add[new_column_add.size()-1].push_back(chains_add[j]);
                }

                // Sinon, si creer une nouvelle chaine est le meilleur choix
                else {
                    vector<node> new_chain;
                    new_chain.push_back(new_column_add[0][index]);
                    new_chain.push_back(chains_add[j]);
                    new_column_add.push_back(new_chain);
                }

            }

        }
        data_new_add[indice] = new_column_add;
        cost_add = cost_solution(data_new_add, distances);
    }
    // Sinon, on met le cout a l'infini pour dire que ce n'est pas possible
    else {
        cost_add = INT_MAX;
    }





    // On compare les couts et on renvoie le meilleur choix possible
    if (cost_change <= min(cost_remove, cost_add)) {
        return data_new_change;
    }
    else if (cost_remove <= cost_add) {
        return data_new_remove;
    }
    else {
        return data_new_add;
    }
}
