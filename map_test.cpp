#include <map>
#include <vector>
#include <iostream>


using namespace std;


int main() {
    map<string, int> m;
    int il[3] = {1, 2, 3};
    vector<string> vs;
    vs.push_back("abc");
    vs.push_back("bac");
    vs.push_back("cba");

    vector<string>::iterator it;
    int i = 0;
    for (it=vs.begin(); it!=vs.end(); it++) {
        m[*it] = il[i];
        i++;
    }

    map<string, int>::iterator iit;
    pair<string, int> p("hello", 6);
    m.insert(p);
    for (iit=m.begin(); iit!=m.end(); iit++) {
        cout << iit->first << " " << iit->second << endl;
    }
    return 0;

}
