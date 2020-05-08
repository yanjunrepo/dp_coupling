//
// Created by Yanjun zhang on 2020-08-03.
//

#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <zmq.h>

void matMsg_agg(int row, int column) {

    void *context = zmq_init (1);
    void *responder = zmq_socket (context, ZMQ_REP);
    zmq_bind (responder, "tcp://*:8887");

    zmq_msg_t request;
    zmq_msg_init (&request);
    zmq_recvmsg (responder, &request, 0);
    int size = zmq_msg_size(&request);
    double *buf = (double *) malloc(size + 1);
    memcpy(buf, zmq_msg_data(&request), size);
    Eigen::MatrixXd eigenX = Eigen::Map<Eigen::MatrixXd>( buf, row, column );
    zmq_msg_close (&request);


    free(buf);//

    double buffer[ row * column ];
    for (int i = 0; i < column; ++i) {
        Eigen::Map< Eigen::MatrixXd>(&buffer[i * row], row, 1) = eigenX.col(i);

    }

    zmq_msg_t reply;
    zmq_msg_init_size (&reply, row * column * 8);
    memcpy (zmq_msg_data (&reply), buffer, row * column * 8);
    zmq_sendmsg (responder, &reply, 0);
    zmq_msg_close (&reply);


    zmq_close (responder);
    zmq_term (context);

}
