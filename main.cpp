#include "read_write.h"
#include "kiro.h"

int main()
{
    srand(clock());
    vector<int> D;
    vector<node> distribution;
    vector<node> terminal;

    int n = read_file(string("pim/distances.csv"),string("pim/nodes0.csv"), D, distribution, terminal);

    vector<vector<vector<node>>> data = clustering(distribution, terminal, D);

    int N = 100000;
    int cnt = 0;

    //switch_chain(data,D);


//    int c = cost_solution( data, D);
//    int c_test;
//    while(cnt<N){
//        if (cnt%(N/100)==0) cout << cnt/(N/100) << endl;
//        vector<vector<vector<node>>> data_test = data;
//        echange_aleat(data_test,distribution, terminal);
//        c_test = cost_solution(data_test, D);
//        if (c_test< c) {
//            data = data_test;
//            c = c_test;
//            cout << c_test << endl;
//        }
//        cnt+=1;
//    }

//    write(data, string("resu_pim.txt"));

//    cout << c << endl;















    int n_clustering = distribution.size() + terminal.size();
    vector<int> index_clusters;
    for (int i=0; i<terminal.size(); i++) {
        int index = 0;
        int min_distance = D[n*terminal[i].get_indice() + distribution[0].get_indice()];
        for (int j=0; j<distribution.size(); j++) {
            if (D[n*terminal[i].get_indice() + distribution[j].get_indice()] < min_distance) {
                min_distance = D[n*terminal[i].get_indice() + distribution[j].get_indice()];
                index = j;
            }
        }
        index_clusters.push_back(index);
    }
    vector<vector<node>> data_clustering(distribution.size());
    for (int i=0; i<distribution.size(); i++) {
        data_clustering[i].push_back(distribution[i]);
    }
    for (int i=0; i<terminal.size(); i++) {
        data_clustering[index_clusters[i]].push_back(terminal[i]);
    }
    for (int i=0; i<data_clustering.size(); i++) {
        // On ordonne data pour le ieme distributeur de facon a avoir le (j+1)eme element qui est le proche voisin
        // du jeme element parmi ceux qu'on a pas encore parcouru pour tout j
        for (int j=0; j<data_clustering[i].size()-1; j++) {
            int distance_min = D[n*data_clustering[i][j].get_indice() + data_clustering[i][j+1].get_indice()];
            int index = j+1;
            for (int k=j+2; k<data_clustering[i].size(); k++) {
                if (D[n*data_clustering[i][j].get_indice() + data_clustering[i][k].get_indice()] < distance_min) {
                    distance_min = D[n*data_clustering[i][j].get_indice() + data_clustering[i][k].get_indice()];
                    index = k;
                }
            }
            data_clustering[i].insert(data_clustering[i].begin()+j+1, data_clustering[i][index]);
            data_clustering[i].erase(data_clustering[i].begin()+index+1);
        }
    }

    int rep = 10000;
    int best_cost = cost_solution(data, D);
    cout << best_cost << endl;
    vector<vector<vector<node>>> best_data = clustering(distribution, terminal, D);
    for (int indice=0; indice<data_clustering.size(); indice++) {
        for (int i=0; i<rep; i++) {
            vector<vector<vector<node>>> new_data = heuristic_loop(data_clustering, n_clustering, D, indice);
            int new_cost = cost_solution(new_data, D);
            if (new_cost < best_cost) {
                best_cost = new_cost;
                best_data = new_data;
                cout << best_cost << endl;
            }
        }
        vector<node> best_loop_chain;
        for (int j=0; j<best_data[indice][0].size(); j++) {
            best_loop_chain.push_back(best_data[indice][0][j]);
        }
        for (int k=1; k<best_data[indice].size(); k++) {
            for (int j=1; j<best_data[indice][k].size(); j++) {
                best_loop_chain.push_back(best_data[indice][k][j]);
            }
        }
        data_clustering[indice] = best_loop_chain;
        cout << "indice : " << indice << endl;
        write(best_data, string("resu_pim.txt"));
    }

    cout << best_cost << endl;




    return 0;
}
