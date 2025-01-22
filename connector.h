#ifndef CONNECTOR_H_INCLUDED
#define CONNECTOR_H_INCLUDED
int calculator();
void menu_func();
int graph();
char** string_to_array(const char* str, int* array_size);
double eval_expr(char** tokens, int array_size);
void replace_substring(char* str, const char* old_substr, const char* new_substr);
#endif // CONNECTOR_H_INCLUDED
