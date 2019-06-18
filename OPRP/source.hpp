#ifndef __SOURCE_H_INCLUDED__
#define __SOURCE_H_INCLUDED__
#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
using namespace std;

namespace oprp {    
    set < string > passwords_buff; 
    char alpha[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ./";

    namespace {
        bool is_valid(string &s) {
            char *result;
            int ok;
            for (auto &a : passwords_buff) {
                result = crypt(a.data(), s.data());
                if (strcmp(result, s.data()) == 0)
                    cout << "senha quebrada";
            }
        }

        void worker_recursive(string s, int h, int max) {
            if (h <= max) {
                for (int i = 0; i < 65; i++) {
                    string stemp = s + alpha[i];
                    cout << stemp << endl;
                    std::getchar();
                    //is_valid(stemp);
                    worker_recursive(stemp, h + 1, max);
                }
            }
        }
    }

    void run_recursive() {
#pragma omp parallel for
        for(int i = 1; i < 8; i++) {
            for(int i = 0; i < 65; i++) {
                string str = "";
                str += alpha[i];
                worker_recursive(str, 1, i);
            }
        }
    }

    void run_omp() {
#pragma omp parallel for
        for (long long unsigned i = 0; i < 318644812890625; i++) {

        }
    }
} // namespace sort

#endif