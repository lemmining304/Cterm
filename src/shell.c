#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include "shell.h"
#include "history.h"
#include "utils.h"

#define MAX_CMD 512
#define MAX_ALIAS 50

typedef struct {
    char name[64];
    char value[256];
} Alias;

Alias aliases[MAX_ALIAS];
int alias_count = 0;

void add_alias(const char *name, const char *value) {
    if (alias_count < MAX_ALIAS) {
        strncpy(aliases[alias_count].name, name, 63);
        strncpy(aliases[alias_count].value, value, 255);
        alias_count++;
    }
}

char* check_alias(const char *cmd) {
    for (int i = 0; i < alias_count; i++)
        if (strcmp(cmd, aliases[i].name) == 0) return aliases[i].value;
    return NULL;
}

void print_prompt() {
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("\033[1;34m[%s] \033[1;32mCterm> \033[0m", cwd);
}

void execute_command(char *cmd) {
    char *alias_val = check_alias(cmd);
    if (alias_val) cmd = alias_val;

    if (strcmp(cmd, "exit") == 0) exit(0);
    else if (strcmp(cmd, "history") == 0) print_history();
    else if (strncmp(cmd, "alias ", 6) == 0) {
        char name[64], value[256];
        sscanf(cmd + 6, "%63[^=]=%255[^\n]", name, value);
        add_alias(name, value);
    } else {
        if (fork() == 0) {
            execlp("/bin/sh", "sh", "-c", cmd, NULL);
            exit(1);
        } else wait(NULL);  // corrigido com sys/wait.h
    }
}

// Função para carregar scripts .cterm
void load_cterm_scripts() {
    DIR *d;
    struct dirent *dir;
    d = opendir("scripts");
    if (!d) return;
    while ((dir = readdir(d)) != NULL) {
        if (strstr(dir->d_name, ".cterm")) {
            char path[512];
            snprintf(path, sizeof(path), "scripts/%s", dir->d_name);
            FILE *f = fopen(path, "r");
            if (!f) continue;
            char line[512];
            while (fgets(line, sizeof(line), f)) {
                trim_newline(line);
                if (strlen(line) == 0) continue;
                execute_command(line);
            }
            fclose(f);
        }
    }
    closedir(d);
}

void shell_loop() {
    char cmd[MAX_CMD];
    while (1) {
        print_prompt();
        if (!fgets(cmd, MAX_CMD, stdin)) break;
        trim_newline(cmd);
        if (strlen(cmd) == 0) continue;
        add_history(cmd);
        execute_command(cmd);
    }
}
