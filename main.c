#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "htod.h"
#include "functions.h"
//#include "opecodes.h"
#define SIZE 256
#define MGC_TMSTMP 8



int *const_array;
int *name_array;
int instruction_array[SIZE *8];
int lenofinstr=0;

//int const_array_pos[SIZE];

int i=0,counter=0,len_const_array = 0;
char *var_array[SIZE];

//-----------------------------------------
// external functions

int push(int);
void start_interpreter(int*, int*);
int htod(char*);
unsigned char *next_byte(unsigned char*,char *opcode);


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
    int start=0;
    int len,check=0,counter=0,opcode_seventyfour=0,counter_mgc=MGC_TMSTMP;
    unsigned char buffer[2],hex_str[SIZE*8];
    unsigned char *hex_str_p = hex_str;
    char opcode[3], values[8],magic[8],values1[8];
    
    int i,j;
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


    
   //copying magic and timestamp to string magic 
   i =counter_mgc;
   while(i>0){
	
     hex_str_p = next_byte(hex_str_p,opcode);
     if(i==counter_mgc)
	strcpy(magic,opcode);
     else
	strcat(magic,opcode);
	    
     i--;
   }

   
   //skipping mainfun attributes;
   i=16;
   while(i>0){

     hex_str_p = next_byte(hex_str_p,opcode);
     i--;
	
   }

   hex_str_p = next_byte(hex_str_p,opcode);
   hex_str_p = next_byte(hex_str_p,opcode);
	
   printf("opcode%s\n",opcode);
   if(strcmp(opcode,"73")==0 && start==0){
      start =1;
      //get next 4 bytes = length of incstructions.
      for(i=4;i>0;i--){

	hex_str_p = next_byte(hex_str_p,opcode);
	if(i==4)
	   strcpy(values,strrev(opcode));
	else
	   strcat(values,strrev(opcode));
      }

   } 
      
   //printf("in strng%s\t",values);
   lenofinstr = htod(strrev(values));
   printf("length of inst %d\n",lenofinstr);

   i=0;
   j=0;
   while(*hex_str_p){

     
    
      
      hex_str_p = next_byte(hex_str_p,opcode);
      printf("%s\t",opcode);
      
      
      if(i<lenofinstr){
	instruction_array[i] = htod(opcode);
	printf("%d \t%d\n",htod(opcode),i);

      }else if(strcmp(opcode,"69")==0){

	 for(j=4;j>0;j--){
	 
         hex_str_p = next_byte(hex_str_p, opcode);
	 if(j==4)
           strcpy(values1,strrev(opcode));
	 else
	   strcat(values1,strrev(opcode));
	 //printf("%s",opcode);
       }
	//printf("%s\t",strrev(values));
       printf("vals %d\t", htod(strrev(values1))); 
       //push value to array;
       len_const_array = push(htod(strrev(values1)));
       printf("length %d",len_const_array);
      }
	 
      i++;
      }
      

   printf("\n");
   for(i=0;i<len_const_array;i++){

     printf("%d\n",const_array[i]);
   }


   start_interpreter(instruction_array, const_array);
   }
  }
