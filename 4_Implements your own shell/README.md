# mysh-3: Implements your own shell

We're going to implement,
  * Process Memory allocation

TODO:
  * none
  
  <br>

## Solution
본 과제는 Process Memory allocation을 구현하는 과제이다. 

<p><sub> >_기본적으로 memory를 할당하기위해서는 heap 공간을 원하는 만큼 늘려주고 줄여준다. 여기서 heap은 mapped region, unmapped region 그리고 unusable region의 공간이 있는데, mapped region안에 메모리 block들을 할당시켜주고, unmapped region과 mapped region의 경계를 말해주는 brk를 움직여주면서 mapped region 공간을 원하는 만큼 변경시켜주는 것이 이 과제의 주 목적이다. </sub></p>

<br>
### > Preprocess about inputs

> [main.c](https://github.com/Chokyungsun/2018OS_Assignment/blob/master/4_Implements%20your%20own%20shell/main.c)에 구현되어 있음.

```C
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
  
  ```
  <br>
  <br>

## > Main structure
  
  ```C
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
  ```
  <br>
 <br>

  
 ## TODO
 
   **M_free 함수 구현** 
 
   free시켜주고자하는 메모리의 주소값을 받아 그 주소값에 있는 스트링을 없애주고, 과제의 요구사항에 따라 memory를 다시 돌려주는 것이 아니라 전후 블락들의 상태를 보고 메모리를 합칠 것인지 없앨 것인지 구현하는 것이 최종 목표이다.



