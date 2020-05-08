#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <zmq.h>
#include "matMsg.h"

using namespace Eigen;
using namespace std;

int main() {
    int epoch = 1000;
    int R_size = 1193997;//m=1000, n=1000, R_size = 12438
    int m = 35934;//1000
    int n = 18664;//1000

    for (int j = 0; j < epoch; ++j) {
        for (int i = 0; i < R_size; ++i) {
            cout<<"ep:"<< j<<","<< i<<"'s sample" <<endl;
            matMsg_agg(15, 1); // g_u
            matMsg_agg(15, 1); // g_v
            matMsg_agg(m, 1); // g_s
            matMsg_agg(n, 1); // g_w

    }

    }

}