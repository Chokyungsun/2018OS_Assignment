#include "alloc.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> //brk, sbrk
#include <string.h>

meta i_meta[10];
int t;

int n_line = 0;
int n_command = 0; 
char* type_fit; //F, B or W
char* addr[10];

void init_meta(char* command, size_t n_line);
void do_command(char* command , size_t n);
void output(int free, int size,char* content);

int main()
{
	FILE* file = NULL;
	char filename[256];
	char command_infile[256][256];

	printf("Input the file name: ");
	scanf("%s", filename);

	printf("The file name is %s\n", filename);
	printf("-------INPUT--------\n");

	file = fopen(filename, "r");
	if(file == NULL){
		printf("ERROR: open file is empty.\n");
		return 0;
	}
	else{
		char *pStr;

		while(1){
			pStr = fgets(command_infile[n_line], 200, file);
			if(feof(file)) break;
			if(pStr == NULL){
				printf("ERROR: reading file.\n");
				return 0;
			}
			printf("%s\n", pStr);
			n_line++;
		}
		fclose(file);
	}
	//read the file line by line

    /*for(int i=0; i<n_line; i++){
		printf("line [%d]: %s\n", i, command_infile[i]);
    }*/

	init_meta(command_infile[0], n_line);
	//make meta struct lists and check the number of commands and fit type

	for(int k=0; k<n_command; k++){
		do_command(command_infile[k+1], k);
	}

	printf("--------OUTPUT--------\n");
	for(int j=0; j<n_command; j++){
		output(i_meta[j].free, i_meta[j].size,addr[j]);
	}

	//if command is s, compute the size of string
	//invoke m_malloc(size) function line by line

	//if command is f, invoke m_free(meta[n]) for nth block

	//if command is r, invoke m_relloc(meta[n], size) to meta[n+1]
	//before that, must check if there is meta[n+1]

	//if command is e, invoke m_malloc(size) but don't put the string and there is nothing


	return 0;
}

void init_meta(char* command, size_t n_line){
	//printf("line [0]: %s\n", command);

	n_command = atoi(strtok(command , " \n\t"));
	//printf("the # of commands: %d\n",n_command);
	type_fit = strtok(NULL, " \n\t");
	//printf("fit type: %s\n", type_fit);

	if(n_command == 0){
		printf("ERROR: nothing in file.\n");
		return;
	}
	else{
		for(int i=0; i<n_command; i++){
			if(i == 0){
				i_meta[0].prev = NULL;
				i_meta[0].next = i_meta[1].prev;
				i_meta[0].free = 1; //false
				i_meta[0].size = 0;
			}
			else{
				i_meta[i].prev = i_meta[i-1].next;
				i_meta[i].next = NULL;
				i_meta[i].free = 1; //false
				i_meta[i].size = 0;
			}
		}

	}
	return;
}

void do_command(char* command, size_t k){
	//printf("line: %s\n", command);

	char* command_type;
	char* content;
	unsigned int content_size;
	int n,m;

	command_type = strtok(command , " \n\t");
	//printf("the type of commands: %s\n",command_type);


	switch(*command_type){
		case 's':
		if(i_meta[k].free == 1){
			content = strtok(NULL, "\n");
			addr[k] = (char*)m_malloc(strlen(content));
			strcpy(addr[k], content);
			t++;
		}
		break;
		case 'f':
		n = atoi(strtok(NULL, "\n"));
		m_free(addr[n]);
		if(strcmp(addr[n],"")==0){
			i_meta[n].free = 1;
			i_meta[n].next = NULL;
			//change info about the one which is free
			i_meta[k].prev = NULL;
			i_meta[k].free = NULL;
			i_meta[k].size = NULL;
			//info about f command line 
		}
		t++;
		break;
		case 'r':
		n = atoi(strtok(NULL, " \n\t"));
		m = atoi(strtok(NULL, "\n"));
		printf("the type of commands: %c\n",*command_type);
		break;
		case 'e':
		n = atoi(strtok(NULL, "\n"));
		printf("the type of commands: %c\n",*command_type);
		break;
		default:
		printf("There is no command like that.\n");
		break;

	}

	return;
}

void output(int free, int size,char* content){
	printf("%d %d %s\n",free,size,content);
	return;
}

