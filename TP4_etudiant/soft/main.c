#include "stdio.h"

__attribute__((constructor)) void main()
{
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
            timer_set_mode(0x2);
        }
        if(byte == 'q'){
            tty_puts("Sortie du programme, ciao ciao\n");
            exit();
        }
    }

    exit();
} // end main
