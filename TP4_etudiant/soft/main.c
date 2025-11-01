#include "stdio.h"

__attribute__((constructor)) void main()
{
    char c;
    const int TIMER_PERIOD = 500000; // Timer period in microseconds
    timer_set_period(TIMER_PERIOD);

    tty_puts("- a : activation des interruptions générées par le TIMER\n");
    tty_puts("- d : desactivation des interruptions générées par le TIMER\n");
    tty_puts("- q : sortie de l'interprêteur par l'appel système `exit()`\n");
    
    while (1)
    {
        tty_getc_irq(&c); // Wait for a character input (blocking)

        if (c == 'd'){
	    tty_puts("Désactivation des interruptions\n");
            timer_set_mode(0x0); // Deactivate timer
	}
	if (c == 'a'){
	    tty_puts("Activation des interruptions\n");
            timer_set_mode(0x3); // Activate timer with periodic interrupts
        }
	if (c == 'q'){
	    tty_puts("Sortie du programme\n");
            timer_set_mode(0x0); // Deactivate timer
            exit();
	}
    }

    exit();
} // end main

