#include <iostream>
#include "read_write.h"

using namespace std;

int main()
{
    vector<int> D;
    vector<node> distribution;
    vector<node> terminal;

    int n = read_file(string("grenoble/distances.csv"),string("grenoble/nodes0.csv"), D, distribution, terminal);

    cout << n << endl;
    return 0;
}
