//
// Created by Yanjun Zhang on 2020-04-29.
//

#ifndef LASTFM_DPCOUPLING_READLASTFM_H
#define LASTFM_DPCOUPLING_READLASTFM_H
#include <vector>
#include <string>
using namespace std;

void read_data(std::string filename, int m, int n, vector<vector<double>> &vec);
void split_data(vector<vector<double>> &vec_in, vector<vector<double>> &vec_train, vector<vector<double>> &vec_test);

#endif //LASTFM_DPCOUPLING_READLASTFM_H
