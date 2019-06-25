#ifndef __SOURCE_H_INCLUDED__
#define __SOURCE_H_INCLUDED__
#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
using namespace std;

namespace oprp {    
    int rank;
    int size;
    int passwords_total;
    map< string, set< string > > passwords_buff;
    map< string, bool> broken;


    string alpha = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ./";

    namespace {
        vector< string > roots;

        void is_valid(string &s) {
            char * result;
            for(auto & vec : passwords_buff) {
                result = crypt(s.data(), vec.first.data());

                int i = 0;
                for(auto & str : vec.second) {
                    if(!broken[str]) {
                        if(strcmp(result, str.data()) == 0) {
                            cout << str << " = " << s << endl << vec.first.data();
                            broken[str] = true;
                        }
                    }
                }
            }
        }

        void worker_recursive(string s, int h, int max) {
            if (h < max) {
                for (int i = 0; i < 65; i++) {
                    string stemp = s + alpha[i];
                    //if(rank == size - 1)
                    //    cout << stemp << endl;
                    is_valid(s);
                    worker_recursive(stemp, h + 1, max);
                }
            }
        }

        void init_root(string s, int h) {
            if(h < 2) {
                for(int i = 0; i < 65; i++) {
                    string stemp = s + alpha[i];

                    //cout << stemp << endl;
                    if(rank == 0)
                        is_valid(stemp);

                    if(stemp.size() == 2) {
                        roots.push_back(stemp);
                    }

                    init_root(stemp, h+1);
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
        read_entry(file);
        init_root("", 0);
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

#pragma omp parallel for num_threads(2)
        for(int i = init; i <= end; i++) {
            for(int l = 3; l <= 8; l++ ) {
                worker_recursive(roots[i], 2, l);
            }
        }
    }

} // namespace sort

#endif