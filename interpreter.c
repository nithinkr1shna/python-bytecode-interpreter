#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "include.h"
#include "functions.h"

extern int len_const_array;
//extern int *const_array;
//extern int *instruction_array;
extern int lenofinstr;



int store_array[256];
int store_array_index=0;
int status =0;
int condition;

int  push_w(int, int, int*, int*);
int  pop_w(int, int, int*, int*);
void interpreter_loop(int*, int*);
void push_stack(int);
int pop(void);
int sum(int, int);
int sub(int , int);
int mul(int , int);
int divi(int, int);
int  condition_if(int, int*,int*, int);
int while_loop(int, int*, int*,int);

void start_interpreter(int *instruction_array, int *const_array){

  interpreter_loop(instruction_array, const_array);
 
}

void interpreter_loop(int *instructions, int *constants){

  int i,opc;
  for(i=0;i<lenofinstr;i++){

    printf("opc %d\t",instructions[i]);
    opc = instructions[i];
    switch(opc){
  
    case 100: //64
      i= push_w(opc,i,instructions, constants);
      printf("i %d\t",i);
      break;
    case 90: //5a
      i=pop_w(opc,i,instructions,constants);
      break;
    case 101: //65
      i= pop_w(opc,i,instructions,store_array);
      break;
    case 20: //14 mul
      pop_w(opc,i,instructions,store_array);
      break;
    case 21: // 15 div
      pop_w(opc,i,instructions,store_array);
      break;
    case 23: //17 sum
      pop_w(opc,i,instructions,store_array);
      break;
    case 24: // 18 sub
      pop_w(opc,i,instructions,store_array);
      break;
    case 107: //6b if
      printf("in if\n");
      i = condition_if(i,instructions,store_array,status);
      break;
    case 113: //71 absolute jump while loop
      // jump hence present while loop
      printf("jump addr %d\n",instructions[i+1]);
      i=while_loop(i,instructions,store_array,status);    
      break;
    case 71: //47 print
      printf("\nans is %d\n",pop());
      break;
    }
  }
 }


int  push_w(int ins, int pos, int* instructions, int* constants){

  int args =0;
  if(ins =100) // 64
    args =2;

  if(args ==2){


    push_stack(constants[instructions[pos+1]]);
    pos =pos+2;
  }

  return pos;
  
}


int pop_w(int ins, int pos, int* instructions, int *constants){


  if(ins ==90){ // store array
    int val=0,next;
    val =pop();
    next = instructions[pos+1];
    store_array[next] = val;
    pos = pos+2;
   
    
  }else if(ins ==101){ // 65 load 

    int val =0,next;
    next=instructions[pos+1];
    push_stack(store_array[next]);
    printf("loading : %d %d\n",next,store_array[next]);
    pos=pos+2;
    
  }else if(ins == 23){ // 17 sum

    int bottom =0, top=0;
    top =pop();
    bottom =pop();
    push_stack(sum(top,bottom));
    
  }else if(ins ==24){ // 18 sub;
    int bottom=0,top=0;
    top=pop();
    bottom=pop();
    push_stack(sub(bottom,top));

  }else if(ins == 20){ // 14 mul

    int bottom =0,top=0;
    top=pop();
    bottom = pop();
    push_stack(mul(top,bottom));
  }else if(ins == 21){ // 15 div

    int top=0,bottom =0;
    top=pop();
    bottom =pop();
    push_stack(divi(bottom,top));
  }
   return pos;
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


//other fns

int condition_if(int pos, int *instructions, int *constants,int status){

  int operator, top=0,bottom =0,pos1;
  operator = instructions[pos+1];
  top=pop();
  bottom =pop(); //first
  printf("tb %d %d %d %d\n",bottom, top,pos,pos-9);
  switch(operator){

  case 0: //<
    if(bottom < top){
      pos = pos+2;
    }else{

      if(instructions[pos-9] == 120){ // 78 while
        int hop = instructions[pos-8];
	printf("positions hop: %d pos :%d\n",hop,pos);
        printf("total hops %d\n",hop+pos);
        return hop+pos;
      }else{
        while(instructions[pos] != 72){
          printf("in skip\n");

          pos++;
        }
      }
   
  }
    break;
  case 4: // >
    printf("grtr than bottom: %d top : %d\n",bottom,top);
    if(bottom > top ){
      pos = pos+2;
     
    }else{

      if(instructions[pos-9] == 120){ // 78 while
        int hop = instructions[pos-8];
        printf("positions hop: %d pos :%d\n",hop,pos-9);
        printf("total hops %d\n",hop+pos-9);
        return hop+(pos-9);
      }else{
        while(instructions[pos] != 72){
          printf("in skip\n");

          pos++;
        }
      }
   
  }
  break;
  case 1: //<=
    if(bottom <= top){

      pos =pos+2;
    }else{

      if(instructions[pos-9] == 120){ // 78 while
        int hop = instructions[pos-8];
        printf("heyyaaaaaaa");
        printf("total hops %d\n",hop+pos);
        return hop+pos;
      }else{
        while(instructions[pos] != 72){
          printf("in skip\n");

          pos++;
        }
      }
   
  }
    break;
  case 2: // ==
    if(bottom == top){

      pos =pos+2;
    }else{

      if(instructions[pos-9] == 120){ // 78 while
        int hop = instructions[pos-8];
        printf("heyyaaaaaaa");
        printf("total hops %d\n",hop+pos);
        return hop+pos;
      }else{
        while(instructions[pos] != 72){
          printf("in skip\n");

          pos++;
        }
      }
   
  }
    break;
  case 3: // !=
    printf("not el to");
    if(bottom != top){

      pos = pos+2;
    }else{

      if(instructions[pos-9] == 120){ // 78 while
        int hop = instructions[pos-8];
        printf("heyyaaaaaaa");
        printf("total hops %d\n",hop+pos);
        return hop+pos;
      }else{
        while(instructions[pos] != 72){
          printf("in skip\n");

          pos++;
        }
      }
   
  }
    break;
  case 5: //>=
    if(bottom >= top){

      pos =pos+2;
    }else{

      if(instructions[pos-9] == 120){ // 78 while
        int hop = instructions[pos-8];
        printf("heyyaaaaaaa");
        printf("total hops %d\n",hop+pos);
        return hop+pos;
      }else{
        while(instructions[pos] != 72){
          printf("in skip\n");

          pos++;
        }
      }
   
  }
    break;
  default:
    printf("in defaut\n");
    
  } 
  return pos;
}


int while_loop(int pos, int* instructions, int *constants,int status){

  int next;
  next = instructions[pos+1];
  pos = next;
  
  return pos-1;
}
