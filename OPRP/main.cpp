#include <iostream>
#include <bits/stdc++.h>
#include "source.hpp"

using namespace std;

int main () {
    string str;
    while(cin >> str)
        oprp::passwords_buff.insert(str);

    oprp::run_recursive();
    return 0;
}