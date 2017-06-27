#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "include.h"
#include "htod.h"

extern int len_const_array;
extern int *const_array;
int stack_size;

int stack[200];
//functions for stack manipulations


// push elements to end of array max size of constants array = SIZE.. // use dynamic allocation to make it dynamic

int push(int a){
   int  *ptr;
  static int i=0;
  
  if(i==0){
    const_array =(int*)malloc(sizeof(int) * 1);
    *const_array = a;
  }
  else{

    ptr = (int*)realloc(const_array,sizeof(int) *i);
    *(ptr + i) =a;
  }
 
  i= i+1;
  return i;
    
}


//reverses the string and returns the reversed string
char* strrev(char *str){
  
    int start, end, len;
    char temp, *ptr = NULL;
     
    len = strlen(str);  
    ptr = malloc(sizeof(char)*(len+1)); 
    ptr = strcpy(ptr,str);           
    for (start=0,end=len-1; start<=end; start++,end--)
    {
        temp = ptr[start];
        ptr[start] = ptr[end];       
        ptr[end] = temp;
    }
     
    return ptr; 
}

//pops up the element on the top of stack /last element in array
int pop(){

  int top;
  int index = stack_size -1;
  top = stack[index];
  stack_size = stack_size -1;
  return top;
  
  
}


void push_stack(int a){

  static int len =0;
  //len = len_const_array;
  stack[len] = a;
  stack_size = len+1;
  len = len+1;
  
}

// functions for creating instruction set

unsigned char *next_byte(unsigned char *hex_str_p, char *opcode){

     opcode[0] = *hex_str_p;
     opcode[1] = *(hex_str_p +1);
     opcode[2] = '\0';
     //printf("%s\t",opcode);
     hex_str_p++;
     hex_str_p++;
     return hex_str_p;
     
  
}

//determines length of offset

int deter_len(char *opcode){

  if(strcmp(opcode,"73") == 0 || strcmp(opcode,"69") == 0) // start
    return 4;
  else if(strcmp(opcode, "64") == 0 || strcmp(opcode, "5A") == 0 || strcmp(opcode, "65") == 0 || strcmp(opcode,"6B") == 0 || strcmp(opcode, "6E") == 0 || strcmp(opcode, "71") == 0) //load const, store_name , load_name
    return 2;
  else if(strcmp(opcode,"17") == 0 || strcmp(opcode,"47") == 0 || strcmp(opcode,"48") == 0 || strcmp(opcode, "53") == 0 || strcmp(opcode,"18") == 0 || strcmp(opcode,"15") == 0 || strcmp(opcode, "14") == 0) //binary_add, print_item, print_new_line
    return 0;
  else if(strcmp(opcode,"74") == 0)
    return 4;
  else
    return -1;
}

// create instruction set as linked list;
void push_ll(instruction_node **headRef, int opcode, int pos, int snd_pos){ // creates ll on the go

  instruction_node *current = *headRef;
  instruction_node* newNode, *temp;
  newNode =(instruction_node*)malloc(sizeof(instruction_node));
  if(newNode == NULL){

    printf("Cant Allocate\n");
    exit(0);
  }
  newNode->opcode = opcode;
  newNode->pos = pos;
  newNode->snd_pos = snd_pos;
  newNode->next = NULL;
  newNode->prev = NULL;
  //printf("%d %d %d\t",opcode,pos,snd_pos);
  
  if(current == NULL){
    
    newNode->prev = NULL;
    *headRef = newNode;
    
    
    
    
  }else{
    while(current->next != NULL){
      current = current->next;
    }
    
    //temp = current;
     current->next = newNode;
    newNode->prev = current; // added extra
   
   
    
  }
}


// prints the instruction set
void printll(instruction_node *ll){
  printf("call\t");
  int counter =0;
  instruction_node *current,*snd;
  for(current = ll;current != NULL; current= current->next){
    counter++;
    //printf("inside ll\t'");
    printf("\n%d\t",current->opcode);
    printf("%d\t",current->pos);
    printf("%d\n",current->snd_pos);
    
        
  }
  
  
}



int push_gen(int a, int* array){
   int  *ptr;
  static int k=0;
  
  if(k==0){
    array =(int*)malloc(sizeof(int) * 1);
    *array = a;
  }
  else{

    ptr = (int*)realloc(array,sizeof(int) *k);
    *(ptr + k) =a;
  }
 
  k= k+1;
  return k;
    
}
