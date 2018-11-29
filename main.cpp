#include "read_write.h"
#include "kiro.h"

int main()
{
    srand(clock());
    vector<int> D;
    vector<node> distribution;
    vector<node> terminal;

    int n = read_file(string("nice/distances.csv"),string("nice/nodes0.csv"), D, distribution, terminal);

    vector<vector<node>> data = clustering(distribution, terminal, D);

    int N = 1000000;
    int cnt = 0;

    while(cnt<N){
        vector<int> couple(2);
        couple = echange_aleat(data,distribution, terminal);

    }

    write(data, string("resu_nice.txt"));

    cout << n << endl;
    return 0;
}
