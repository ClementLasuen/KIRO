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
    int rep = 1000;
    vector<vector<vector<node>>> best_data = clustering(distribution, terminal, D);
    int best_cost = cost_solution(best_data, D);
    cout << best_cost << endl;
    for (int indice=0; indice<best_data.size(); indice++) {
        for (int i=0; i<rep; i++) {
            vector<vector<vector<node>>> new_data = heuristic_loop(best_data, n_clustering, D, indice);
            int new_cost = cost_solution(new_data, D);
            if (new_cost < best_cost) {
                best_cost = new_cost;
                best_data = new_data;
                cout << best_cost << endl;
            }
        }
        cout << "indice : " << indice << endl;
        write(best_data, string("resu_pim.txt"));
    }

    cout << best_cost << endl;




    return 0;
}
