#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include "opecodes.h"
#define SIZE 256

typedef struct instruction{

  int opcode;
  int pos;
  int snd_pos;
  struct instruction* next;
  
}instruction_node;

instruction_node *head = NULL;

int const_array[SIZE];
int const_array_pos[SIZE];

int i=0;

void printll(instruction_node*);
int deter_len(char *opcode);
unsigned char *next_byte(unsigned char*,char *opcode);


int main(int argc , char **argv){

  if(argc < 2){

    printf("Need compiled python file as cmd line argument\n Program exited\n");
    exit(0);
  }else{
    FILE *pyc_pointer;
    size_t n;
    int len;
    unsigned char buffer[2],hex_str[SIZE*2];
    unsigned char *hex_str_p = hex_str;
    char opcode[3];
    int counter = 0;
    
    pyc_pointer = fopen(argv[1],"rb");
    //printf("%s",argv[1]);
    if(pyc_pointer){
      while(!feof(pyc_pointer)){

	fread(buffer,sizeof(char),1,pyc_pointer);
	hex_str_p += sprintf(hex_str_p,"%02x",(unsigned char)buffer[0]); //converting binary to hex 

	
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

       //printf("load_const");
       head = malloc(sizeof(instruction_node));
       head->opcode = 64;
       hex_str_p = next_byte(hex_str_p,opcode);
       printf("pos %d",atoi(opcode));
       head->pos = atoi(opcode);
       hex_str_p = next_byte(hex_str_p,opcode);
       printf(":pos_s%d",atoi(opcode));
       head->snd_pos =atoi(opcode);
       head->next = NULL;
        
     }
     
    }
    printf("\n");
    printll(head);
    
  }
  
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

  if(strcmp((char *)opcode,"73") == 0) // start
    return 4;
  else if(strcmp(opcode, "64") == 0 || strcmp(opcode, "5A") == 0) //load const
    return 2;
  else
    return -1;
}

void printll(instruction_node *ll){

  instruction_node *current = ll;
  while(current != NULL){

    printf("\n%d\t",current->opcode);
    printf("%d\t",current->pos);
    printf("%d\n",current->snd_pos);
    current = current->next;
  }
  
}
