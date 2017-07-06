#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "include.h"
#include "htod.h"

extern int len_const_array;
extern int *const_array;
int stack_size;

int stack[10000];
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
  int i;
  //len = len_const_array;
  stack[len] = a;
  stack_size = len+1;
  len = len+1;

    printf("\nstack: ");
  for(i=0;i<len;i++)
    printf("%d\t",stack[i]);
      printf("\n");
  
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




// basic arithmatic functions

int sum(int a, int b){
  printf("the sum is %d\n",a+b);
  return a+b;
  
}

int sub(int a, int b){

  return a-b;
}

int mul(int a, int b){

  return a*b;
}

int divi(int a, int b){

  return a/b;
}

int modulo(int a, int b){

  return a%b;
}



