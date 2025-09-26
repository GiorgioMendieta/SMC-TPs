#include "stdio.h"

__attribute__((constructor)) void main()
{
    tty_printf("On rentre dans main de base");
    char byte;
    timer_set_period(500000);
    timer_set_mode(0x1);

    tty_puts("Veuillez taper 'a' pour activer les interruptions timer, 'd' pour les désactiver et 'q' pour quitter le programme \n");
    while(1){
        tty_getc_irq(&byte);
        
        if(byte == 'a'){
            tty_puts("Activation des interruptions timer\n");
            timer_set_mode(0x3);
        }
        if(byte == 'd'){
            tty_puts("Désactivation des interruptions timer\n");
            timer_set_mode(0x1);
        }
        if(byte == 'q'){
            tty_puts("Sortie du programme, ciao ciao\n");
            timer_set_mode(0x0);
            exit();
        }
    }

    exit();
} // end main
