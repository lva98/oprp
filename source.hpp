#ifndef __SOURCE_H_INCLUDED__
#define __SOURCE_H_INCLUDED__
#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
using namespace std;

struct gonna_cry {
    int missing;
    set<string> buff;

    gonna_cry() {
        missing = 0;
    }
};

using gonna_cry = struct gonna_cry;

namespace oprp {    
    int rank;
    int size;
    int passwords_total;
    int cont[2] {};
    map< string, gonna_cry > passwords;
    map< string, bool> broken;
    map< string, struct crypt_data > c_data_global[2];
    list< char * > rootsx[2]; //Lista linkada B)
    string alpha = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz/.";

    namespace {
        vector< string > roots;

        void is_valid(const char * s, map<string, struct crypt_data> & c_data) {
            //Rank:  2	cifra(180): Hu9GrIJkNMpr2	senha: 5Im
            char * result;
            map< string, gonna_cry >::iterator it;
            for(it = passwords.begin(); it != passwords.end(); it++) {
                if(it->second.missing > 0) {
                    const char * salt = it->first.c_str();
                    struct crypt_data * cd = &c_data[it->first];
                    result = crypt_r(s, salt, cd);

                    for(auto & str : it->second.buff) {
                        if(strcmp(result, str.c_str()) == 0) {
                            cout << str << " = " << s << endl;
                            it->second.missing--;
                        }
                    }
                }
            }
        }

        void worker_recursive(const char * s, int h, int max, map<string, struct crypt_data> & c_data, int id) {
            if (h < max) {
                for (int i = 0; i < 64; i++) {
                    char stemp[h+2];
                    strcpy(stemp, s);
                    stemp[h] = alpha[i];
                    stemp[h+1] = '\0';
                    //rootsx.push_back(string(stemp));

                    //if(h + 1 == max)
                    is_valid(stemp, c_data);

                    if (h+1 == max) {
                        cont[id]++;
                        //char * sstemp = (char *) malloc(sizeof(char) * h+2);
                        //strcpy(sstemp, stemp);
                    //#pragma omp critical
                        rootsx[id].push_back(&stemp[0]);
                        //cout << sstemp << endl;
                    }
                    worker_recursive(&stemp[0], h + 1, max, c_data, id);
                }
            }
        }

        void init_root(string s, int h, map<string, struct crypt_data> & c_data) {
            if(h < 2) {
                for(int i = 0; i < 64; i++) {
                    string stemp = "";
                    stemp = s + alpha[i];

                    if(rank == 0) {
                        is_valid(stemp.c_str(), c_data);
                    }

                    if(h == 1) {
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
                    if(passwords.count(salt) == 0)
                        passwords[salt] = gonna_cry();

                    passwords[salt].buff.insert(line);
                    passwords[salt].missing++;
                    passwords_total++;
                }
            } else 
                cout << "Erro ao abrir arquivo" << endl;

            myfile.close();

            /* Printar estrutura passwords
            if(rank == 0) {
                map< string, gonna_cry >::iterator it;
                for(it = passwords.begin(); it != passwords.end(); it++) {
                    cout << it->first << ": " << it->second.missing << endl;

                    for(auto & str : it->second.buff) {
                        cout << "   " << str << endl;
                    }
                }
            }
            */
            

            for(auto & vec : passwords) {
                c_data_global[0][vec.first].initialized = 0;
                c_data_global[1][vec.first].initialized = 0;
            }
        }
    }

    void start_up(string file) {
        //c_data.initialized = 0;
        read_entry(file);
        init_root("", 0, c_data_global[0]);
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

        //cout << "rank: " << rank << ", " << "init: " << init << ", " << "end: " << end << endl;
        
        //for(int k = 3; k <= 8; k++) {
        int i;
        #pragma omp parallel for num_threads(2) shared(c_data_global, broken) private(i)                                                                                                                                                                                                                                                                                                                                                                                                         
        for(i = init; i <= end; i++) {
            int id = omp_get_thread_num();
            worker_recursive(roots[i].c_str(), 2, 5, c_data_global[id], id);
        }

        #pragma omp parallel sections
        {
            #pragma omp section
            { 
                int id = omp_get_thread_num();
                int size = roots[id].size();
                for(auto &str : rootsx[id])
                    worker_recursive(str, 5, 6, c_data_global[id], id);
            }

            #pragma omp section
            { 
                int id = omp_get_thread_num();
                int size = roots[id].size();
                for(auto &str : rootsx[id])
                    worker_recursive(str, 5, 6, c_data_global[id], id);
            }
        }
    }

} // namespace sort

#endif