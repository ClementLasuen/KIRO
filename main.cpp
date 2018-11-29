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

    switch_chain(data,D);

    write(data, string("resu_nice.txt"));

    cout << n << endl;
    return 0;
}
