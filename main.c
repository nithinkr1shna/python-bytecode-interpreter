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
//-----------------------------------------


void print_var_array(int);
void printll(instruction_node*);
void push_ll(instruction_node**, int, int, int);
int deter_len(char *opcode);
unsigned char *next_byte(unsigned char*,char *opcode);
unsigned char *create_var_array(char*, int, unsigned char*, int);

int main(int argc , char **argv){

  if(argc < 2){

    printf("Need compiled python file as cmd line argument\n Program exited\n");
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

unsigned char *next_byte(unsigned char *hex_str_p, char *opcode){

     opcode[0] = *hex_str_p;
     opcode[1] = *(hex_str_p +1);
     opcode[2] = '\0';
     //printf("%s\t",opcode);
     hex_str_p++;
     hex_str_p++;
     return hex_str_p;
     

  
}

int deter_len(char *opcode){

  if(strcmp(opcode,"73") == 0 || strcmp(opcode,"69") == 0) // start
    return 4;
  else if(strcmp(opcode, "64") == 0 || strcmp(opcode, "5A") == 0 || strcmp(opcode, "65") == 0) //load const, store_name , load_name
    return 2;
  else if(strcmp(opcode,"17") == 0 || strcmp(opcode,"47") == 0 || strcmp(opcode,"48") == 0 || strcmp(opcode, "53") == 0 || strcmp(opcode,"18") == 0 || strcmp(opcode,"15") == 0 || strcmp(opcode, "14") == 0) //binary_add, print_item, print_new_line
    return 0;
  else if(strcmp(opcode,"74") == 0)
    return 4;
  else
    return -1;
}

void printll(instruction_node *ll){
  printf("call\t");
  instruction_node *current;
  for(current = ll;current != NULL; current= current->next){

    // printf("inside ll\t'");
    printf("\n%d\t",current->opcode);
    printf("%d\t",current->pos);
    printf("%d\n",current->snd_pos);
    
  }
  
}

void push_ll(instruction_node **headRef, int opcode, int pos, int snd_pos){ // creates ll on the go

  instruction_node *current = *headRef;
  instruction_node* newNode;
  newNode =(instruction_node*)malloc(sizeof(instruction_node));
  if(newNode == NULL){

    printf("Cant Allocate\n");
    exit(0);
  }
  newNode->opcode = opcode;
  newNode->pos = pos;
  newNode->snd_pos = snd_pos;
  newNode->next = NULL;
  //printf("%d %d %d\t",opcode,pos,snd_pos);
  
  if(current == NULL){
    
    *headRef = newNode;
    
    
  }else{
    while(current->next != NULL){
      current = current->next;
    }
    

    current->next = newNode;
  }
}


void print_var_array(int opcode_seventyfour){
  printf("%d",opcode_seventyfour);
  int i;
  for(i= opcode_seventyfour-1; i> 0;i--){

    printf("%s\n",var_array[i-(opcode_seventyfour-2)]);
  }
}
