#include "terminal.h"
#include "shell.h"

int main() {
    init_terminal();
    load_cterm_scripts();
    print_banner();
    shell_loop();
    cleanup_terminal();
    return 0;
}
