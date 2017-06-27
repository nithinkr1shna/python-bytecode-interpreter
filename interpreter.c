#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "include.h"
#include "functions.h"

extern int len_const_array;
extern int *const_array;

instruction_node *current;
int binary_add_flag =0;
int binary_sub_flag = 0;
int store_array[256];
int index_store_array =0;
int loaded =0;
int end =0;
int while_loop_counter=0;
int trueness_of_loop =0;
  
void printll(instruction_node*);
void interpreter_loop(instruction_node*, int*);
void push_stack(int);
int pop(void);

void hop_back(int, instruction_node*, int*);
void start_interpreter(instruction_node *instruction_set, int *constants){

  printll(instruction_set);
  interpreter_loop(instruction_set, constants);
}

void interpreter_loop(instruction_node *instructions, int *constants){

  for(current = instructions; current !=NULL ; current = current->next){

    while_loop_counter++;
    int op = current->opcode;
    printf("opcode is%d\n", op);
    
   
    if(op == 65){
      int val = store_array[current->pos];
      push_stack(val);
      printf("\tsnd%d\t",val);
    }else if(op == 14){ // mul

      int val1=0,val2=0;
      val1 = pop();
      val2 =pop();
      push_stack(val1*val2);
      
    }else if(op == 18){ // sub

      int top = pop();
      int top2 = pop();
      push_stack( top2-top);
    }else if(op ==17){ // add

      int val =pop();
      int val2 = pop();
      printf("values %d %d", val,val2);
      push_stack(val+val2);
    }else if(op == 15){ // div

      int val = pop();
      int val1 = pop();
      push_stack(val1/val);
    }else if(op == 47){ // output

      int val = pop();
      printf("ans is %d\t",val);
    }else if(op == 71){

      int hops = while_loop_counter;
      while_loop_counter =0;
      printf("hops %d",hops);
    
	hop_back(hops+2, current, constants);
      
    }else if(op == 107){ //if
      while_loop_counter =0;
      int val1=0, val2=0;
      val1 =pop();
      val2 =pop();
      int operator = current->pos;
      printf("val1 %d, val2 %d\t", val1,val2);

      if(operator == 4){ // >

	if(val2 > val1){
           trueness_of_loop =1;
	   current =current->next;
	   printf("4four");
	   interpreter_loop(current, constants);
	  
	}else{
          
	  int opcode = current->next->opcode;
	  if(trueness_of_loop ==1){

	    while(opcode !=71){

	       opcode = current->next->opcode;
	          current= current->next;
	    }
	  }else{
	    
	       while(opcode != 110){
	   
	          opcode = current->next->opcode;
	          current= current->next;
	        }
	  }
	}
	
      
      }else if(operator = 5){
	
	if(val2 >= val1){
	  trueness_of_loop =1;
          current = current->next;
	  printf("5five");
	  interpreter_loop(current,constants);
	  
	}else{
          int opcode= current->next->opcode;
	  if(trueness_of_loop ==1){
	      while(opcode != 71){

	        opcode = current->next->opcode;
	        current= current->next;

	      }
	  }else{

		while(opcode != 110){

	        opcode = current->next->opcode;
	        current= current->next;
		
	      }
	  }
	}
      }else if(operator ==3 ){ // !=

	if(val2 != val1){
           trueness_of_loop =1;
	  current =current->next;
	  printf("three");
	  interpreter_loop(current,constants);
	  
	}else{

	   int opcode = current->next->opcode;
	   if(trueness_of_loop ==1){
	       while(opcode != 71){

	         opcode = current->next->opcode;
	         current= current->next;
	  
	       }
	   }else{

	     while(opcode != 110){

	        opcode = current->next->opcode;
	        current= current->next;
	   }
	
      }

	}
      }else if(operator == 2){ // ==

	if(val2 == val1){
           trueness_of_loop =1;
	  current = current->next;
	  printf("two");
	  interpreter_loop(current, constants);
	  
	}else{

	   int opcode = current->next->opcode;
	   if(trueness_of_loop ==1){
         	  while(opcode != 71){
 
	             opcode = current->next->opcode;
	            current= current->next;
	          }
	   }else{

	     while(opcode != 110){

	        opcode = current->next->opcode;
	        current= current->next;
	   }
	
         }

	}
      }else if(operator == 1){ // <=

	if(val2 <= val1){
           trueness_of_loop =1;
	  current = current->next;
	  printf("one");
	  interpreter_loop(current, constants);
	  
	}else{

	   int opcode = current->next->opcode;
	   if(trueness_of_loop ==1){
	      while(opcode != 71){

	        opcode = current->next->opcode;
	        current= current->next;
	      }
	   }else{

	     while(opcode != 110){

	        opcode = current->next->opcode;
	        current= current->next;
	      }
	
           }
       }
      }else if(operator == 0){ // <

	if(val2 < val1){
           trueness_of_loop =1;
	  current = current->next;
	  interpreter_loop(current, constants);
	  
	}else{

	   int opcode = current->next->opcode;
	   if(trueness_of_loop == 1){
	      while(opcode != 71){

	         opcode = current->next->opcode;
	         current= current->next;
	      }
	   }else{

	     while(opcode != 110){

	        opcode = current->next->opcode;
	        current= current->next;
	   }
	}
      }
     }
	
      

     
    
    }else if(op == 90){ //5a store name

      int previous_instr =current->prev->opcode;
      if( previous_instr== 64){
           int val = current->prev->pos;
           store_array[index_store_array] = constants[val];
	   printf("vals %d",constants[val]);
	   
      }else if(previous_instr == 17   || previous_instr == 18){

	    int val = pop();
	    store_array[current->pos] = val;
      }
      index_store_array++;
    }else if(op == 73){
        end++;
        if(end >1){
	  printf("\nbreakingg....end of prgrm\n");
	  exit(0);
	}
    }
  
    
    
    
  } // end of for
} // end of function interpreter_loop


// other functions

void hop_back(int count, instruction_node* addr, int* constants){

  instruction_node* current = addr;
  while(count >0){

    current = current->prev;
    count--;
    
  }
  printf("the current value %d", current->opcode);
  interpreter_loop(current, constants);
}
