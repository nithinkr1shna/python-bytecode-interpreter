extern int htod(char* hex);
//extern instruction_node *head;


typedef struct instruction{

  int opcode;
  int pos;
  int snd_pos;
  struct instruction* next;
}instruction_node;


