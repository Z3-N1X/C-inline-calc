#include <stdio.h>
#include <conio.h>
#include "connector.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define and &
#define or |


#define ARROW 224
#define ARROW_UP 72
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define ARROW_DOWN 80
#define ENTER 13
#define ESC 27

int position = 0;

void render()
{
	system("cls");
	printf(WHT "  ___    __    __    ___  __  __  __      __   ____  _____  ____ \n"     RESET);
	printf(CYN " / __)  /__\\  (  )  / __)(  )(  )(  )    /__\\ (_  _)(  _  )(  _ \\ \n"   RESET);
	printf(MAG "( (__  /(__)\\  )(__( (__  )(__)(  )(__  /(__)\\  )(   )(_)(  )   /\n"  RESET);
	printf(BLU " \\___)(__)(__)(____)\\___)(______)(____)(__)(__)(__) (_____)(_)\\_)\n"    RESET);
	printf("\n\n\n\n");

	char* a = "", b = "", c = "";
	if (position == 0)
	{
		printf(RED "       - Start\n" RESET);
		printf("      - Graph\n");
		printf("      - Setting\n");
		printf("      - Exit\n");
	}

	if (position == 1)
	{
		printf("      - Start\n");
		printf(RED"       - Graph\n"RESET);
		printf("      - Setting\n");
		printf("      - Exit\n");
	}


	if (position == 2)
	{
		printf("      - Start\n");
		printf("      - Graph\n");
		printf(RED"       - Setting\n"RESET);
		printf("      - Exit\n");
	}
	if (position == 3)
	{
		printf("      - Start\n");
		printf("      - Graph\n");
		printf("      - Setting\n");
		printf(RED"       - Exit\n"RESET);
	}
}


void setting()
{
    int angle_mode;
    FILE* file = fopen("settings.txt", "r");
    if (file == NULL) {
        angle_mode = 1;
        FILE* file2 = fopen("settings.txt", "a");
        fprintf(file2, "1");
        fclose(file2);

    } else{
    fscanf(file, "%d", &angle_mode);
    fclose(file);}
    printf(WHT "  ___    __    __    ___  __  __  __      __   ____  _____  ____ \n"     RESET);
	printf(CYN " / __)  /__\\  (  )  / __)(  )(  )(  )    /__\\ (_  _)(  _  )(  _ \\ \n"   RESET);
	printf(MAG "( (__  /(__)\\  )(__( (__  )(__)(  )(__  /(__)\\  )(   )(_)(  )   /\n"  RESET);
	printf(BLU " \\___)(__)(__)(____)\\___)(______)(____)(__)(__)(__) (_____)(_)\\_)\n"    RESET);
	printf("\n\n\n\n");
    printf(GRN "Mode: %s\nPress Enter to change\nPress Esc to exit" RESET, angle_mode ? "deg": "rad");
    while(1)
    {
        char inp = (int)getch();
        if (inp == ENTER)
        {
            FILE* file2 = fopen("settings.txt", "w");
            fprintf(file2, angle_mode ? "0" : "1");
            angle_mode = angle_mode ? 0 : 1;
            fclose(file2);
            system("cls");
            printf(WHT "  ___    __    __    ___  __  __  __      __   ____  _____  ____ \n"     RESET);
            printf(CYN " / __)  /__\\  (  )  / __)(  )(  )(  )    /__\\ (_  _)(  _  )(  _ \\ \n"   RESET);
            printf(MAG "( (__  /(__)\\  )(__( (__  )(__)(  )(__  /(__)\\  )(   )(_)(  )   /\n"  RESET);
            printf(BLU " \\___)(__)(__)(____)\\___)(______)(____)(__)(__)(__) (_____)(_)\\_)\n"    RESET);
            printf("\n\n\n\n");
            printf(GRN "Mode: %s\nPress Enter to change\nPress Esc to exit" RESET, angle_mode ? "deg": "rad");
        } if (inp == ESC)
        {
            menu_func();
        }
    }

}

void menu_func()
{
    render();
	while (1) {
		int x = (int)_getch();

		if (x == ARROW_UP and position > 0) position -= 1;
		if (x == ARROW_DOWN and position < 3) position += 1;
		if (x == ENTER)
        {
            if (position == 0)
            {
                system("cls");
                calculator();
            }
            if (position == 1)
            {
                system("cls");
                graph();
            }
            if (position == 2)
            {
                system("cls");
                setting();
            }
            if (position == 3)
            {
                return 0;
            }

        }

		render();

	}
}


int main() {



	menu_func();

	return 0;
}
