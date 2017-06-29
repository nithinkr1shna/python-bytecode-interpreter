extern int htod(char* hex);
//extern instruction_node *head;


typedef struct instruction{

  struct instruction* prev;
  int sr;
  int opcode;
  int pos;
  int snd_pos;
  struct instruction* next;
}instruction_node;


