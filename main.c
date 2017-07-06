#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "htod.h"
#include "functions.h"
//#include "opecodes.h"
#define SIZE 256
#define MGC_TMSTMP 8


 struct function{

  int fn_body[SIZE];
  int ln_of_fn_body;
  int fn_name[SIZE];
  int ln_of_name;
  int local_constants[SIZE];
  int ln_local_constants;
  int cnt_local_variables;
  int local_vars_names[SIZE];
  int module_name[SIZE];
 
  
}fun[SIZE];

int *const_array;
int *dup_const_array[SIZE];
int *name_array;
int instruction_array[SIZE *8];
int lenofinstr=0,k=0,l=0;
int struct_index=0;
int no_of_funs =0;

//int const_array_pos[SIZE];

int i=0,counter=0,len_const_array = 0;
char *var_array[SIZE];
int no_fns;

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
    int start=0, counter_local_const=0;
    int len,check=0,counter=0,opcode_seventyfour=0,counter_mgc=MGC_TMSTMP;
    unsigned char buffer[2],hex_str[SIZE*8];
    unsigned char *hex_str_p = hex_str;
    char opcode[3], values[8],magic[8],values1[8],values2[8],values3[8],values4[8], values5[8],values6[8];
    
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

   // getting the length of main instructions 
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
   printf("length of inst %d\n",lenofinstr); // length of instructions

   i=0;
   j=0;
   while(*hex_str_p){

     
    
      
      hex_str_p = next_byte(hex_str_p,opcode);
      printf("%s\t",opcode);
     
      
      
      if(i<lenofinstr){
	instruction_array[i] = htod(opcode);
	printf("%d \t%d\n",htod(opcode),i);
	if(strcmp(opcode,"84")==0) // checking if MAKE FUNCTION present in the instruction.
	   no_fns++;
	no_of_funs = no_fns;

      }else if(strcmp(opcode,"73")==0 && no_fns >0){ // start of functions
        
	int function_body=0,local_vars=0, module_name=0,fn_name=0,counter =0,lenofargmnts=0;
	for(j=4;j>0;j--){
	 
         hex_str_p = next_byte(hex_str_p, opcode);
	 if(j==4)
           strcpy(values2,strrev(opcode));
	 else
	   strcat(values2,strrev(opcode));
	 //printf("%s",opcode);
       }
	int lenofarg =htod(strrev(values2));
					      
	fun[struct_index].ln_of_fn_body =lenofarg; // assigning len of fn body [ASSIGN]
	
	counter = lenofarg;
	printf("arg of 73 is %d\t", lenofarg);
        k=0;
	while(counter > 0){ // saving fun_body onto struct

	  hex_str_p = next_byte(hex_str_p, opcode);
	  printf("nxt byte%s",opcode);
	 
	  printf("counter %d\t",counter);
	  fun[struct_index].fn_body[k] =htod(opcode); // assigning function body
	  k++;
	  counter--;
	}


        
	
	
	hex_str_p = next_byte(hex_str_p,opcode);
	
	  while(strcmp(opcode,"74") != 0){ // for local variables / skiping till local vars

	    printf("in skip 74:opcode : %s\n",opcode);
	    
	    if(strcmp(opcode,"69") == 0){ // looking for local constants.
	      printf("in local constants %s \n",opcode);
	      // creating local constants.
	       for(l=4;l>0;l--){
	 
		 hex_str_p = next_byte(hex_str_p, opcode);
	           if(l==4)
                      strcpy(values6,strrev(opcode));
	           else
	              strcat(values6,strrev(opcode));
	              //printf("%s",opcode);
               }
	       
	       fun[struct_index].local_constants[counter_local_const] = htod(strrev(values6));
	       counter_local_const++;
	     }
	    
	    fun[struct_index].ln_local_constants = counter_local_const;
	    hex_str_p = next_byte(hex_str_p,opcode);
	  }

	  int total_no_of_local_vars =0;
	  while(strcmp(opcode,"74") ==0){  // now at 74
            //printf("local %s\n",opcode);
	    for(l=4;l>0;l--){

	       hex_str_p = next_byte(hex_str_p, opcode);
	       if(l==4)
                  strcpy(values3,strrev(opcode));
	      else
	         strcat(values3,strrev(opcode));
	    }
	    lenofargmnts = htod(strrev(values3));
	    l=0;
	    //printf("string at 74 %s\n",strrev(values3));
	    //printf("len of argumnts at 74 %d\n",lenofargmnts);
	    while(lenofargmnts >0){

	      hex_str_p = next_byte(hex_str_p,opcode);
              fun[struct_index].local_vars_names[l] = htod(opcode); // stores decimal of hex of local variable name;

	      l++;
	      lenofargmnts--;
	    }
	    total_no_of_local_vars++;
	    hex_str_p = next_byte(hex_str_p,opcode);
	  }

	  //printf("total no of local vars %d\n", total_no_of_local_vars);
	fun[struct_index].cnt_local_variables = total_no_of_local_vars;
        hex_str_p = next_byte(hex_str_p, opcode);
	//printf("before %s\n",opcode);

	while(strcmp(opcode,"73") !=0) {// skip until 73
	  //printf("skip until 73\n");
	  hex_str_p = next_byte(hex_str_p, opcode);
	}

	l=0;
	for(l=4;l>0;l--){

	    hex_str_p = next_byte(hex_str_p, opcode);
	       if(l==4)
                  strcpy(values4,strrev(opcode));
	      else
	         strcat(values4,strrev(opcode));
	}
	lenofargmnts =0;
	//printf("string : %s\n",strrev(values4));
	lenofargmnts = htod(strrev(values4));
      l=0;
      //printf("len of argmnts %d\n",lenofargmnts);
      while(lenofargmnts >0){
	

	hex_str_p = next_byte(hex_str_p,opcode);
	//printf("%s\n",opcode);
	fun[struct_index].module_name[l] = htod(opcode); // storing module name .
	l++;
	lenofargmnts--;
	
      }

      hex_str_p = next_byte(hex_str_p,opcode);
      while(strcmp(opcode,"74") !=0){
	//printf("skip until 74\n");
	hex_str_p = next_byte(hex_str_p,opcode); //skip until 74;
      }

      l=0;
      for(l=0;l>0;l--){

	 hex_str_p = next_byte(hex_str_p, opcode);
	 if(j==4)
              strcpy(values5,strrev(opcode));
	 else
	      strcat(values5,strrev(opcode));
      }
      lenofargmnts =0;
      lenofargmnts = htod(strrev(values5));
      
      fun[struct_index].ln_of_name = lenofargmnts; // assigning len of fun name [ASSIGN]
      
      l=0;
      while(lenofargmnts >0){

	hex_str_p = next_byte(hex_str_p,opcode);
	fun[struct_index].fn_name[l]; // storing fun name
	l++;
	lenofargmnts--;
      }
      
      no_fns--;
	  // struct index counter 
      struct_index++;

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
       printf("length of const array %d",len_const_array);
      }
	 
      i++;
      }
      

   printf("\n");
   for(i=0;i<len_const_array;i++){

     //dup_const_array[i] = const_array[i];
     printf("%d\n",const_array[i]);
   }

   
   
   //---------------------------------check-----to delt
   int s;
   printf("heyyyaaa %d",no_of_funs);
   

     
   for(s=0;s<fun[0].ln_of_fn_body;s++){
    
     printf("\n%d\n",fun[0].fn_body[s]);
   }
   //----------------check end -----------------


   //call for interpreter.
   start_interpreter(instruction_array, const_array);
   }
  }
