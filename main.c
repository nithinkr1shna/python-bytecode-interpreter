#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "htod.h"
#include "functions.h"
//#include "opecodes.h"
#define SIZE 256

typedef struct instruction{

  int opcode;
  int pos;
  int snd_pos;
  struct instruction* next;
  
}instruction_node;


instruction_node *head = NULL;

int *const_array;

//int const_array_pos[SIZE];

int i=0,counter=0,len_const_array = 0;
char *var_array[SIZE];

//-----------------------------------------
// external functions

int push(int);
void start_interpreter(instruction_node*, int*);
int htod(char*);
void printll(instruction_node*);
void push_ll(instruction_node**, int, int, int);
int deter_len(char *opcode);
unsigned char *next_byte(unsigned char*,char *opcode);
unsigned char *push_ll_wrapper(unsigned char*,instruction_node*, int, char*,int, int, int );

//-----------------------------------------


void print_var_array(int);




unsigned char *create_var_array(char*, int, unsigned char*, int);

int main(int argc , char **argv){

  if(argc < 2){

    printf("Need compiled python file as cmd line argument\nProgram exited\n");
    exit(0);
  }else{
    FILE *pyc_pointer;
    size_t n;
    int len,check=0,counter=0,opcode_seventyfour=0;
    unsigned char buffer[2],hex_str[SIZE*2];
    unsigned char *hex_str_p = hex_str;
    char opcode[3], values[8];
    int counter_pos_arg = 0,j;
    int opcode_arg,pos_arg,snd_pos_arg;
    pyc_pointer = fopen(argv[1],"rb");
    //printf("%s",argv[1]);
    if(pyc_pointer){
      while(!feof(pyc_pointer)){

	fread(buffer,sizeof(char),1,pyc_pointer);
	hex_str_p += sprintf(hex_str_p,"%02X",(unsigned char)buffer[0]); //converting binary to hex 

	
      }

    }else{

      printf("Error, opening file %s\n",argv[1]);
    }
    *(hex_str_p+1) ='\0';
    fclose(pyc_pointer);
    hex_str_p = hex_str;
    printf("\n");

    
    while(*hex_str_p){ //loop for creating instructions and assosciated data structure
      hex_str_p = next_byte(hex_str_p,opcode);
 
      len =deter_len(opcode); // determining the offset of each instruction
     //printf("len %d\t", len);
     if(len == 4 && strcmp(opcode,"73") ==0 ){
      
       ; //do nothing
     
     }else if(len ==2 && strcmp(opcode,"64") == 0){ // 64 load_const 
       
       opcode_arg =64;
       hex_str_p = next_byte(hex_str_p,opcode);
       pos_arg =atoi(opcode);
       hex_str_p = next_byte(hex_str_p,opcode); 
       snd_pos_arg = atoi(opcode);
       push_ll(&head, opcode_arg,pos_arg,snd_pos_arg);
      
       
      
     
     }else if(len == 2 && strcmp(opcode,"5A") ==0){ // store name
       
       opcode_arg = htod("5A"); // converted 5A as 90 
       hex_str_p = next_byte(hex_str_p,opcode);
       pos_arg = atoi(opcode);
       hex_str_p = next_byte(hex_str_p,opcode);
       snd_pos_arg = atoi(opcode);
       push_ll(&head,opcode_arg, pos_arg, snd_pos_arg);
       
	
       
     }else if(len == 2 && strcmp(opcode,"65") == 0){ // load name

       opcode_arg = 65;
       hex_str_p = next_byte(hex_str_p, opcode);
       pos_arg = atoi(opcode);
       hex_str_p = next_byte(hex_str_p, opcode);
       snd_pos_arg = atoi(opcode);
       push_ll(&head, opcode_arg, pos_arg, snd_pos_arg);
       
     }else if(len ==0 && strcmp(opcode,"17") == 0){ // binary add

       opcode_arg =17;
       pos_arg =-1;
       snd_pos_arg =-1;
       push_ll(&head,opcode_arg,pos_arg,snd_pos_arg);
       
     }else if(len ==0  && strcmp(opcode,"18") == 0){

       opcode_arg = 18;
       pos_arg =-1;
       snd_pos_arg =-1;
       push_ll(&head,opcode_arg,pos_arg,snd_pos_arg);
       
     }else if(len ==0 && strcmp(opcode,"15") == 0){

       opcode_arg =15;
       pos_arg =-1;
       snd_pos_arg =-1;
       push_ll(&head,opcode_arg,pos_arg,snd_pos_arg);
       
     }else if(len == 0 && strcmp(opcode,"14") ==0){

       opcode_arg =14;
        pos_arg =-1;
       snd_pos_arg =-1;
       push_ll(&head,opcode_arg,pos_arg,snd_pos_arg);
       
     }else if(len ==0 && strcmp(opcode,"47") == 0){

       opcode_arg =47;
       pos_arg =-1;
       snd_pos_arg =-1;
       push_ll(&head, opcode_arg, pos_arg, snd_pos_arg);
       
     }else if(len == 0 && strcmp(opcode,"48") == 0){

       opcode_arg =48;
       pos_arg = -1;
       snd_pos_arg = -1;
       push_ll(&head, opcode_arg, pos_arg, snd_pos_arg);
       
     }else if(len == 0 && strcmp(opcode,"53") == 0){

       opcode_arg = 53;
       pos_arg= -1;
       snd_pos_arg = -1;
       push_ll(&head, opcode_arg, pos_arg, snd_pos_arg);
       
     }else if(len == 4 && strcmp(opcode,"74") == 0){
       opcode_seventyfour = opcode_seventyfour +1;
       opcode_arg = 74;
       hex_str_p = next_byte(hex_str_p, opcode);
       pos_arg = atoi(opcode); //count of characters in the variable // use hextod
       counter_pos_arg = pos_arg; // count of characters in varaible set to counter
       hex_str_p = next_byte(hex_str_p,opcode); //skipped extra 3 bytes which defines the length of the variable.
       hex_str_p = next_byte(hex_str_p,opcode);
       hex_str_p = next_byte(hex_str_p,opcode);
       snd_pos_arg =-1;
       hex_str_p = create_var_array(opcode,counter_pos_arg,hex_str_p,opcode_seventyfour);
       push_ll(&head, opcode_arg, pos_arg, snd_pos_arg);
       
     }else if(len == 4 && strcmp(opcode, "69") ==0 ){ //store constants
       
        for(j=len;j>0;j--){
	 
         hex_str_p = next_byte(hex_str_p, opcode);
	 if(j==len)
           strcpy(values,strrev(opcode));
	 else
	   strcat(values,strrev(opcode));
	 //printf("%s",opcode);
       }
       printf("%s\t",strrev(values));
       printf("%d", htod(strrev(values))); 
       //push value to array;
       len_const_array = push(htod(strrev(values))); // pudhing to array of contants
       printf("const_array_len:%d",len_const_array); 
     }
    }
    
    printf("\n");
    // printll(head);
    //print_var_array(opcode_seventyfour);
    // call to do the operations
    //for(i=0;i<len_const_array;i++)
    //printf("%d\t",const_array[i]);
    start_interpreter(head,const_array);
  }
  
}

unsigned char *create_var_array(char *opcode, int counter, unsigned char *hex_str_p, int opcode_seventyfour){

  char *each_var = malloc(sizeof(char));
  
  int i;
 
  for(i=counter;i>0;i--){

    hex_str_p = next_byte(hex_str_p, opcode);
    strcpy(&each_var[i],opcode);

  }
  
  //char *element;
  // take each element and form a string of variable. and create a
  var_array[opcode_seventyfour] = each_var;
  return hex_str_p;
  
}


void print_var_array(int opcode_seventyfour){
  printf("%d",opcode_seventyfour);
  int i;
  for(i= opcode_seventyfour-1; i> 0;i--){

    printf("%s\n",var_array[i-(opcode_seventyfour-2)]);
  }
}
