// main system management application running on the client zedboard
// John Craffey

#include "cv_main.h"
#include <pthread.h>
#include <queue>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef __CV_STRUCT_H__
#include "../common_headers/system_management.h"
#else
#endif

#define QUEUE_SIZE 32

// create global structs
// camera queue
// std::queue<> cv_data_q;
// stream queue
// std::queue<> cv_data_q;
// struct cv_data queue
std::queue<struct cv_data> cv_data_q;

// camera thread main function
void *camera(void *thread_args) { return NULL; }
// stream thread main function
void *stream(void *thread_args) { return NULL; }
// computer vision main function
void *cv(void *thread_args) {
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

#ifdef __CV_STRUCT_H__
  // Code for when sysman has access to CV code
  // TODO: Delete the #ifdef/#endif after CV is integrated to allow this code to
  // run
  bool do_run_cv = true;
  while (do_run_cv) {
    // TODO: Add "gstream camera_stream" as argument to GetBBoxesFromFrame (or
    // whatever the gstream type is)
    cv_data_q.push(GetBBoxesFromFrame());
  }
#endif

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
