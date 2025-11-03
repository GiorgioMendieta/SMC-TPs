#include "stdio.h"

__attribute__((constructor)) void main()
{
    char byte;
    int iteration = 0;
    unsigned int opa, opb;
    unsigned int status;
    unsigned int result;

    while (1)
    {
        // 1. Set cycle number and iteration number
        iteration++;
        // 2. Generate random operands
        opa = rand() + 1;
        opb = rand() + 1;
        // 3. Store operand a in GCD coprocessor
        gcd_set_opa(opa);
        // 4. Store operand b in GCD coprocessor
        gcd_set_opb(opb);
        // 5. Start GCD computation
        if(gcd_start())
	{
	    tty_printf("GCD Error\n");
	}
        // 6. Poll GCD status until computation is done
        while(gcd_get_status(&status)){ };
        // 7. Get the result from GCD coprocessor
        gcd_get_result(&result);
        // 8. Print the results
        tty_printf("--------------------\n");
        tty_printf("cycle: %d\n", proctime());
        tty_printf("iteration: %d\n", iteration);
        tty_printf("operand a: %u\n", opa);
        tty_printf("operand b: %u\n", opb);
        tty_printf("GCD result: %u\n\n", result);
        // 9. Wait for a byte from TTY before starting the next iteration
        tty_getc(&byte);
    }

} // end main
