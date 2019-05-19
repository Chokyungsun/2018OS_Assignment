#include <string.h>
#include "commands.h"
#include <unistd.h>
#include <stdio.h>

int result;

int do_cd(int argc, char** argv) {

//change dir -> chdir()

  result = chdir(argv[1]);


  if (!validate_cd_argv(argc, argv)){
     return -1;
   }

//mid-test
//printf("%s\n", argv[1]);

  return 0;
}

int do_pwd(int argc, char** argv) {

  if (!validate_pwd_argv(argc, argv))
    return -1;

//print working dir
//check working dir -> getcwd()

  char wd[1000];
  getcwd(wd, 1000);
  printf("%s\n", wd);

  return 0;
}


int validate_cd_argv(int argc, char** argv) {

  if(argc>2){
    return 0;
  }

  if (result == -1){
    return 0;
  }
  else{
    return 1;
  }
}

int validate_pwd_argv(int argc, char** argv) {

  if(argc>1){
    return 0;
  }
  if(strcmp((argv)[0],"pwd") != 0){
    return 0;
  }
  
  return 1;
}

