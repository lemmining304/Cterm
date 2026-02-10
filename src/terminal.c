#include <stdio.h>
#include "terminal.h"

void init_terminal() {
    printf("\033[2J\033[H");
}

void cleanup_terminal() {
    printf("\033[0m");
}

void print_banner() {
    printf("\033[1;34m=== Bem-vindo ao Cterm ===\033[0m\n");
}
