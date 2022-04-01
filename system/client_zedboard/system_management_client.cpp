// main system management application running on the client zedboard
// John Craffey

#include <stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<stdio.h>
#include<queue>
#ifndef __CV_STRUCT_H__
#include "../system_management.h"
#else
#include "cv_structs.h"
#endif

#define QUEUE_SIZE 32

// create global structs
// camera queue
// std::queue<> cv_data_q;
// stream queue
// std::queue<> cv_data_q;
// struct cv_data queue
std::queue<struct cv_data> cv_data_q;

void* cv(void* thread_args){
    struct cv_data metadata1 = {
        .num_bbox = 1,
    };
    struct cv_data metadata2 = {
        .num_bbox = 22,
    };

    cv_data_q.push(metadata1);
    cv_data_q.push(metadata2);

    struct cv_data out;
    out = cv_data_q.front();
    cv_data_q.pop();
    printf("meta1 should be 1? %d\n", out.num_bbox);
    out = cv_data_q.front();
    cv_data_q.pop();
    printf("meta2 should be 22? %d\n", out.num_bbox);

    return NULL;
}

int main() {

    // ### init ###
    // networking stuff

    // ### kick off threads ###
    // camera

    // stream

    // CV
    pthread_t cv_main_thread;
    pthread_create(&cv_main_thread, NULL, cv, NULL);

    // ### cleanup ###
    pthread_join(cv_main_thread, NULL); 

    return 0;
}
