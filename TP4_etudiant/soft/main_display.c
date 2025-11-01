#include "stdio.h"

__attribute__((constructor)) void main()
{
    unsigned char tab[128][128]; // Image buffer
    int index_block = 0;
    char c;
    tty_puts("Bienvenue dans le main display\n");

    while (1)
    {
        // Wait for a character input (blocking)
        tty_getc_irq(&c);
        if (c > 0)
        {
            if (ioc_read(index_block, &tab, 32) != 0)
            {
                tty_puts("Error during ioc read\n");
            }
            if (ioc_completed() != 0)
            {
                tty_puts("Error during ioc transfer\n");
            }

            if (fb_write(0, &tab, 16384) != 0)
            {
                tty_puts("Error during the fbf write\n");
            }
        }
    }

    exit();
} // end main
