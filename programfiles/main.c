#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include "../headers/htod.h"
#include "../headers/functions.h"

#define SIZE 256
#define MGC 4
#define TIMESTAMP 4
#define MGC_NUM "03F30D0A" //magic number for python 2.7.10

struct
function {

  int fn_body[SIZE];
  int ln_of_fn_body;
  int fn_name[SIZE];
  int ln_of_name;
  int local_constants[SIZE];
  int ln_local_constants;
  int cnt_local_variables;
  int local_vars_names[SIZE];
  int module_name[SIZE];

}
fun[SIZE];

int * const_array;
int * dup_const_array[SIZE];
int * name_array;
int instruction_array[SIZE * 8];
int lenofinstr = 0, k = 0, l = 0;
int struct_index = 0;
int no_of_funs = 0;

//int const_array_pos[SIZE];

int i = 0, counter = 0, len_const_array = 0;
char * var_array[SIZE];
int no_fns;

//-----------------------------------------
// external functions

int push(int);
void start_interpreter(int * , int * );
int htod(char * );
unsigned char * next_byte(unsigned char * , char * opcode);

void print_var_array(int);
unsigned char * create_var_array(char * , int, unsigned char * , int);

int main(int argc, char ** argv) {
  printf("Python Bytecode Interpreter\nVersion: 2.7.10\n");
  if (argc < 2) {
    printf("Need compiled python file as cmd line argument\nProgram exited\n");
    exit(0);
  } else {
    FILE * pyc_pointer;
    size_t n;
    time_t time;
    int start = 0, counter_local_const = 0;
    int len, check = 0, counter = 0, opcode_seventyfour = 0, counter_mgc = MGC, counter_ts = TIMESTAMP;
    unsigned char buffer[2], hex_str[SIZE * 8];
    unsigned char * hex_str_p = hex_str;
    char opcode[3], values[8], timestamp[8], values1[8], values2[8], values3[8], values4[8], values5[8], values6[8], magic[8];
    char magic_of_this_version[8];
    int i, j;
    pyc_pointer = fopen(argv[1], "rb");

    if (pyc_pointer) {
      while (!feof(pyc_pointer)) {
        fread(buffer, sizeof(char), 1, pyc_pointer);
        hex_str_p += sprintf(hex_str_p, "%02X", (unsigned char) buffer[0]); //converting binary to hex 
      }
    } else {
      printf("Error, opening file %s\n", argv[1]);
      exit(0);
    }
    *(hex_str_p + 1) = '\0';
    fclose(pyc_pointer);
    hex_str_p = hex_str;
    printf("\n");
    //copying magic to magic and comparing magic to 2.7.10

    i = counter_mgc;
    while (i > 0) {
      hex_str_p = next_byte(hex_str_p, opcode);
      if (i == counter_mgc)
        strcpy(magic, opcode);
      else
        strcat(magic, opcode);
      i--;
    }
    if (strcmp(magic, MGC_NUM) != 0) {
      printf("\nThis pyvm only supports version 2.7.10\n");
      exit(0);
    }

    //copying timestamp of pyc file  to timestamp
    i = counter_ts;
    while (i > 0) {
      hex_str_p = next_byte(hex_str_p, opcode);
      if (i == counter_mgc)
        strcpy(timestamp, opcode);
      else
        strcat(timestamp, opcode);
      i--;
    }

    //skipping mainfun attributes;
    i = 16;
    while (i > 0) {
      hex_str_p = next_byte(hex_str_p, opcode);
      i--;
    }

    hex_str_p = next_byte(hex_str_p, opcode);
    hex_str_p = next_byte(hex_str_p, opcode);

    // getting the length of main instructions 

    if (strcmp(opcode, "73") == 0 && start == 0) {
      start = 1;
      //get next 4 bytes = length of instructions.
      for (i = 4; i > 0; i--) {
        hex_str_p = next_byte(hex_str_p, opcode);
        if (i == 4)
          strcpy(values, strrev(opcode));
        else
          strcat(values, strrev(opcode));
      }
    }
    lenofinstr = htod(strrev(values));

    i = 0;
    j = 0;
    while ( * hex_str_p) {
      hex_str_p = next_byte(hex_str_p, opcode);
      if (i < lenofinstr) {
        instruction_array[i] = htod(opcode);
        if (strcmp(opcode, "84") == 0) // checking if MAKE FUNCTION present in the instruction.
          no_fns++;
        no_of_funs = no_fns;
      } else if (strcmp(opcode, "73") == 0 && no_fns > 0) { // start of functions
        int function_body = 0, local_vars = 0, module_name = 0, fn_name = 0, counter = 0, lenofargmnts = 0;
        for (j = 4; j > 0; j--) {
          hex_str_p = next_byte(hex_str_p, opcode);
          if (j == 4)
            strcpy(values2, strrev(opcode));
          else
            strcat(values2, strrev(opcode));
        }

        int lenofarg = htod(strrev(values2));
        fun[struct_index].ln_of_fn_body = lenofarg; // assigning len of fn body [ASSIGN]
        counter = lenofarg;

        while (counter > 0) { // saving fun_body onto struct
          hex_str_p = next_byte(hex_str_p, opcode);
          fun[struct_index].fn_body[k] = htod(opcode); // assigning function body
          k++;
          counter--;
        }

        hex_str_p = next_byte(hex_str_p, opcode);
        while (strcmp(opcode, "74") != 0) { // for local variables / skiping till local vars
          if (strcmp(opcode, "69") == 0) { // looking for local constants.
            // creating local constants.
            for (l = 4; l > 0; l--) {
              hex_str_p = next_byte(hex_str_p, opcode);
              if (l == 4)
                strcpy(values6, strrev(opcode));
              else
                strcat(values6, strrev(opcode));
            }

            fun[struct_index].local_constants[counter_local_const] = htod(strrev(values6));
            counter_local_const++;
          }

          fun[struct_index].ln_local_constants = counter_local_const;
          hex_str_p = next_byte(hex_str_p, opcode);
        }

        int total_no_of_local_vars = 0;
        while (strcmp(opcode, "74") == 0) { // now at 74
          for (l = 4; l > 0; l--) {
            hex_str_p = next_byte(hex_str_p, opcode);
            if (l == 4)
              strcpy(values3, strrev(opcode));
            else
              strcat(values3, strrev(opcode));
          }
          lenofargmnts = htod(strrev(values3));
          l = 0;
          while (lenofargmnts > 0) {
            hex_str_p = next_byte(hex_str_p, opcode);
            fun[struct_index].local_vars_names[l] = htod(opcode); // stores decimal of hex of local variable name;
            l++;
            lenofargmnts--;
          }
          total_no_of_local_vars++;
          hex_str_p = next_byte(hex_str_p, opcode);
        }
        fun[struct_index].cnt_local_variables = total_no_of_local_vars;
        hex_str_p = next_byte(hex_str_p, opcode);

        while (strcmp(opcode, "73") != 0) { // skip until 73
          hex_str_p = next_byte(hex_str_p, opcode);
        }

        l = 0;
        for (l = 4; l > 0; l--) {
          hex_str_p = next_byte(hex_str_p, opcode);
          if (l == 4)
            strcpy(values4, strrev(opcode));
          else
            strcat(values4, strrev(opcode));
        }
        lenofargmnts = 0;
        lenofargmnts = htod(strrev(values4));
        l = 0;
        while (lenofargmnts > 0) {
          hex_str_p = next_byte(hex_str_p, opcode);
          fun[struct_index].module_name[l] = htod(opcode); // storing module name .
          l++;
          lenofargmnts--;
        }
        hex_str_p = next_byte(hex_str_p, opcode);
        while (strcmp(opcode, "74") != 0) {
          hex_str_p = next_byte(hex_str_p, opcode); //skip until 74;
        }
        l = 0;
        for (l = 4; l > 0; l--) {
          hex_str_p = next_byte(hex_str_p, opcode);
          if (l == 4)
            strcpy(values5, strrev(opcode));
          else
            strcat(values5, strrev(opcode));
        }
        lenofargmnts = 0;
        lenofargmnts = htod(strrev(values5));
        fun[struct_index].ln_of_name = lenofargmnts; // assigning len of fun name [ASSIGN]
        l = 0;
        while (lenofargmnts > 0) {
          hex_str_p = next_byte(hex_str_p, opcode);
          fun[struct_index].fn_name[l]; // storing fun name
          l++;
          lenofargmnts--;
        }
        no_fns--; 
        struct_index++;
      } else if (strcmp(opcode, "69") == 0) {
        for (j = 4; j > 0; j--) {
          hex_str_p = next_byte(hex_str_p, opcode);
          if (j == 4)
            strcpy(values1, strrev(opcode));
          else
            strcat(values1, strrev(opcode));
        }
        len_const_array = push(htod(strrev(values1)));
      }
      i++;
    }
    start_interpreter(instruction_array, const_array);
  }
}
