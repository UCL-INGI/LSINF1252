#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

string xorCust(string s1, string s2)
{
    string out;
    for (unsigned int i = 0 ; s1.length() > i && s2.length() > i ; i++)
    {
        out += (char)((s1[i]+s2[i])/2);
    }
    string ret(out);
    return ret;
}

int main(int argc, char** argv)
{
    string un(argv[1]);
    string de(argv[2]);
    cout << xorCust(un, de) << endl;
    return 0;
}
