#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void mysh_parse_command(const char* command,
                        int *argc, char*** argv)
{
        
//memory allocation
	*argc = 0;
	(*argv) = (char**)malloc(sizeof(char*)*40);

//separate command  using strtok function
//delimiter is ' \n\t'

	char* temp_c = strtok(command, " \n\t");
	//exception when you write nothing
	if(temp_c == NULL){
		*argc = 1;
		(*argv)[0] = "";
		return;
	}

	//in the case of alias command
	if(strcmp(temp_c, "alias") == 0){
			(*argv)[*argc] = (char*)malloc(sizeof(char)*100);
			strcpy((*argv)[*argc], temp_c);
			(*argc)++;

			temp_c = strtok(NULL, "\n");

			if (temp_c == NULL){
				return;
			}
			else{
			(*argv)[*argc] = (char*)malloc(sizeof(char)*100);
			strcpy((*argv)[*argc], temp_c);
			(*argc)++;
		return;

			}
		}
	

	//set argc and argv
	while(temp_c != NULL){
		(*argv)[*argc] = (char*)malloc(sizeof(char)*100);
		strcpy((*argv)[*argc], temp_c);
		temp_c = strtok(NULL, " \n\t");
		(*argc)++;
		}
	
	//checking
	
	/*printf("%d index\n",*argc);
	for(int i=0; i<(*argc) ; i++){
		printf("argv[%d] = %s\n", i, (*argv)[i]);
		}
	*/

	return;

}
