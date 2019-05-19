# mysh-2: Implements your own shell

We're going to implement,
  * User-level threading
  * 1:n thread model

TODO:
  * mythread_join function for main func waiting for finishing tid thread
  
  <br>

## Solution
본 과제는 User-level threading을 구현한다. 라이브러리 형태로 1:n thread model을 구현하는 것이 목적이다.

Scheduling으로는 First-in First-out과 Round robin을 구현한다.

### > User-level threading 

> [3_Implements your own shell/lib/mythread.c](https://github.com/Chokyungsun/2018OS_Assignment/blob/master/3_Implements%20your%20own%20shell/lib/mythread.c)에 구현되어 있음.

```C
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
  
  ```
  <br>
  <br>

 
 <p align = "center">
<img src="https://github.com/Chokyungsun/2018OS_Assignment/blob/master/3_Implements%20your%20own%20shell/timer.PNG" width = "600">
</p>

<p align = "center"> Timer </p>
  
  ```C
  void new_stub(void (*stub)(void*), void* args){
  printf("Do stub\n");
  stub(args);
  tcbs[current_thread_id]->finished =1;
  if (swapcontext(&tcbs[current_thread_id]->ucp, &ucp_main) == -1){
    printf("ERROR: swapcontext\n");
  }
  ```
  <br>
 <p align = "center">
<img src="https://github.com/Chokyungsun/2018OS_Assignment/blob/master/3_Implements%20your%20own%20shell/thread.PNG" width = "600">
</p>
<p align = "center"> Threads </p>
 
 <br>

  
 ### > TODO
 
   **while(1)** 
 
   tick의 함수는 timer에 따라 불렀지만, while(1)을 주석 처리하면 tick 함수에서 빠져나오지 못하는 상황이 발생한다.
 
   **Tick 함수 구현 / nextTcb 함수 구현** 
 
   Tick 함수의 경우 nextTcb()의 함수를 불러 Tcb의 tid를 불러서 scheduling 방식마다 scheduling을 해주는 영역이다. 이에 더해 nextTcb()도 scheduling방식에 따라 time quantum이 지났을 때 다음으로 불러야 하는 thread가 무엇인지 알려주는 함수이다.

  **mythread_join 함수** 

  각 thread가 모두 끝날 때까지 main 함수가 기다려주는 함수



