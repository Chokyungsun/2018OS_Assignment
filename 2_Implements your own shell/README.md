# mysh-1: Implements your own shell

We're going to implement,
  * Program execution by accessing absolute path, Background processing
  * Pipe between two processes, signal handling 

TODO:
  *  Background processing
  
  <br>

## Solution
본 과제는 Program execution by accessing absolute path, Background processing, Pipe between two processes, 그리고 signal handling 을 구현하는 것이 목표이다. 

- [x]  Program execution by accessing absolute path

- [x] Pipe between two processes

- [x] Signal handling

- [ ] Backgound processing 

<br>

### >  Program execution by accessing absolute path

> [commands.c](https://github.com/Chokyungsun/2018OS_Assignment/blob/master/2_Implements%20your%20own%20shell/src/commands.c)에 구현되어 있음.

```C
//execvp 
void execvp_mysh(struct single_command* com){
  if(strstr(com->argv[0],"/") != NULL){
    execv(com->argv[0], com->argv);
  } else {
    char path[512] = "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin";
    for(char* temp = strtok(path, ":"); temp != NULL; temp = strtok(NULL, ":")) {
      char* ntemp = (char*)malloc(sizeof(char)*100);
      strcpy(ntemp, temp);
      strcat(ntemp,"/");
      strcat(ntemp, com->argv[0]);
      execv(ntemp, com->argv);
    }
  }
}
```


### >   Pipe between two processes

> [commands.c](https://github.com/Chokyungsun/2018OS_Assignment/blob/master/2_Implements%20your%20own%20shell/src/commands.c)에 구현되어 있음.

```C
 int status;
  pthread_join(thread, (void**)&status);

  pid_t pid;
  int fstatus;
  int instream = dup(0);

  switch(fork()) {
    case -1:
    printf( "Fork failed\n");
    break;

    case 0:
    dup2(client_sock, 0);
    execvp_mysh(com+1);
    fprintf(stderr, "%s: command not found\n", (com+1)->argv[0]);
    close(client_sock);
    dup2(instream, 0);
    close(0);
    exit(0);
    break;

    default:
    close(client_sock);
    wait(&fstatus);
    break;
  }

  close(client_sock);
  close(server_sock);
}
```


### >  Signal handling

> [signal_handlers.c](https://github.com/Chokyungsun/2018OS_Assignment/blob/master/2_Implements%20your%20own%20shell/src/signal_handlers.c)에 구현되어 있음.

```C
void catch_sigint(int signalNo)
{
   signal(SIGINT, SIG_IGN);
   return;
}

void catch_sigtstp(int signalNo)
{
   signal(SIGTSTP, SIG_IGN);
   return;
}
```


### >   Backgound processing 

 - Not implement yet.

