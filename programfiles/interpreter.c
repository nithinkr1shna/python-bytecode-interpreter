#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "../headers/include.h"
#include "../headers/functions.h"
#include "../headers/opcode.h"

extern int len_const_array;
extern int * const_array;
extern int * dup_const_array;
//extern int *instruction_array;
extern int lenofinstr;
extern int no_of_funs;

int store_array[256];
int store_array_index = 0;
int status = 0;

int condition;
int make_counter = 0;
int fn_instructions[SIZE];
int onethirtyone = 0;
int function_position = 0;

int push_w(int, int, int * , int * , int);
int pop_w(int, int, int * , int * );
void interpreter_loop(int * , int * , int, int);
void push_stack(int);
int pop(void);
int sum(int, int);
int sub(int, int);
int mul(int, int);
int divi(int, int);
int modulo(int, int);
int condition_if(int, int * , int * , int);
int while_loop(int, int * , int * , int);
int run_func(int, int, int * , int * , int);
char values9[4];

void start_interpreter(int * instruction_array, int * const_array) {
  int fn_status = 0;
  interpreter_loop(instruction_array, const_array, lenofinstr, fn_status);

}

void interpreter_loop(int * instructions, int * constants, int length, int fn_status) {
  int i, opc;
  for (i = 0; i < length; i++) {
    opc = instructions[i];
    switch (opc) {
    case LOAD_CONST: //64
      i = push_w(opc, i, instructions, constants, fn_status);
      break;
    case STORE_NAME: //5a
      i = pop_w(opc, i, instructions, constants);
      break;
    case LOAD_NAME: //65
      i = pop_w(opc, i, instructions, store_array);
      break;
    case BINARY_MULTIPLY: //14 mul
      pop_w(opc, i, instructions, store_array);
      break;
    case BINARY_DIVIDE: // 15 div
      pop_w(opc, i, instructions, store_array);
      break;
    case BINARY_MODULO: // 16 modulo %
      pop_w(opc, i, instructions, store_array);
      break;
    case BINARY_SUM: //17 sum
      pop_w(opc, i, instructions, store_array);
      break;
    case BINARY_SUB: // 18 sub
      pop_w(opc, i, instructions, store_array);
      break;
    case IF: //6b if
      i = condition_if(i, instructions, store_array, status);
      break;
    case JUMP_WHILE: //71 absolute jump while loop
      // jump hence present while loop
      i = while_loop(i, instructions, store_array, fn_status);
      break;
    case PRINT_ITEM: //47 print
      printf("%d\n", pop());
      break;
    case CALL_FUNCTION: // call fn 83
      i = push_w(opc, i, instructions, store_array, fn_status);
      fn_status = 0;
      break;
    case LOAD_FAST: // load ref 7c
      i = push_w(opc, i, instructions, store_array, fn_status);
      break;
    case STORE_FAST: // 7d store
      i = push_w(opc, i, instructions, store_array, fn_status);
      break;
    }
  }
}

int push_w(int ins, int pos, int * instructions, int * constants, int fn_status) {
  if (ins == LOAD_CONST) { // 64 
    if (instructions[pos + 3] == MAKE_FUNCTION) { // 84 make function
      push_stack(instructions[pos + 1]); //push position of function to stack
      //pos = pos+5;
    } else {
      if (fn_status == 0) {
        push_stack(constants[instructions[pos + 1] - no_of_funs]);
        pos = pos + 2;
      } else if (fn_status == 1) {
        push_stack(fun[function_position].local_constants[instructions[pos + 1] - len_const_array]);
        pos = pos + 2;
      }
    }
  } else if (ins == CALL_FUNCTION) { // call function
    int l;
    int temp = pos, args_length = 0, hop = 0, which_function;
    onethirtyone = pos;
    pos = pos + 1;
    args_length = instructions[pos]; // first argument of function call showing taking how many argumnts
    hop = (args_length + 1);
    while (hop > 0) {
      pos = pos - 3;
      push_stack(store_array[instructions[pos]]);
      hop--;
    }
    which_function = pop();
    function_position = which_function;
    pos = run_func(which_function, onethirtyone, instructions, store_array, args_length);

  } else if (ins == LOAD_FAST) { // 7c push refrence of local_varnames to stack

    push_stack(const_array[instructions[pos + 1]]);
    pos = pos + 2;

  } else if (ins == STORE_FAST) {
    int next_pos = pos + 1;
    int val = instructions[next_pos];
    int top = pop();
    const_array[val] = top;
    pos = pos + 2;
  }
  return pos;

}

