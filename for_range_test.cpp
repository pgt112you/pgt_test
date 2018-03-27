#include "pgt_test.h"
#include <vector>
#include <iostream>
#include <stack>


using namespace std;

int main() {
    vector<int> v = {1,2,3,4,5,6,7,8,9};
    for (auto &i : v) {
        i += 10;
        i *= 2;
    }
    for (auto ii : v) {
        cout << ii << endl;
    }

    cout << "---------------------------------------------" << endl;
    stack<int, vector<int>> st(v) ;
    st.pop();
    st.push(9);
    for (auto ii : v) {
        cout << ii << endl;
    }

    cout << "---------------------------------------------" << endl;
    //decltype(st.size()) index = 0;
    //while (index < st.size()) {
    //    cout << st[index] << endl;
    //}
    while (st.size() > 0) {
        auto value = st.top();
        cout << value << endl;
        st.pop();
    }


}
