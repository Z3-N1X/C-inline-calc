#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "connector.h"
#define WIDTH 80
#define HEIGHT 20

#define STEP 0.1

double myFunction(double x);
int mapToGrid(double value, double min, double max, int size);
char input_u[100];
int X_MIN;
int X_MAX;
int graph(){

    while(1)
    {


    printf("Enter a mathematical expression (or 'exit' to quit): ");
    scanf(" %[^\n]", input_u);
    if (strcmp(input_u, "exit")==0) menu_func();
    printf("Enter min and max (ex. -5 5): ");
    scanf("%d %d", &X_MIN, &X_MAX);

    char graph[HEIGHT][WIDTH];
    double yMin = DBL_MAX, yMax = -DBL_MAX;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            graph[i][j] = ' ';
        }
    }
    for (double x = X_MIN; x <= X_MAX; x += STEP)
    {
        double y = myFunction(x);
        if (!isnan(y) && !isinf(y))
        {
            if (y < yMin) yMin = y;
            if (y > yMax) yMax = y;
        }
    }
    if (yMin > 0) yMin = 0;
    if (yMax < 0) yMax = 0;
    for (double x = X_MIN; x <= X_MAX; x += STEP)
    {
        double y = myFunction(x);
        if (!isnan(y) && !isinf(y))
        {
            int xIndex = mapToGrid(x, X_MIN, X_MAX, WIDTH);
            int yIndex = mapToGrid(y, yMin, yMax, HEIGHT);
            if (xIndex >= 0 && xIndex < WIDTH && yIndex >= 0 && yIndex < HEIGHT)
            {
                graph[HEIGHT - yIndex - 1][xIndex] = '*';
            }
        }
    }
    int xAxis = mapToGrid(0, yMin, yMax, HEIGHT);
    int yAxis = mapToGrid(0, X_MIN, X_MAX, WIDTH);
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("%c", graph[i][j]);
        }
        printf("\n");
    }
    }
    return 0;
}
double myFunction(double x)
{

        char* input[40];
        strcpy(input, input_u);
        // Replace constants
        replace_substring(input, " ", "");
        char x_str[40];
        sprintf(x_str,"(%f)", x);
        replace_substring(x_str, "-", "0_");
        replace_substring(input, "x", x_str);
        replace_substring(input, "_", "-");

        int array_size = 0;
        char** result = string_to_array(input, &array_size);
        double result_value = eval_expr(result, array_size);

        free(result);

        return result_value;
}
int mapToGrid(double value, double min, double max, int size)
{
    if (value < min) return 0;
    if (value > max) return size - 1;
    return (int)((value - min) * (size - 1) / (max - min));
}
