#include "stdio.h"

__attribute__((constructor)) void main_proc3()
{
    char user_char;
    tty_puts("Bienvenue dans le main_proc3\n");

    while(1){
        tty_getc_irq(&user_char);
        tty_puts("Je suis Shrek");
    }

    exit();
} // end main