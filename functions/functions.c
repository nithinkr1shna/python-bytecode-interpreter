#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/include.h"
#include "../headers/htod.h"

extern int len_const_array;
extern int *const_array;

int stack_size;
int *stack;

int push(int a){
  int *ptr;
  static int i = 0;

  if (i == 0){
    const_array = (int *)malloc(sizeof(int) * 1);
    if (const_array == 0){
      perror("error in malloc: push()");
      exit(1);
    }
    *const_array = a;
  }else{
    ptr = (int *)realloc(const_array, sizeof(int) * i + 1);
    if (ptr == 0){
      perror("error in realloc: push()");
      free(const_array);
      exit(1);
    }
    *(ptr + i) = a;
  }

  i = i + 1;
  return i;
}

char *strrev(char *str){
  int start, end, len;
  char temp, *ptr = NULL;

  len = strlen(str);
  ptr = malloc(sizeof(char) * (len + 1));
  if (ptr == 0){
    perror("error in malloc: strrev()");
    exit(1);
  }
  ptr = strcpy(ptr, str);
  for (start = 0, end = len - 1; start <= end; start++, end--){
    temp = ptr[start];
    ptr[start] = ptr[end];
    ptr[end] = temp;
  }

  return ptr;
}

int pop()
{
  int top;
  int index = stack_size - 1;
  top = stack[index];
  stack_size = stack_size - 1;
  return top;
}

void push_stack(int a){
  static int l = 0;
  int *ptr;
  if (l == 0){
    stack = (int *)malloc(sizeof(int) * 1);
    if (stack == 0){
      perror("error in malloc: push_stack()");
      exit(1);
    }
    *stack = a;
  }
  else{
    ptr = (int *)realloc(stack, sizeof(int) * l + 1);
    if (ptr == 0){
      perror("error in realloc : push_stack()");
      free(stack);
      exit(1);
    }
    *(ptr + l) = a;
  }

  l = l + 1;
  stack_size = l;
}

unsigned char *next_byte(unsigned char *hex_str_p, char *opcode){
  opcode[0] = *hex_str_p;
  opcode[1] = *(hex_str_p + 1);
  opcode[2] = '\0';
  hex_str_p++;
  hex_str_p++;

  return hex_str_p;
}

int sum(int a, int b){
  return a + b;
}

int sub(int a, int b){
  return a - b;
}

int mul(int a, int b){
  return a * b;
}

int divi(int a, int b){
  return a / b;
}

int modulo(int a, int b){
  return a % b;
}
