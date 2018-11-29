#include <iostream>
#include "read_write.h"

using namespace std;

int main()
{
    vector<int> D;
    vector<node> distribution;
    vector<node> terminal;

    read_file(string("grenoble/distances.csv"),string("grenoble/nodes.csv"), D, distribution, terminal);
    return 0;
}
