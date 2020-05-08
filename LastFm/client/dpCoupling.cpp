//
// Created by Yanjun Zhang on 2020-04-30.
//

#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <set>
#include <cmath>
#include "dpCoupling.h"
#include <fstream>
#include<string>
#include <ctime>
#include <algorithm>
#include <random>
#include <zmq.h>



using namespace std;
using namespace Eigen;

dpCoupling::dpCoupling(vector<vector<double> > R, vector<vector<double> > Rt, int m, int n) {
    this->R = R;
    this->Rt = Rt;
    this->m = m;
    this->n = n;

}

void dpCoupling::train(double alpha, double zeta, int d, int epoch, double sigma) {
    //initialization
    double uweight = sqrt((double)6/(10000+d)) ;
    double vweight = sqrt((double)6/(10000+d)) ;
    double sweight = sqrt((double)6/(10000+10000)) ;
    double wweight = sqrt((double)6/(10000+10000)) ;

    srand((unsigned int) time(0));
    MatrixXd U = uweight * MatrixXd::Random(m, d);
    MatrixXd V = vweight * MatrixXd::Random(n, d);
    MatrixXd S = sweight * MatrixXd::Random(m, m);
    MatrixXd W = wweight * MatrixXd::Random(n, n);



    for (int ep = 0; ep < epoch; ++ep) {

        //generate a random sequence [0, R.size()-1]
        int a[R.size()-1];
        for (int k = 0; k < R.size()-1; ++k) {a[k]=k;}
        srand(time(0)); //Use current time as seed for random generator
        for (int k = R.size()-1; k >=1 ; --k) {swap(a[k-1], a[rand()%k]);}

        for (int r = 0; r < R.size()-1; ++r) {

            int i = R[a[r]].at(0)-1; // i: user index, starts at 0
            int j = R[a[r]].at(1)-1; // j: item index, starts at 0

            double iauser = ((S.row(i).transpose()).cwiseProduct(U*(V.row(j).transpose()))).sum();
            double iaitem = (W.row(j).cwiseProduct(U.row(i)*(V.transpose()))).sum();
            double R_hat = U.row(i) * (V.row(j).transpose()) + iauser + iaitem;

            //compute gradients
            MatrixXd g_u = (R_hat - R[a[r]].at(2)) * (V.row(j)+S(i,i)*V.row(j)+W.row(j)*V) + zeta * U.row(i);
            MatrixXd g_v = (R_hat - R[a[r]].at(2)) * (U.row(i)+S.row(i)*U+W(j,j)*U.row(i)) + zeta * V.row(j);
            MatrixXd g_s = (R_hat - R[a[r]].at(2)) * (V.row(j)*U.transpose()) + zeta * S.row(i);
            MatrixXd g_w = (R_hat - R[a[r]].at(2)) * (U.row(i)*V.transpose()) + zeta * W.row(j);

            //normalize gradients to bound l2-sensitivity
            for (int k = 0; k < d; ++k) {
                g_u(0,k) = g_u(0,k) / max(1.0, g_u(0,k)*g_u(0,k));
                g_v(0,k) = g_v(0,k) / max(1.0, g_v(0,k)*g_v(0,k));
            }

            for (int i1 = 0; i1 < m; ++i1) {
                g_s(0,i1) = g_s(0,i1) / max(1.0, g_s(0,i1)*g_s(0,i1));
            }

            for (int k1 = 0; k1 < n; ++k1) {
                g_w(0,k1) = g_w(0,k1) / max(1.0, g_w(0,k1)*g_w(0,k1));

            }


            //add noise
            g_u = add_noise(g_u, sigma);
            g_v = add_noise(g_v, sigma);
            g_s = add_noise(g_s, sigma);
            g_w = add_noise(g_w, sigma);


            //push gradients to the Agg node, then pull gradients from the Agg node
            g_u = msg(g_u);
            g_v = msg(g_v);
            g_s = msg(g_s);
            g_w = msg(g_w);

            //descent
            U.row(i) = U.row(i) - alpha * g_u;
            V.row(j) = V.row(j) - alpha * g_v;
            S.row(i) = S.row(i) - alpha * g_s;
            W.row(j) = W.row(j) - alpha * g_w;

        }

        //compute RMSE on train
        double rms = rmse(R, U, V, S, W);
        //compute RMSE on test
        double rms_t = rmse(Rt, U, V, S, W);
        cout<< "RMSE at "<<ep<<"th epoch on train is: "<<rms<<"; on test is: "<<rms_t<<endl;

        //compute MAE on train
        double ma = mae(R, U, V, S, W);
        //compute MAE on test
        double ma_t = mae(Rt, U, V, S, W);
        cout<< "MAE at "<<ep<<"th epoch on train is: "<<ma<<"; on test is: "<<ma_t<<endl;



    }
    this->U = U;
    this->V = V;
    this->S = S;
    this->W = W;
}

