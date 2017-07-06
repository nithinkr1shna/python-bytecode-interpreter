#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "include.h"
#include "functions.h"

extern int len_const_array;
extern int *const_array;
extern int *dup_const_array;
//extern int *instruction_array;
extern int lenofinstr;
extern int no_of_funs;


int store_array[256];
int store_array_index=0;
int status =0;

int condition;
int make_counter =0;
int fn_instructions[SIZE];
int onethirtyone =0;
int function_position =0;

int  push_w(int, int, int*, int*,int);
int  pop_w(int, int, int*, int*);
void interpreter_loop(int*, int*,int,int);
void push_stack(int);
int pop(void);
int sum(int, int);
int sub(int , int);
int mul(int , int);
int divi(int, int);
int modulo(int, int);
int  condition_if(int, int*,int*, int);
int while_loop(int, int*, int*,int);
int run_func(int,int,int*,int*,int);

void start_interpreter(int *instruction_array, int *const_array){

  int fn_status =0;
  interpreter_loop(instruction_array, const_array, lenofinstr, fn_status);
  
}

void interpreter_loop(int *instructions, int *constants, int length, int fn_status){

  int i,opc;
  for(i=0;i<length;i++){

    printf("opc %d\t",instructions[i]);
    opc = instructions[i];
    switch(opc){
  
    case 100: //64
      i= push_w(opc,i,instructions, constants,fn_status);
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
    case 22: // 16 modulo %
      pop_w(opc,i,instructions,store_array);
      break;
    case 23: //17 sum
      printf("running sum");
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
      i=while_loop(i,instructions,store_array,fn_status);    
      break;
    case 71: //47 print
      printf("\nans is %d\n",pop());
      break;
    case 131: // call fn 83
      i=push_w(opc, i,instructions,store_array,fn_status);
      printf("return position after fn call :%d\t",i);
      fn_status =0;
      break;
    case 124: // load ref 7c
      i = push_w(opc,i,instructions,store_array,fn_status);
      break;
    case 125: // 7d store
      printf("in 7d store");
      i = push_w(opc, i,instructions,store_array,fn_status);
      break;
    }
  }
 }


int  push_w(int ins, int pos, int* instructions, int* constants, int fn_status){

  if(ins == 100){ // 64 
      if(instructions[pos+3] == 132){ // 84 make function

    
           push_stack(instructions[pos+1]); //push position of function to stack
           //pos = pos+5;
	   printf("changed pos %d %d\t",pos+5 ,pos);
    
      }else{

	if(fn_status ==0){

	   printf("no of fnx:%d",no_of_funs);
           push_stack(constants[instructions[pos+1] - no_of_funs]);
	   printf("\npushing to stack at 64 withe status =0: %d\n",constants[instructions[pos+1]]);
           pos =pos+2;
	}else if(fn_status ==1){

	  printf("no of fnx:%d",no_of_funs);
	  push_stack(fun[function_position].local_constants[instructions[pos+1] -len_const_array]);
	  
	  printf("\npushing to stack at 64 with status =1 7c : %d\n",fun[function_position].local_constants[instructions[pos+1]]);
	  pos = pos+2;
	  
	}
      }
  }else if(ins == 131){ // call function
    int l;
    printf("inside call");
      int  temp =pos,args_length=0,hop=0,which_function;
      onethirtyone =pos;
      pos = pos+1;
      args_length = instructions[pos]; // first argument of function call showing taking how many argumnts
    
      hop = (args_length+1);
      printf("\nstore_array:");
      for(l=0;l<3;l++){

	printf("%d\t",store_array[l]);
	
	
      }
      
      while(hop>0){

        pos = pos-3;
	printf("\nstack element pushing at call: %d\n",store_array[instructions[pos-3]]);
        push_stack(store_array[instructions[pos]]);

       
        hop--;
     }
      which_function = pop();
      function_position = which_function;
      printf("which fn:%d\t",which_function);
      pos = run_func(which_function,onethirtyone,instructions,store_array,args_length);
    
    
    
  }else if(ins == 124){ // 7c push refrence of local_varnames to stack

    push_stack(const_array[instructions[pos+1]]);
    pos = pos+2;

  }else if(ins == 125){

    int next_pos = pos+1;
    int val= instructions[next_pos];
    int top = pop();
    printf("storing %d\n",top);
    const_array[val] = top;
    pos = pos+2;
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
    
  }else if (ins == 22){ // binary modulo %

    int bottom =0, top=0;
    top = pop();
    bottom = pop();
    printf("top: %d bottom : %d\t",top,bottom);
    push_stack(modulo(bottom,top));

  }else if(ins == 23){ // 17 sum

    int bottom =0, top=0;
    top =pop();
    bottom =pop();
    printf("top: %d bottom : %d\t",top,bottom);
    push_stack(sum(top,bottom));
    
  }else if(ins ==24){ // 18 sub;
    int bottom=0,top=0;
    top=pop();
    bottom=pop();
    printf("bottom: %d top %d\n",bottom,top);
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






//other fns

int condition_if(int pos, int *instructions, int *constants,int fn_status){

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
	if(fn_status ==1)
           return hop+(pos-9);
	else
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


int run_func(int fun_pos, int old_pos, int* instructions, int* store_array, int no_of_args){

  if(no_of_args = 2){

    int len_of_fn = fun[fun_pos].ln_of_fn_body, in_function;
    printf("lnof functin: %d",len_of_fn);
    printf("fns pos%d",fun_pos);
    int l=0;
    for(l=0;l<len_of_fn-1;l++){

      fn_instructions[l] = fun[fun_pos].fn_body[l];
      printf("inst:%d\t",fn_instructions[l]);
      
    }
    in_function =1;
    interpreter_loop(fn_instructions,const_array,len_of_fn, in_function);
    
  }
  return onethirtyone+3;
}
