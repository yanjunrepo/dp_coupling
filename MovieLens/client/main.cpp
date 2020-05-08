#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <zmq.h>
#include <fstream>
#include "readMovielens.h"
#include "dpCoupling.h"


using namespace Eigen;
using namespace std;

int main() {
    int m = 71567; //m = #user 1000
    int n = 10681; //n = #item 1000
    int c = 100;

    string dirname = "/data";
    string filename = dirname + "/ra.train";

    vector<vector<double> > R_o;
    read_data(filename, m, n, R_o);
    cout<< "Number of ratings in original train =" <<R_o.size()<<endl;

    filename = dirname + "/ra.test";
    vector<vector<double> > Rt_o;
    read_data(filename, m, n, Rt_o);
    cout<< "Number of ratings in original test =" <<Rt_o.size()<<endl;

    vector<vector<double> > R;
    trim(c, R_o, R);
    cout<< "Number of ratings in trimmed train =" <<R.size()<<endl;
    vector<vector<double> > Rt;
    trim(c, Rt_o, Rt);
    cout<< "Number of ratings in trimmed test =" <<Rt.size()<<endl;

    dpCoupling dpC(R, Rt, m, n);

    /**
     * Train the model with the supplied parameters.
     *
     * @param alpha         The learning rate, e.g. 0.01.
     * @param zata          Regularization parameter.
     * @param d             Dimension of the latent space.
     * @param epoch         # training epoch.
     * @param sigma         # dp-Gaussian variance.
     */
    dpC.train(0.0001, 0.15, 15, 1000, 0.1);


    return 0;
}
