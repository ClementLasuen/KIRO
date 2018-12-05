#include "read_write.h"
#include "kiro.h"

int main()
{
    srand(clock());
    vector<int> D;
    vector<node> distribution;
    vector<node> terminal;

    int n = read_file(string("nice/distances.csv"),string("nice/nodes0.csv"), D, distribution, terminal);

    vector<vector<node> > data = clustering(distribution, terminal, D);

    int N = 1000000;
    int cnt = 0;

    //switch_chain(data,D);


    int c = cost_solution( data, D);
    cout << c << endl;
    int c_test;
    while(cnt<N){
        vector<vector<node> > data_test = data;
        echange_aleat(data_test,distribution, terminal);
        c_test = cost_solution(data_test, D);
        if (c_test< c) {
            data = data_test;
            c = c_test;
        }
        if (cnt%(N/100)==0) cout<<cnt/(N/100)<<endl;
        cnt+=1;
    }

    write(data, string("resu_nice.txt"));

    cout << c << endl;
    return 0;
}
