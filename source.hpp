#ifndef __SOURCE_H_INCLUDED__
#define __SOURCE_H_INCLUDED__
#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
using namespace std;

namespace oprp {    
    int rank;
    int size;
    int passwords_total;
    map< string, set< string > > passwords_buff;
    map< string, bool> broken;

    string alpha = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz./";

    namespace {
        vector< string > roots;

        void is_valid(string &s, struct crypt_data & c_data) {
            char * result;
            for(auto & vec : passwords_buff) {
                result = crypt_r(s.c_str(), vec.first.c_str(), &c_data);

                int i = 0;
                for(auto & str : vec.second) {
                    if(!broken[str]) {
                        if(strcmp(result, str.c_str()) == 0) {
                            cout << str << " = " << s << endl;
                            broken[str] = true;
                        }
                    }
                }
            }
        }

        void worker_recursive(string s, int h, int max, struct crypt_data & c_data) {
            if (h < max) {
                for (int i = 0; i < 64; i++) {
                    string stemp = s + alpha[i];
                    //if(rank == size - 1)
                    cout << stemp << endl;
                    is_valid(s, c_data);
                    worker_recursive(stemp, h + 1, max, c_data);
                }
            }
        }

        void init_root(string s, int h, struct crypt_data & c_data) {
            if(h < 2) {
                for(int i = 0; i < 65; i++) {
                    string stemp = s + alpha[i];

                    //cout << stemp << endl;
                    if(rank == 0)
                        is_valid(stemp, c_data);

                    if(stemp.size() == 2) {
                        roots.push_back(stemp);
                    }

                    init_root(stemp, h+1, c_data);
                }
            }
        }

        void read_entry(string file) {
            ifstream myfile(file);
            string line;

            if(myfile.is_open()) {
                while (getline(myfile, line)) {
                    broken[line] = false;
                    string salt = "";
                    salt += line[0];
                    salt += line[1];
                    passwords_buff[salt].insert(line);
                    passwords_total++;
                }
            } else 
                cout << "Erro ao abrir arquivo" << endl;

            myfile.close();
        }
    }

    void start_up(string file) {
        //c_data.initialized = 0;
        read_entry(file);
        struct crypt_data c_data;
        c_data.initialized = 0;
        init_root("", 0, c_data);
    }

    void run_recursive() {
        int lenght = roots.size()/size;

        int init = rank * lenght;
        int end  = init + lenght;

        if(rank != 0)
            init++;

        if(rank == size-1) {
            end = roots.size() - 1;
        }

        //cout << "rank: " << rank << ", init: " << init << ", end: " << end << endl;
        struct crypt_data c_data;
        c_data.initialized = 0;
#pragma omp parallel for num_threads(2) private(c_data)
        for(int i = init; i <= end; i++) {
            for(int l = 3; l <= 8; l++ ) {
                worker_recursive(roots[i], 2, l, c_data);
            }
        }
    }

} // namespace sort

#endif