
Operating system Assignment 4
===================

TA Info & Assignment URL
---

- TA Mail: ckai22@ajou.ac.kr
- TA gitlab: http://git.ajou.ac.kr/ckai22

File Info
---
- **main.c**: Implements input and output
- **alloc.h**: Implements meta structure
- **alloc.c**: Implements malloc, free, and realloc
- **Makefile**: compiles the 
- **input** directory: saves the input file

TEST
---
##### Test proceeds as follows.
- - - 
1.	Input File name to the main function parameter.
2.	Read File (File IO)
3.	Interpret commands in File
4.	Output to stdout

##### **Command structure in File**
- - -
"Number of command lines" "Fit"<br>
"Command" "Contents"<br>
"Command" "Contents"<br>
...<br>

##### **Fit type (case sensitive)**
- - -
F: first fit<br>
B: Best fit<br>
W: worst fit<br>

##### **Command type**
- - -
**s** String: String The command to put the value into<br>
**f** n: nth allocation area free<br>
**r** n m: reallocation with m bytes in the nth allocation area<br>
**e** n: Allocate space without contents to n bytes<br>

##### **Output**
- - -
“Free" "size" "content (if command is m only)"<br>
"Free" "size" "content (if command is m only)"<br>
...<br>

##### **Example**
- - -
E1)<br>
**Input**<br>
3 F<br>
m Think like a man of action and act like man of thought.<br>
m Courage is very important. Like a muscle, it is strengthened by use.<br>
m Life is the art of drawing sufficient conclusions from insufficient premises.<br>
**Output**<br>
0 56 Think like a man of action and act like man of thought.<br>
0 72 Courage is very important. Like a muscle, it is strengthened by use.<br>
0 80 Life is the art of drawing sufficient conclusions from insufficient premises.<br>
<br>
E2)<br>
**Input**<br>
2 F<br>
m Think like a man of action and act like man of thought.<br>
f 0<br>
**Output**<br>
1 56

