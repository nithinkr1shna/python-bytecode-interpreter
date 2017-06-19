#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "include.h"
#include  "functions.h"

void push_stack(int);
int pop(void);
void printll(instruction_node*);
void interpreter_loop(instruction_node*, int*);
extern int len_const_array;
void start_interpreter(instruction_node* instruction_set, int* constants){
  
  printll(instruction_set);
  interpreter_loop(instruction_set, constants);
}



void interpreter_loop( instruction_node* instructions, int *constants){

  instruction_node *current;
  char str[5];
  
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
      
    }else if(strcmp(str,"15") == 0){

      int x;
      int y;
      x = pop();
      y = pop();
      push_stack(y/x);
    }else if(strcmp(str, "14") == 0){

      int x;
      int y;
      x = pop();
      y = pop();
      push_stack(y*x);
    }
    
  }
} 
