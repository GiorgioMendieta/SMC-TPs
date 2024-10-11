#include "stdio.h"

__attribute__((constructor)) void  main()
{
	char  byte;
	int iteration = 0;
	int cycle = 0;
	int opa, opb;
	unsigned int res;
	unsigned int status;
	while(1) 
    {
		iteration++;
		cycle = proctime();
		opa = rand() + 1;
		opb = rand() + 1;
		tty_printf("-----------------------\n");
		tty_printf("Numéro de cycle: %d\n", cycle);
		tty_printf("Numéro d'itération: %d\n", iteration);
		tty_printf("Opérande A: %d\n", opa);
		tty_printf("Opérande B: %d\n", opb);
		gcd_set_opa(opa);
		gcd_set_opb(opb);
		if(gcd_start()){
			tty_printf("GCD Error\n");
		};
		gcd_get_status(&status);
		while(status){
			tty_printf("Calculating\n");
			gcd_get_status(&status);
		};
		gcd_get_result(&res);
		cycle = proctime();
		tty_printf("Numéro de cycle fin: %d\n", cycle);
		tty_printf("Numéro d'itération fin: %d\n", iteration);
		
		tty_printf("res: %d\n", res);
		tty_printf("\n\n");
		tty_getc(&byte);
	}

} // end main
