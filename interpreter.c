#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "include.h"
#include  "functions.h"

instruction_node* pointer;
void loop(void);
void push_stack(int);
int pop(void);
void printll(instruction_node*);
void interpreter_loop(instruction_node*, int*);
int push_gen(int, int*);
extern int len_const_array;
extern int *const_array;
//int index_pos =0;
int const_pos_index=0, name_pos_index =0;
int count_of_start_end =0;
char str[5];
char snd_str[5];
char thrd_str[5];
char fourth_str[5];
int first,second;
instruction_node *current;
int if_condition =0;

void start_interpreter(instruction_node* instruction_set, int* constants){
  
  printll(instruction_set);
  interpreter_loop(instruction_set, constants);
}



void interpreter_loop( instruction_node* instructions, int *constants){

 
  
  //int load_name[256];
  //int load_const[256]; // used for instruction just above  opcode 6b(if) in instruction set
  
  int const_pos[256];
  int name_pos[256];
  
  for(current = instructions; current != NULL; current = current->next){
    
   
    
    sprintf(str, "%d",current->opcode);
    if(strcmp(str,"73") == 0){

      count_of_start_end++;
    }
    if(count_of_start_end > 1)
      break;
    

    if(strcmp(str,"65") == 0){  // 65 00 00
      if(if_condition)
	push_stack(const_array[current->pos]);
      
      first = const_array[current->pos];
      printf("first one %d",first);
      sprintf(snd_str,"%d",current->next->opcode);
      if(strcmp(snd_str,"65") == 0){  // 65 00 00 
	        if_condition =0;
         	second = const_array[current->next->pos];
		printf("snd %d",second);
	        sprintf(thrd_str, "%d",current->next->next->opcode);
	        if(strcmp(thrd_str,"17") == 0){  // 17 binary add

	               int sum = first + second;
		       printf("sum %d",sum);
	               push_stack(sum);
		       current = current->next->next;
	  
	        }else if(strcmp(thrd_str, "18") == 0){ //18 subtract

		        int diff = first - second;
		        push_stack(diff);
			current = current->next->next;

		}else if(strcmp(thrd_str ,"15") == 0){

		  int quotient = first / second;
		  push_stack(quotient);
		  current = current->next->next;
		  
		}else if(strcmp(thrd_str, "14") == 0){

		  int product = first * second;
		  push_stack(product);
		  current = current->next->next;
		}
		
        }else{
        
         	loop();
	
       } 
		
    }else if(strcmp(str,"47")==0){
      int x;
      x = pop();
      printf("val %d",x);
    }else if(strcmp(str,"48")==0){

      printf("\n");
   
    }else if(strcmp(str,"6E") ==0 || strcmp(str,"28") == 0){

      break;
    }
      
  }
} 


void loop(void){

         
	 sprintf(snd_str, "%d", current->next->opcode);
	     if(strcmp(snd_str,"17") == 0){

	       int top = pop();
	        printf("top %d", top);
	       int sum = first + top;
	       printf("last sum %d",sum);
	       push_stack(sum);
	       current = current->next;
	       loop();
	       
	     }else if(strcmp(snd_str,"18") == 0){ // subtract

	       int top = pop();
	       printf("top %d", top);
	       int diff = top-first;
	       push_stack(diff);
	       current = current->next;
	       loop();
	     }else if(strcmp(snd_str,"15") == 0){

	       int top = pop();
	       printf("top %d", top);
	       int quotient = first / top;
	       push_stack(quotient);
	       current = current->next;
	       loop();
	     }else if(strcmp(snd_str,"14") == 0){

	       int top = pop();
	       printf("top %d", top);
	       int product = first * top;
	       push_stack(product);
	       current = current->next;
	       loop();
	     }else if(strcmp(snd_str,"64") ==0){ // 65 00 00 , 64 00 00 , 107(if)

	         current = current->next;
		 int constant = const_array[current->pos];
	         //printf("64 %d", constant);
		 //printf("opcode %d",current->opcode); // at 64
		 current = current ->next;
		 sprintf(thrd_str,"%d",current->opcode);
		 if(strcmp(thrd_str,"107")==0){

		   if_condition = 1;
		   int operation = current->pos;
		   if(operation == 5){ // >=
		       //printf("\tval of first %d\t",first);
		       //printf("\tval of const %d\t",constant);
		       if(first >= constant){

		             interpreter_loop(current, const_array);
		       }else{

		             int opc = current->next->opcode;
		             //printf("opx%d",opc);
		             while( opc != 110){

		                 current = current->next;
		                 opc = current->opcode;
			         //printf("%d\t",opc);
			 
		             }
		       
		       }

		   }
		   if(operation == 0){ // <

		       if(first < constant){

		          interpreter_loop(current,const_array);
		       

		       }else{

		             int opc = current->next->opcode;
		             //printf("opx%d",opc);
		             while( opc != 110){

		                  current = current->next;
		                  opc = current->opcode;
			          //printf("%d\t",opc);
		           }
		     
		     }
		   
		  }
		  if(operation == 1){ // <=

		       if(first <= constant){

			  interpreter_loop(current,const_array);
		       

		       }else{

		          int opc = current->next->opcode;
		          //printf("opx%d",opc);
		          while( opc != 110){

		                 current = current->next;
		                 opc = current->opcode;
			         //printf("%d\t",opc);
		          }
		     
		     }
		 }
		 if(operation == 2){ // ==

		       if(first == constant){

		          interpreter_loop(current,const_array);
		       

		       }else{

		          int opc = current->next->opcode;
		          //printf("opx%d",opc);
		          while( opc != 110){

		                  current = current->next;
		                  opc = current->opcode;
			          //printf("%d\t",opc);
		          }
		     
		     }
		}
		if(operation == 3){ // !=

		     if(first != constant){

			 interpreter_loop(current,const_array);
		       

		     }else{

		           int opc = current->next->opcode;
		           //printf("opx%d",opc);
		           while( opc != 110){

		               current = current->next;
		               opc = current->opcode;
			       //printf("%d\t",opc);
		           }
		     
		    }
		}
		if(operation == 4){ // >

		     if(first > constant){

		 	 interpreter_loop(current,const_array);
		       

		     }else{

		           int opc = current->next->opcode;
		           //printf("opx%d",opc);
		           while( opc != 110){

		                current = current->next;
		                opc = current->opcode;
			        //printf("%d\t",opc);
		            }
		      
		     }
		}
		     
		   
		   
	   }
		 
       }
}
