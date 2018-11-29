#include "read_write.h"
#include "kiro.h"

int main()
{
    vector<int> D;
    vector<node> distribution;
    vector<node> terminal;

    int n = read_file(string("pim/distances.csv"),string("pim/nodes0.csv"), D, distribution, terminal);

    vector<vector<node>> data = clustering(distribution, terminal, D);

    write(data, string("resu_pim.txt"));

    cout << n << endl;
    return 0;
}