double dpCoupling::rmse(vector<vector<double> > R_eval, MatrixXd U, MatrixXd V, MatrixXd S, MatrixXd W) {
    double result = 0;
    for (int r = 0; r < R_eval.size()-1; ++r) {
        int i = R_eval[r].at(0)-1;
        int j = R_eval[r].at(1)-1;
        double iauser = ((S.row(i).transpose()).cwiseProduct(U*(V.row(j).transpose()))).sum();
        double iaitem = (W.row(j).cwiseProduct(U.row(i)*(V.transpose()))).sum();
        double R_hat = U.row(i) * (V.row(j).transpose()) + iauser + iaitem;
        result += (R_hat - R_eval[r].at(2))*(R_hat - R_eval[r].at(2));
    }
    return sqrt(result/(R_eval.size()-1));

}

double dpCoupling::mae(vector<vector<double> > R_eval, MatrixXd U, MatrixXd V, MatrixXd S, MatrixXd W) {
    double result = 0;
    for (int r = 0; r < R_eval.size()-1; ++r) {
        int i = R_eval[r].at(0)-1;
        int j = R_eval[r].at(1)-1;
        double iauser = ((S.row(i).transpose()).cwiseProduct(U*(V.row(j).transpose()))).sum();
        double iaitem = (W.row(j).cwiseProduct(U.row(i)*(V.transpose()))).sum();
        double R_hat = U.row(i) * (V.row(j).transpose()) + iauser + iaitem;
        result += abs(R_hat - R_eval[r].at(2));
    }
    return result/(R_eval.size()-1);

}

MatrixXd dpCoupling::add_noise(MatrixXd g, double sigma){
    default_random_engine generator;
    normal_distribution<double> dist(0, sigma);

    for (int i = 0; i < g.cols(); ++i) {
        g(0,i) += dist(generator);
    }

    return g;

}

MatrixXd dpCoupling::msg(Eigen::MatrixXd g){

    void *context = zmq_init (1);

    void *requester = zmq_socket (context, ZMQ_REQ);
    /*** local*/
    zmq_connect (requester, "tcp://127.0.0.1:8887");
    /*** wan*/
    //zmq_connect (requester, "tcp://13.210.93.203:8887");
    /*** lan*/
    //zmq_connect (requester, "tcp://172.31.15.210:8887");

    int len = g.cols()*g.rows();
    double array[len];

    for (int i = 0; i < g.cols(); ++i) {
        Eigen::Map< Eigen::MatrixXd>(&array[i * g.rows()], g.rows(), 1) = g.col(i);
    }

    zmq_msg_t request;
    zmq_msg_init_size (&request, len * 8);
    memcpy (zmq_msg_data (&request), array, len * 8);
    zmq_sendmsg (requester, &request, 0);
    zmq_msg_close (&request);



    zmq_msg_t reply;
    zmq_msg_init (&reply);
    zmq_recvmsg (requester, &reply, 0);

    int size = zmq_msg_size (&reply);
    double *buf = (double *) malloc(size + 1);
    memcpy (buf, zmq_msg_data (&reply), size);

    Eigen::MatrixXd agg = Eigen::Map<Eigen::MatrixXd>( buf, g.rows(), g.cols() );
    free(buf);
    zmq_close (requester);
    zmq_term (context);

    return agg;

}