#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include "connector.h"

// Global variable to determine angle mode (0 = radians, 1 = degrees)
int angle_mode = 0;

// Replace a substring with another substring in a string
void replace_substring(char* str, const char* old_substr, const char* new_substr) {
    char* pos;
    int old_substr_length = strlen(old_substr);
    int new_substr_length = strlen(new_substr);
    int length_diff = new_substr_length - old_substr_length;

    while ((pos = strstr(str, old_substr)) != NULL) {
        if (length_diff > 0) {
            memmove(pos + new_substr_length, pos + old_substr_length, strlen(pos + old_substr_length) + 1);
        }
        memcpy(pos, new_substr, new_substr_length);

        if (length_diff < 0) {
            memmove(pos + new_substr_length, pos + old_substr_length, strlen(pos + old_substr_length) + 1);
        }
    }
}

// Convert an input string into an array of tokens
char** string_to_array(const char* str, int* array_size) {
    int length = strlen(str);
    char** array = (char**)malloc(length * sizeof(char*));
    int j = 0;

    for (int i = 0; i < length; i++) {
        if (isdigit(str[i]) || (str[i] == '.' && isdigit(str[i - 1]))) {
            int start = i;
            while (i < length && (isdigit(str[i]) || str[i] == '.')) i++;
            int num_length = i - start;
            array[j] = (char*)malloc((num_length + 1) * sizeof(char));
            strncpy(array[j], &str[start], num_length);
            array[j][num_length] = '\0';
            j++;
            i--;
        } else if (isalpha(str[i])) { // Handle functions like rnd
            int start = i;
            while (i < length && isalpha(str[i])) i++;
            int name_length = i - start;
            array[j] = (char*)malloc((name_length + 1) * sizeof(char));
            strncpy(array[j], &str[start], name_length);
            array[j][name_length] = '\0';
            j++;
            i--;
        } else if (!isspace(str[i])) {
            array[j] = (char*)malloc(2 * sizeof(char));
            array[j][0] = str[i];
            array[j][1] = '\0';
            j++;
        }
    }
    *array_size = j;
    return array;
}

// Factorial function
double factorial(int n) {
    if (n < 0) {
        fprintf(stderr, "Error: Factorial of negative number is not defined.\n");
        exit(EXIT_FAILURE);
    }
    double result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Convert degrees to radians if necessary
double to_radians(double angle) {
    return angle_mode == 1 ? angle * (M_PI / 180.0) : angle;
}

// Evaluate supported functions
double evaluate_function(const char* func_name, double arg1, double arg2) {
    if (strcmp(func_name, "rnd") == 0) return arg1 + (rand() / (RAND_MAX + 1.0)) * (arg2 - arg1);
    if (strcmp(func_name, "pow") == 0) return pow(arg1, arg2);
    if (strcmp(func_name, "ln") == 0) return log(arg1);
    if (strcmp(func_name, "log") == 0) return log10(arg1);
    if (strcmp(func_name, "fact") == 0) return factorial((int)arg1);
    if (strcmp(func_name, "sin") == 0) return sin(to_radians(arg1));
    if (strcmp(func_name, "cos") == 0) return cos(to_radians(arg1));
    if (strcmp(func_name, "tan") == 0) return tan(to_radians(arg1));
    if (strcmp(func_name, "cot") == 0) return 1 / tan(to_radians(arg1));
    if (strcmp(func_name, "asin") == 0) return asin(arg1) * (angle_mode == 1 ? 180.0 / M_PI : 1);
    if (strcmp(func_name, "acos") == 0) return acos(arg1) * (angle_mode == 1 ? 180.0 / M_PI : 1);
    if (strcmp(func_name, "atan") == 0) return atan(arg1) * (angle_mode == 1 ? 180.0 / M_PI : 1);
    if (strcmp(func_name, "acot") == 0) return atan(1 / arg1) * (angle_mode == 1 ? 180.0 / M_PI : 1);
    if (strcmp(func_name, "abs") == 0) return fabs(arg1);
    if (strcmp(func_name, "exp") == 0) return exp(arg1);
    if (strcmp(func_name, "sqrt") == 0) return sqrt(arg1);

    fprintf(stderr, "Unsupported function: %s\n", func_name);
    exit(EXIT_FAILURE);
}

// Evaluate an expression using shunting-yard algorithm with function support
double eval_expr(char** tokens, int array_size) {
    double values[100];
    char operators[100];
    int value_top = -1;
    int operator_top = -1;

    void apply_operator() {
        double b = values[value_top--];
        double a = values[value_top--];
        char op = operators[operator_top--];
        double result;

        if (op == '+') result = a + b;
        else if (op == '-') result = a - b;
        else if (op == '*') result = a * b;
        else if (op == '/') result = a / b;

        values[++value_top] = result;
    }

    for (int i = 0; i < array_size; i++) {
        if (isdigit(tokens[i][0]) || (tokens[i][0] == '.' && isdigit(tokens[i][1]))) {
            values[++value_top] = atof(tokens[i]);
        } else if (isalpha(tokens[i][0])) {
            char* func_name = tokens[i];
            i++;
            if (tokens[i][0] != '(') {
                fprintf(stderr, "Expected '(' after function name: %s\n", func_name);
                exit(EXIT_FAILURE);
            }
            i++;
            double arg1 = atof(tokens[i++]);
            double arg2 = 0;
            if (tokens[i][0] == ',') {
                i++;
                arg2 = atof(tokens[i++]);
            }
            if (tokens[i][0] != ')') {
                fprintf(stderr, "Expected ')' after function arguments: %s\n", func_name);
                exit(EXIT_FAILURE);
            }
            values[++value_top] = evaluate_function(func_name, arg1, arg2);
        } else if (tokens[i][0] == '(') {
            operators[++operator_top] = '(';
        } else if (tokens[i][0] == ')') {
            while (operator_top >= 0 && operators[operator_top] != '(') {
                apply_operator();
            }
            operator_top--;
        } else if (strchr("+-*/", tokens[i][0])) {
            while (operator_top >= 0 && strchr("+-*/", operators[operator_top]) &&
                   ((tokens[i][0] == '+' || tokens[i][0] == '-') ||
                    (tokens[i][0] == '*' || tokens[i][0] == '/'))) {
                apply_operator();
            }
            operators[++operator_top] = tokens[i][0];
        }
    }

    while (operator_top >= 0) {
        apply_operator();
    }

    return values[value_top];
}

// Read settings from file
void read_settings() {
    FILE* file = fopen("settings.txt", "r");
    if (file == NULL) {
        angle_mode = 1;
    }else{
    fscanf(file, "%d", &angle_mode);
    fclose(file);}
}

int calculator() {
    srand(time(NULL));
    read_settings();

    char input[100];

    while (1) {
        printf("Enter a mathematical expression (or 'exit' to quit): ");
        scanf(" %[^\n]", input);

        if (strcmp(input, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        // Replace constants
        replace_substring(input, " ", "");
        replace_substring(input, "pi", "3.14");
        replace_substring(input, "e", "2.71");

        int array_size = 0;
        char** result = string_to_array(input, &array_size);

        double result_value = eval_expr(result, array_size);

        printf("Result: %.6f\n", result_value);

        for (int i = 0; i < array_size; i++) {
            free(result[i]);
        }
        free(result);
    }


}
