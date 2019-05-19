# mysh-0: Implements your own shell

We're going to implement,
  * shell command parser
  * three built‐in commands in shells, pwd, cd, alias

TODO:
  * Update for alias implementation
  
  <br>
  
 ## Solution
 
 본 과제에서는 Linux shell을 이용하여, shell command parser 함수를 구현하고 pwd, cd, alias
command들을 실행시키는 함수를 구현하는 것이다. <br>

### > What is 'shell'
운영체제의 중심부인 kernel과 user사이의 인터페이스
- shell에서는 간단한 명령어로 파일을 열거나 application을실행시킬 수 있다. 


### > To parse commands.

> [1_Implements your own shell/src/utils.c](https://github.com/Chokyungsun/2018OS_Assignment/blob/master/1_Implements%20your%20own%20shell/src/utils.c) 에 구현되어 있음
```C    
//memory allocation
*argc = 0;
(*argv) = (char**)malloc(sizeof(char*)*40);
 ```

```C
//set argc and argv
while(temp_c != NULL){
  (*argv)[*argc] = (char*)malloc(sizeof(char)*100);
  strcpy((*argv)[*argc], temp_c);
  temp_c = strtok(NULL, " \n\t");
  (*argc)++;
 }
  ```
  

### > To implement commands.
  
  > [1_Implements your own shell/src/main.c](https://github.com/Chokyungsun/2018OS_Assignment/blob/master/1_Implements%20your%20own%20shell/src/main.c) 에 구현되어 있음
  
```C
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
    }
    
 ```
<br>

> [1_Implements your own shell/src/commands.c](https://github.com/Chokyungsun/2018OS_Assignment/blob/master/1_Implements%20your%20own%20shell/src/commands.c) 에 구현되어 있음.


```C
int do_cd(int argc, char** argv) {

//change dir -> chdir()

  result = chdir(argv[1]);


  if (!validate_cd_argv(argc, argv)){
     return -1;
   }
   ```

