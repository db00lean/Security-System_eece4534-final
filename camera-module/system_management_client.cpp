// main system management application running on the client zedboard
// John Craffey

#include "cv_main.h"
#include "stream.h"
#include "../network/client.h"
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
pthread_mutex_t mutex;

struct stream_args
{
  int argc;
  char **argv;
};

// stream thread main function
void *stream(void *thread_args)
{
  struct stream_args *args = (struct stream_args *)thread_args;
  stream_server(args->argc, args->argv);
  return 0;
}

// computer vision main function
void *cv_t(void *thread_args)
{
  bool do_run_cv = true;
  cv_data current_data;

  while (do_run_cv)
  {
    // TODO: Add "gstream camera_stream" as argument to GetBBoxesFromFrame (or
    // whatever the gstream type is)
    current_data = GetBBoxesFromFrame();
    pthread_mutex_lock(&mutex); // Lock
    cv_data_q.push(current_data);
    pthread_mutex_unlock(&mutex); // Unlock
  }

  return NULL;
}

int main(int argc, char *argv[])
{

  // ### init ###
  // networking stuff
  // TODO: find cam_id
  const char *port = "55000"; // Statically defined for now
  const char *address = "127.0.0.1";
  int cam_id;
  struct client *c = new_client(port, address);

  // ### kick off threads ###

  // stream
  pthread_t stream_thread;
  struct stream_args arg = {argc, argv};
  pthread_create(&stream_thread, NULL, stream, &arg);

  // CV
  pthread_t cv_main_thread;
  pthread_mutex_init(&mutex, NULL);
  pthread_create(&cv_main_thread, NULL, cv_t, NULL);
  struct cv_data out;

  pthread_mutex_lock(&mutex); // Lock
  out = cv_data_q.front();
  cv_data_q.pop();
  pthread_mutex_unlock(&mutex); // Unlock
  send_msg(c->requester, cam_id, CV_DATA, (void *)&out, sizeof(struct cv_data) + sizeof(struct coordinate_data));

  // ### cleanup ###
  pthread_join(cv_main_thread, NULL);
  pthread_mutex_destroy(&mutex);
  free(c);

  return 0;
}