int pop_w(int ins, int pos, int * instructions, int * constants) {

  if (ins == STORE_NAME) { // store array
    int val = 0, next;
    val = pop();
    next = instructions[pos + 1];
    store_array[next] = val;
    pos = pos + 2;

  } else if (ins == LOAD_NAME) { // 65 load 
    int val = 0, next;
    next = instructions[pos + 1];
    push_stack(store_array[next]);
    pos = pos + 2;

  } else if (ins == BINARY_MODULO) { // binary modulo %
    int bottom = 0, top = 0;
    top = pop();
    bottom = pop();
    push_stack(modulo(bottom, top));

  } else if (ins == BINARY_SUM) { // 17 sum
    int bottom = 0, top = 0;
    top = pop();
    bottom = pop();
    push_stack(sum(top, bottom));

  } else if (ins == BINARY_SUB) { // 18 sub;
    int bottom = 0, top = 0;
    top = pop();
    bottom = pop();
    push_stack(sub(bottom, top));

  } else if (ins == BINARY_MULTIPLY) { // 14 mul
    int bottom = 0, top = 0;
    top = pop();
    bottom = pop();
    push_stack(mul(top, bottom));

  } else if (ins == BINARY_DIVIDE) { // 15 div
    int top = 0, bottom = 0;
    top = pop();
    bottom = pop();
    push_stack(divi(bottom, top));
  }
  return pos;
}

//other fns

int condition_if(int pos, int * instructions, int * constants, int fn_status) {

  int operator, top = 0, bottom = 0, pos1;
  operator = instructions[pos + 1];
  top = pop();
  bottom = pop(); //first
  switch (operator) {
  case 0: //<
    if (bottom < top) {

      pos = pos + 2;
    } else {

      if (instructions[pos - 9] == SETUP_LOOP) { // 78 while
        int hop = instructions[pos - 8];
        return hop + pos;
      } else {
        pos = instructions[pos + 4] - 1; // -1 because function body indexing is from 0
      }
    }

    break;
  case 4: // >

    if (bottom > top) {
      pos = pos + 2;
    } else {

      if (instructions[pos - 9] == SETUP_LOOP) { // 78 while
        int hop = instructions[pos - 8];
        if (fn_status == 1)
          return hop + (pos - 9);
        else
          return hop + (pos - 9);
      } else {
        pos = instructions[pos + 4] - 1;
      }

    }
    break;
  case 1: //<=
    if (bottom <= top) {
      pos = pos + 2;
    } else {
      if (instructions[pos - 9] == SETUP_LOOP) { // 78 while
        int hop = instructions[pos - 8];
        return hop + pos;
      } else {
        pos = instructions[pos + 4] - 1;
      }
    }
    break;
  case 2: // ==
    if (bottom == top) {
      pos = pos + 2;
    } else {
      if (instructions[pos - 9] == SETUP_LOOP) { // 78 while
        int hop = instructions[pos - 8];
        return hop + pos;
      } else {
        pos = instructions[pos + 4] - 1;
      }

    }
    break;
  case 3: // !=
    if (bottom != top) {
      pos = pos + 2;
    } else {
      if (instructions[pos - 9] == SETUP_LOOP) { // 78 while
        int hop = instructions[pos - 8];
        return hop + pos;
      } else {
        pos = instructions[pos + 4] - 1;
      }
    }
    break;
  case 5: //>=
    if (bottom >= top) {
      pos = pos + 2;
    } else {
      if (instructions[pos - 9] == SETUP_LOOP) { // 78 while
        int hop = instructions[pos - 8];
        return hop + pos;
      } else {
        pos = instructions[pos + 4] - 1;
      }
    }
    break;
  default:
    printf("in defaut\n");
  }
  return pos;
}

int while_loop(int pos, int * instructions, int * constants, int status) {

  int next;
  next = instructions[pos + 1];
  pos = next;
  return pos - 1;
}

int run_func(int fun_pos, int old_pos, int * instructions, int * store_array, int no_of_args) {

  int len_of_fn = fun[fun_pos].ln_of_fn_body, in_function;

  int l = 0;
  for (l = 0; l < len_of_fn - 1; l++) {
    fn_instructions[l] = fun[fun_pos].fn_body[l];
  }
  in_function = 1;
  interpreter_loop(fn_instructions, const_array, len_of_fn, in_function);

  return onethirtyone + 3; // here 2 is the no of arguments for opcode 83 //131 // function call
}
