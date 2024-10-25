#include "stdio.h"

__attribute__((constructor)) void main_display()
{
    unsigned char tab[128][128];
    int index_block = 0;
    int img_nb = 0;
    char byte;
    tty_puts("Bienvenue dans le main display\n");

    while(1){
        tty_getc_irq(&byte);
        if(img_nb == 21){
            index_block=0;
            img_nb = 0;
        }
        if(byte>0){
            if(ioc_read(index_block,&tab,32)){
                tty_puts("Error during ioc read\n");
            }
            if(ioc_completed()){
                tty_puts("Error during ioc transfert\n");
            }

            if(fb_write(0,&tab,16384)){
                tty_puts("Erro during the display of the picture\n");
            }
            index_block+=32;
            ++img_nb;
        }
    }




    exit();
} // end main
