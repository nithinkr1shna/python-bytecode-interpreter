#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "include.h"

void push_stack(int);
int pop(void);
void printls(instruction_node*);
void interpreter_loop(instruction_node*, int*);
extern int len_const_array;
void start_interpreter(instruction_node* instruction_set, int* constants){
  
  printls(instruction_set);
  interpreter_loop(instruction_set, constants);
}

void printls(instruction_node *ll){
  printf("call @interpreter\t");
  instruction_node *current;
  for(current = ll;current != NULL; current= current->next){

    // printf("inside ll\t'");
    printf("\n%d\t",current->opcode);
    printf("%d\t",current->pos);
    printf("%d\n",current->snd_pos);
    
  }
}

void interpreter_loop( instruction_node* instructions, int *constants){

  instruction_node *current;
  char str[5];
  
  for(current = instructions; current != NULL; current = current->next){
    sprintf(str, "%d",current->opcode);
    if(strcmp(str,"17")==0){ // binary add
      int x,y;
      x =pop();
      //printf("%d \n",x);
      y= pop();
      //printf("%d\n",y);
      push_stack(x+y);

      //printf("sum is %d",x+y);
      // printf("len_const_ary %d",len_const_array);
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
