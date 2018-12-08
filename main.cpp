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


    int c = cost_solution( data, D);
    int c_test;
    while(cnt<N){
        if (cnt%(N/100)==0) cout << cnt/(N/100) << endl;
        vector<vector<vector<node>>> data_test = data;
        echange_aleat(data_test,distribution, terminal);
        c_test = cost_solution(data_test, D);
        if (c_test< c) {
            data = data_test;
            c = c_test;
            cout << c_test << endl;
        }
        cnt+=1;
    }

//    write(data, string("resu_pim.txt"));

    cout << c << endl;


    return 0;
}
