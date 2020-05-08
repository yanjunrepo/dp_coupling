//
// Created by Yanjun zhang on 2020-03-03.
//

#ifndef DP_COUPLING_READMOVIELENS_H
#define DP_COUPLING_READMOVIELENS_H
#include <vector>
#include <string>
using namespace std;

void read_data(std::string filename, int m, int n, vector<vector<double>> &vec);

void trim(int c, vector<vector<double>> &vec_in, vector<vector<double>> &vec_out);

#endif //DP_COUPLING_READMOVIELENS_H
