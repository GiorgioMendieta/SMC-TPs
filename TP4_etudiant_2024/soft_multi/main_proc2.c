#include "stdio.h"

__attribute__((constructor)) void main_proc2()
{
    char user_char;
    tty_puts("Bienvenue dans le main_proc2\n");

    while(1){
        tty_getc_irq(&user_char);
        tty_puts("Salut Salut");
        
    }

    exit();
} // end main