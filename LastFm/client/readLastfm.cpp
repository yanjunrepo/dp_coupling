//
// Created by Yanjun Zhang on 2020-04-29.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
#include "readLastfm.h"

using namespace std;

void read_data(std::string filename, int m, int n, vector<vector<double>> &vec){
    ifstream file;
    file.open (filename, ifstream::in);
    int i= 0;

    if (file.is_open()){
        while (file.good() && i<=m){
            string s;
            if (!getline( file, s )) break;
            stringstream ss( s );
            vector<double> tp;

            while (ss){
                string substr;
                getline(ss, substr, ',');
                if(!substr.empty()){
                    tp.push_back( stod(substr) );
                }
            }
            if (tp[1] <= n){
                vec.push_back(tp);
                //t+=1;
            } else {
                tp.clear();
            }
            i= tp[0];

        }

    }
    else
    {
        cout << "Error opening file";
    }

}

void split_data(vector<vector<double>> &vec_in, vector<vector<double>> &vec_train, vector<vector<double>> &vec_test){
    int size_train = floor(vec_in.size() * 0.8);
    int size_test = vec_in.size() - size_train;

    //generate a random sequence [0, vec_in.size()-1]
    int size = vec_in.size();
    int a[size];
    for (int k = 0; k < vec_in.size(); ++k) {a[k]=k;}
    srand(time(0)); //Use current time as seed for random generator
    for (int k = vec_in.size()-1; k >=1 ; --k) {swap(a[k-1], a[rand()%k]);}

    for (int i = 0; i < size_train; ++i) {
        vec_train.push_back(vec_in[a[i]]);
    }

    for (int i = 0; i < size_test; ++i) {
        vec_test.push_back(vec_in[a[i+size_train]]);
    }


}