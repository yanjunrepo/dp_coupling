//
// Created by Yanjun Zhang on 2020-04-30.
//
#include <Eigen/Dense>
#include <vector>
#include <set>
#include <zmq.h>

using namespace Eigen;
using namespace std;

#ifndef LASTFM_DPCOUPLING_DPCOUPLING_H
#define LASTFM_DPCOUPLING_DPCOUPLING_H

class dpCoupling {

public:
    vector<vector<double> > R;
    vector<vector<double> > Rt;

    int m;// #users
    int n;// #items

    Eigen::MatrixXd U; //coefficients U in m*d
    Eigen::MatrixXd V; //coefficients V in n*d
    Eigen::MatrixXd S; //coefficients S in m*m
    Eigen::MatrixXd W; //coefficients W in n*n

    /**
     * Create a new instance from the given data set.
     */
    dpCoupling(vector<vector<double> > R, vector<vector<double> > Rt, int m, int n);

    /**
     * Train the model with the supplied parameters.
     *
     * @param alpha         The learning rate, e.g. 0.01.
     * @param zata          Regularization parameter.
     * @param d             Dimension of the latent space.
     * @param epoch         # training epoch.
     * @param sigma         # dp-Gaussian variance.
     */
    void train(double alpha, double zeta, int d, int epoch, double sigma);
    //report rmse on training and test dataset in each epoch


private:

    /**
     * add noice drawn from gaussian distribution.
     */
    static MatrixXd add_noise(MatrixXd g, double sigma);

    static MatrixXd msg(Eigen::MatrixXd g);
    /**
     * compute Root Mean Square Error (RMSE).
     */
    static double rmse(vector<vector<double> > R_eval, MatrixXd U, MatrixXd V, MatrixXd S, MatrixXd W);

    static double mae(vector<vector<double> > R_eval, MatrixXd U, MatrixXd V, MatrixXd S, MatrixXd W);
};



#endif //LASTFM_DPCOUPLING_DPCOUPLING_H
