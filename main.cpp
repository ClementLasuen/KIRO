#include "read_write.h"
#include "kiro.h"

int main()
{
    srand(clock());
    vector<int> D;
    vector<node> distribution;
    vector<node> terminal;

    int n = read_file(string("pim/distances.csv"),string("pim/nodes0.csv"), D, distribution, terminal);

    vector<vector<node> > data = clustering(distribution, terminal, D);

    int N = 10000;
    int cnt = 0;

    //switch_chain(data,D);


    int c = cost_solution( data, D);
    int c_test;
    while(cnt<N){
        vector<vector<node> > data_test = data;
        echange_aleat(data_test,distribution, terminal);
        c_test = cost_solution(data_test, D);
        cout << c_test << endl;
        if (c_test< c) {
            data = data_test;
            c = c_test;
        }
        cnt+=1;
    }

    write(data, string("resu_pim.txt"));

    cout << c << endl;
    return 0;
}
