#include "stdio.h"

__attribute__((constructor)) void main_display()
{
    unsigned char buf[128][128];
    char user_char;
    unsigned cnt_image = 0;
    tty_puts("Bienvenue dans le main display\n");

    while(1){
        if (ioc_read(cnt_image * 32, buf, 32))
            tty_printf("Error ioc_read\n");

        if (ioc_completed())
            tty_printf("Error ioc_completed\n");

        if (fb_write(0, buf, 16384))
            tty_printf("Error fb_write\n");
        
        if (fb_completed())
            tty_printf("Error fb_completed\n");

        cnt_image++;
        tty_getc_irq(&user_char);
    }

    exit();
} // end main
