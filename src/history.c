#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "history.h"

#define MAX_HISTORY 100
char *history[MAX_HISTORY];
int hist_count = 0;

void add_history(const char *cmd) {
    if (hist_count < MAX_HISTORY) history[hist_count++] = strdup(cmd);
}

void print_history() {
    for (int i = 0; i < hist_count; i++)
        printf("%d: %s\n", i+1, history[i]);
}
