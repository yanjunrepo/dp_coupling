//
// Created by Yanjun zhang on 2020-03-03.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "readMovielens.h"

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
                getline(ss, substr, ':');
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

void trim(int c, vector<vector<double>> &vec_in, vector<vector<double>> &vec_out){
    int k = vec_in[1].at(0);
    int kp = vec_in[0].at(0);
    int j = 0;
    int i = 0;

    while (i < vec_in.size()-1){

        while(k == kp && j<c && i < vec_in.size()-1){
            vec_out.push_back(vec_in[i]);
            j++;
            i++;
            k = vec_in[i].at(0);
            kp = vec_in[i-1].at(0);
        }

        k = vec_in[i].at(0);
        kp = vec_in[i-1].at(0);
        if(k != kp){
            j=0;
        }

        i++;

    }

    //for (int i = 1; i <= vec_in.size()-1; ++i) {

    //}
}