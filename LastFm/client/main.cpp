#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include "readLastfm.h"
#include "dpCoupling.h"

using namespace Eigen;
using namespace std;

int main() {
    string dirname = "/data/preprocess";;
    string filename = dirname + "/merge.csv";
    int m = 35934; //m = #user 1000
    int n = 18664; //n = #item 1000
    vector<vector<double> > R_all;
    vector<vector<double> > R;
    vector<vector<double> > Rt;
    read_data(filename, m, n, R_all);
    cout<< "Number of ratings in the dataset =" <<R_all.size()<<endl;
    split_data(R_all, R, Rt);
    cout<< "Number of ratings in the training dataset =" <<R.size()<<endl;
    cout<< "Number of ratings in the test dataset =" <<Rt.size()<<endl;

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
    dpC.train(0.0001, 0.15, 15, 1000, 0.2);

}