#define SIZE 256
extern int htod(char* hex);
//extern instruction_node *head;



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
