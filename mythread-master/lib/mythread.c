#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>


#include "mythread.h"

static enum mythread_scheduling_policy sched_policy = MYTHREAD_RR;
static int waiting = 0;
static int running = 1;
static int ready = 2;
static int terminated = 3;

static ucontext_t ucp_main;


// Thread control block
struct tcb {
  unsigned int status; // waiting = 0, running = 1, ready = 2 or terminated
  ucontext_t ucp; //context
  char stack[16384];
  int thread_id; // Tread ID
  int finished; // 0이면 끝나지 않음, 1이면 끝남.
};

static struct tcb* tcbs[1024];
static int n_tcbs = 0;
static int current_thread_id = 0; // thread id before context switching eg)tcbs[current_thread_id]
static struct itimerval time_quantum; // 4msec
static struct sigaction ticker; 

static int nextTcb() {
  if (sched_policy == MYTHREAD_RR) {
    // TODO: Fill your own code
  } else if (sched_policy == MYTHREAD_FIFO) {
      if(tcbs[current_thread_id]->finished == 0){
        return current_thread_id;
      }
      else return current_thread_id++;
    }
    // TODO: Fill your own code
    // check finished
    // if finished is 0 return current_thread_id
    // WARNING: main is not working by FIFO -> could be deadlock
    // ->> the solution is join function 
  
}// inform you what is the next thread scheduling

static void tick(int sig) {
  
  printf("ticktick\n");

  /*if (sched_policy == MYTHREAD_RR) {

  } else if (sched_policy == MYTHREAD_FIFO) {
 if(tcbs[current_thread_id]->finished == 1){
  printf("here\n");
    if (swapcontext(&tcbs[current_thread_id]->ucp, &tcbs[nextTcb()]->ucp) == -1){
      printf("ERROR: swapcontext\n");
      }

    }

 }*/


  // TODO: Implement your own code

  //text switching
  //could be the most difficult part
  // HINT: 이 함수에서 nextTcb() 함수를 통해 scheduling 하고, 해당 schedule 함수를 실행.
}

void mythread_init(enum mythread_scheduling_policy policy)
{
  sched_policy = policy;

  ticker.sa_handler = &tick; //when signal being called tick is executed
  
  sigemptyset(&ticker.sa_mask);
  ticker.sa_flags = 0;
  ticker.sa_handler = &tick; 

  sigaction(SIGALRM, &ticker, NULL);

  time_quantum.it_interval.tv_sec = 0;
  time_quantum.it_value.tv_sec = 0;
  time_quantum.it_interval.tv_usec = 40000;
  time_quantum.it_value.tv_usec = 40000; // expire time


  if(setitimer(ITIMER_REAL, &time_quantum, NULL)==-1){
    printf("ERROR: setitimer\n");
  }

  //while(1);
  //for tick

  // TODO: Implement your own code
  
  // queue init
  // HINT: 이 함수에서 tick 에 대해서 timer signal를...
}
void new_stub(void (*stub)(void*), void* args){
  printf("Do stub\n");
  stub(args);
  tcbs[current_thread_id]->finished =1;
  if (swapcontext(&tcbs[current_thread_id]->ucp, &ucp_main) == -1){
    printf("ERROR: swapcontext\n");
  }

}

int mythread_create(void (*stub)(void*), void* args)
{
  int tid = -1; // Thread ID
  
  printf("Thread create\n");
  tcbs[n_tcbs] = (struct tcb*)malloc(sizeof(struct tcb*));
  current_thread_id = n_tcbs;

  printf("current id: %d\n",current_thread_id);

  if(getcontext(&tcbs[current_thread_id]->ucp)==-1){
    printf("ERROR: getcontext\n");
  }
  tcbs[n_tcbs]->status = ready;

  tcbs[n_tcbs]->thread_id = rand() % 100;
  tid = tcbs[n_tcbs]->thread_id;
  printf("Tid: %d\n",tid);

  tcbs[n_tcbs]->finished = 0;

  tcbs[current_thread_id]->ucp.uc_stack.ss_sp = tcbs[n_tcbs]->stack;
  tcbs[current_thread_id]->ucp.uc_stack.ss_size = sizeof(tcbs[n_tcbs]->stack);
  tcbs[current_thread_id]->ucp.uc_link = &ucp_main;
  makecontext(&tcbs[current_thread_id]->ucp, new_stub, 2, stub, args);

  printf("swap main and tcbs[%d]\n",current_thread_id);
  if (swapcontext(&ucp_main, &tcbs[current_thread_id]->ucp) == -1){
    printf("ERROR: swapcontext\n");
  }


  n_tcbs++;


  // stub -> the body of thread
  // put info in the tcb
  // TODO: Implement your own code

  return tid;
}



void mythread_join(int tid)
{
  // main func is waiting for finishing tid thread
  // TODO: Implement your own code
}

