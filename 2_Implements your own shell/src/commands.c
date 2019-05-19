#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "commands.h"
#include "built_in.h"

#define SERVER_PATH "/tmp/mysh_socket.server"
#define CLIENT_PATH "/tmp/mysh_socket.client"
#define DATA "Hello from server"

static struct built_in_command built_in_commands[] = {
  { "cd", do_cd, validate_cd_argv },
  { "pwd", do_pwd, validate_pwd_argv },
  { "fg", do_fg, validate_fg_argv }
};

void execvp_mysh(struct single_command* com);
void *do_client(struct single_command* com);

static int is_built_in_command(const char* command_name)
{
  static const int n_built_in_commands = sizeof(built_in_commands) / sizeof(built_in_commands[0]);

  for (int i = 0; i < n_built_in_commands; ++i) {
    if (strcmp(command_name, built_in_commands[i].command_name) == 0) {
      return i;
    }
  }

  return -1; // Not found
}

void *do_client(struct single_command* com){
  int server_sock, client_sock, len, rc;
  int bytes_rec = 0;
  struct sockaddr_un server_sockaddr;
  struct sockaddr_un client_sockaddr;
  char buf[256];
  int backlog = 5;

  memset(&server_sockaddr, 0 , sizeof(struct sockaddr_un));
  memset(&client_sockaddr, 0 , sizeof(struct sockaddr_un));
  memset(buf, 0 ,256);

  //printf("client socket\n");
  client_sock = socket(AF_UNIX, SOCK_STREAM,0);
  if (client_sock == -1){
    perror("client error: \n");
    return 1;
  }

  unlink(CLIENT_PATH);
  client_sockaddr.sun_family = AF_UNIX;
  strcpy(client_sockaddr.sun_path, CLIENT_PATH);
  len = sizeof(client_sockaddr);
  
  //bind
  //printf("bind\n");
  rc = bind(client_sock, (struct sockaddr*)&client_sockaddr, len);
  if (rc == -1){
    perror("Bind error: \n");
    close(client_sock);
    return 1;
  }

  server_sockaddr.sun_family = AF_UNIX;
  strcpy(server_sockaddr.sun_path, SERVER_PATH);
  len = sizeof(server_sockaddr);
  rc = connect(client_sock, (struct sockaddr*)&server_sockaddr, len);
  if(rc == -1){
    perror("connect error: \n"); 
    close(client_sock);
    close(server_sock);
    return -1;
  }

  int outstream = dup(1);
  dup2(client_sock, 1);
  evaluate_command(1, com);
  close(client_sock);
  fflush(stdout);
  dup2(outstream, 1);
  close(1);

  pthread_exit(NULL);
}


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

int do_pipe(struct single_command (*commands)[512]) {
  struct single_command* com = (*commands);  
  int server_sock, client_sock, len, rc;
  int bytes_rec = 0;
  int nSockopt;
  struct sockaddr_un server_sockaddr;
  struct sockaddr_un client_sockaddr;
  char buf[256];
  int backlog = 5;

  memset(&server_sockaddr, 0 , sizeof(struct sockaddr_un));
  memset(&client_sockaddr, 0 , sizeof(struct sockaddr_un));
  memset(buf, 0 ,256);

  //printf("server socket\n");
  server_sock = socket(AF_UNIX, SOCK_STREAM,0);
  if (server_sock == -1){
    perror("Socket error: \n");
    return 1;
  }

  unlink(SERVER_PATH);
  server_sockaddr.sun_family = AF_UNIX;
  strcpy(server_sockaddr.sun_path, SERVER_PATH);
  len = sizeof(server_sockaddr);

  //bind
  //printf("bind\n");
  rc = bind(server_sock, (struct sockaddr*)&server_sockaddr, len);
  if (rc == -1){
    perror("Bind error: \n");
    close(server_sock);
    return 1;
  }

  //listen for any client sockets
  //printf("listen\n");
  rc = listen(server_sock, backlog);
  if (rc == -1){
    perror("Listen error: \n");
    close(server_sock);
    return 1;
  }

  pthread_t thread;
  pthread_create(&thread, NULL, do_client, com);
  //accept an incoming connection(create client)
  //printf("accept\n");
  client_sock = accept(server_sock, (struct sockaddr*)&client_sockaddr, &len);
  if(client_sock == -1){
    perror("Accept error: \n");
    close(server_sock);
    close(client_sock);
    return 1;
  }

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

/*
 * Description: Currently this function only handles single built_in commands. You should modify this structure to launch process and offer pipeline functionality.
 */
int evaluate_command(int n_commands, struct single_command (*commands)[512])
{
  if(n_commands == 0) {
    return 0;
  }
  else if (n_commands == 1) {
    struct single_command* com = (*commands);
    assert(com->argc != 0);

    int built_in_pos = is_built_in_command(com->argv[0]);
    if (built_in_pos != -1) {
      if (built_in_commands[built_in_pos].command_validate(com->argc, com->argv)) {
        if (built_in_commands[built_in_pos].command_do(com->argc, com->argv) != 0) {
          fprintf(stderr, "%s: Error occurs\n", com->argv[0]);
        }
      } else {
        fprintf(stderr, "%s: Invalid arguments\n", com->argv[0]);
        return -1;
      }
    } else if (strcmp(com->argv[0], "") == 0) {
      return 0;
    } else if (strcmp(com->argv[0], "exit") == 0) {
      return 1;
    } else {
      int status;
      switch(fork()) {
        case -1:
        printf("Fork Err\n");
        break;
        case 0:
        execvp_mysh(com);
        fprintf(stderr, "%s: Invalid arguments\n", com->argv[0]);
        exit(0);
        break;
        default:
        wait(&status);
        break;
      }
      return 0;
    }
  }
  else {
    do_pipe(commands);
  }

  return 0;
}

void free_commands(int n_commands, struct single_command (*commands)[512])
{
  for (int i = 0; i < n_commands; ++i) {
    struct single_command *com = (*commands) + i;
    int argc = com->argc;
    char** argv = com->argv;

    for (int j = 0; j < argc; ++j) {
      //printf("%d\n",argc);
      free(argv[j]);
    }
    //printf("dd\n");
    free(argv);
  }

  memset((*commands), 0, sizeof(struct single_command) * n_commands);
}
