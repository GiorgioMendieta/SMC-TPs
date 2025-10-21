#include "stdio.h"

__attribute__((constructor)) void main()
{
    char byte;
    int cycle;
    int iteration = 0;

    int seed = 123456789;

    unsigned int opa, opb;
    unsigned int status = 1;
    unsigned int result;

    while (1)
    {
        // 1. Set cycle number and iteration number
        cycle = proctime();
        iteration++;
        // 2. Generate random operands
        opa = rand();
        opb = rand();
        // 3. Store operand a in GCD coprocessor
        gcd_set_opa(opa);
        // 4. Store operand b in GCD coprocessor
        gcd_set_opb(opb);
        // 5. Start GCD computation
        gcd_start();
        // 6. Poll GCD status until computation is done
        status = 1;
        while (status != 0)
        {
            gcd_get_status(&status);
        }
        // 7. Get the result from GCD coprocessor
        gcd_get_result(&result);
        // 8. Print the results
        tty_printf("cycle: %d", cycle);
        tty_printf("iteration: %d", iteration);
        tty_printf("operand a: %u", opa);
        tty_printf("operand b: %u", opb);
        tty_printf("GCD result: %u", result);
        // 9. Wait for a byte from TTY before starting the next iteration
        tty_getc(&byte);
    }

} // end main
