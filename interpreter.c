#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "include.h"
#include  "functions.h"

instruction_node* pointer;
void push_stack(int);
int pop(void);
void printll(instruction_node*);
void interpreter_loop(instruction_node*, int*);
extern int len_const_array;
extern int *const_array;
int index_pos =0;
void start_interpreter(instruction_node* instruction_set, int* constants){
  
  printll(instruction_set);
  interpreter_loop(instruction_set, constants);
}



void interpreter_loop( instruction_node* instructions, int *constants){

  instruction_node *current;
  char str[5];
  int load_name[256];
  int load_const[256]; // used for instruction just above  opcode 6b(if) in instruction set 

  for(current = instructions; current != NULL; current = current->next){
    sprintf(str, "%d",current->opcode);
    if(strcmp(str,"17")==0){ // binary add
      int x,y;
      x =pop();
      y= pop();
      push_stack(x+y);

    }else if(strcmp(str,"47")==0){
      int x;
      x = pop();
      printf("val %d",x);
    }else if(strcmp(str,"48")==0){

      printf("\n");
    }else if(strcmp(str,"18") == 0){ //sub
      int x;
      int y;
      x = pop();
      y = pop();
      push_stack(y-x);
      
    }else if(strcmp(str,"15") == 0){ //div

      int x;
      int y;
      x = pop();
      y = pop();
      push_stack(y/x);
    }else if(strcmp(str, "14") == 0){ // mul

      int x;
      int y;
      x = pop();
      y = pop();
      push_stack(y*x);
      
    }else if(strcmp(str,"65") == 0) { 
      static int i =0;
      int position;
      char load_const_string[5];
      char if_string[5];
      
      sprintf(load_const_string,"%d",current->next->opcode);
      if(strcmp(load_const_string,"64") == 0){
	sprintf(if_string,"%d",current->next->next->opcode);
	if(strcmp(if_string,"107") == 0){
        	load_name[i] = current->pos;
		load_const[i] = current->next->pos;
        	//printf("position: %d %d", load_name[i], load_const[i]);
        	i++;
	
        }
      }
      
    }else if(strcmp(str, "107") == 0){ // if else

      int operation,variable_val=0, constant_val=0;
     
      operation = current->pos;
      if(operation == 5){ // >=
          
	//printf("array %d\t",const_array[2]);
	variable_val = const_array[load_name[index_pos]];
	constant_val = const_array[load_const[index_pos]];
	//printf("vals : %d %d",variable_val, constant_val);
	if(variable_val >= constant_val)
	  //interpreter_loop(current->next, constants);
	  
     
      }
     
      index_pos++;
     
    }else if(strcmp(str,"6E") ==0 || strcmp(str,"28") == 0){

      break;
    }
      
  }
} 
