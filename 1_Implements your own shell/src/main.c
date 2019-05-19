#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "utils.h"

/**
 * Aliased command element
 *
 * Example: 
 *
 * $ alias cd_to_etc="cd /etc/"
 *
 * alias: alias of command. (e.g. "cd_to_etc")
 * command: aliased command. (e.g. "cd /etc/")
 */
struct aliased_command
{
  char alias[512];
  char command[8096];
};

static struct aliased_command* aliased_commands[128] = { NULL, };
static int n_aliased_command = 0;

/**
  do_alias(argc, argv)

  params:
    argc: # of arguments. argc must be 1.
    argv: a list of arguments.

  returns:
    If success, return 0.
    Else if arguments are not valid, return -1.
*/
int do_alias(int argc, char** argv);

/**
  validate_alias_argv(argc, argv)

  returns:
   If success, return 1. (true)
   Else return 0. (false)
 */
int validate_alias_argv(int argc, char** argv);

static void release_argv(int argc, char*** argv);

int main()
{
  char buf[8096];
  int argc;
  char** argv;

  while (1) {
    fgets(buf, 8096, stdin);

apply_alias:
    mysh_parse_command(buf, &argc, &argv);


    if (strcmp(argv[0], "") == 0) {
      goto release_and_continue;
    } else if (strcmp(argv[0], "cd") == 0) {
      if (do_cd(argc, argv)) {
        fprintf(stderr, "cd: Invalid arguments\n");
      }
    } else if (strcmp(argv[0], "pwd") == 0) {
      if (do_pwd(argc, argv)) {
        fprintf(stderr, "pwd: Invalid arguments\n");
      }
    } else if (strcmp(argv[0], "alias") == 0) {
      if (do_alias(argc, argv)) {
        fprintf(stderr, "alias: Invalid arguments\n");
      }
    } else if (strcmp(argv[0], "exit") == 0) {
      goto release_and_exit;
    } else {
      for (int i = 0; i < n_aliased_command; ++i) {
        if (strcmp(argv[0], aliased_commands[i]->alias) == 0) {

          strcpy(buf, aliased_commands[i]->command);

          goto apply_alias;
        }
      }

      fprintf(stderr, "%s: command not found\n", argv[0]);
    }
release_and_continue:
    release_argv(argc, &argv);
    continue;
release_and_exit:
    release_argv(argc, &argv);
    break;
  }

  for (int i = 0; i < n_aliased_command; ++i) {
    if (aliased_commands[i]) {
      free(aliased_commands[i]);
      aliased_commands[i] = NULL;
    }
  }

  return 0;
}

static void release_argv(int argc, char*** argv) {
  for (int i = 0; i < argc; ++i) {
    free((*argv)[i]);
  }
  free(*argv);
  *argv = NULL;
}

int do_alias(int argc, char** argv) {
  if (!validate_alias_argv(argc, argv))
    return -1;

  //seprate by =
  char* temp = strtok(argv[1], "=");

  for(int i=0; i<n_aliased_command; i++){
    if(strcmp(aliased_commands[i]->alias,temp)==0){
      printf("The alias already has the command.\n");
      temp = NULL;
      return 0;
    };
  };

  //allocation
  aliased_commands[n_aliased_command] = malloc(sizeof(struct aliased_command));

  strcpy(aliased_commands[n_aliased_command]->alias, temp);
  strcpy(temp,strtok(NULL, "\"\n\\"));
  strcpy(aliased_commands[n_aliased_command]->command, temp);

  (n_aliased_command)++;
  
  //checking
  
  /*for(int i=0; i<n_aliased_command;i++){  
    printf("alias = %s, command = %s\n", aliased_commands[i]->alias,aliased_commands[i]->command);
  }
    printf("command num = %d\n", n_aliased_command);
    */
  
  return 0;
}

int validate_alias_argv(int argc, char** argv) {
  
  if(argc == 1){
    return 0;
  }
}


