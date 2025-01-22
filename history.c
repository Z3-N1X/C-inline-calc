#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  

#define MAX_HISTORY 100  
#define MAX_INPUT 256  
#define FILENAME "history.txt"  
  
char history[MAX_HISTORY][MAX_INPUT];
int history_count = 0;

// Save input to history and file  
void save_to_history(const char* input) {
    if (history_count < MAX_HISTORY) {
        strncpy(history[history_count], input, MAX_INPUT - 1);
        history[history_count][MAX_INPUT - 1] = '\0';
        history_count++;
        FILE* file = fopen(FILENAME, "a");
        if (file) {
            fprintf(file, "%s\n", input);
            fclose(file);
        }
    }
}

// Load history from file  
void load_history() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) return;

    history_count = 0;
    while (fgets(history[history_count], MAX_INPUT, file) && history_count < MAX_HISTORY) {
        history[history_count][strcspn(history[history_count], "\n")] = '\0';
        history_count++;
    }
    fclose(file);
}

// Recall specific history entry  
void recall_history(int index) {
    if (index < 1 || index > history_count) {
        printf("Invalid history index. Current history has %d entries.\n", history_count);
        return;
    }

    printf("History #%d: %s\n", index, history[index - 1]);
}

// Show all history  
void show_history() {
    if (history_count == 0) {
        printf("No history available.\n");
        return;
    }

    printf("Full History: \n");
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}

int main() {
    // Load existing history  
    load_history();

    char input[MAX_INPUT];
    printf("History Tracker Ready (Type exit to quit)\n");

    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        // Remove newline  
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "exit") == 0) {
            break;
        }
        else if (strcmp(input, "#") == 0) {
            show_history();
            continue;
        }
        else if (input[0] == '#') {
            // Try to recall specific history entry  
            int index = atoi(input + 1);
            if (index > 0) {
                recall_history(index);
                continue;
            }
        }

        // Regular input - save to history  
        save_to_history(input);
    }
    FILE* file = fopen(FILENAME, "w");
    if (!file) 
    return 0;
    fclose(file);

    return 0;
}