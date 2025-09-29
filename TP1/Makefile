CC=g++
CFLAGS=-Wno-deprecated -fpermissive
CTOOLS=-std=gnu++0x -I. -I/users/outil/dsx/cctools/include -m32 -c
FASTTOOLS=-std=gnu++0x -I. -I/users/outil/dsx/systemcass/include -m32 -c

all: simulator.x fastsimulator.x
	
fifo_gcd_master.o :
	$(CC) $(CFLAGS) $(CTOOLS) fifo_gcd_master.cpp

fifo_gcd_coprocessor.o :
	$(CC) $(CFLAGS) $(CTOOLS)  fifo_gcd_coprocessor.cpp

tp1_top.o :
	$(CC) $(CFLAGS) $(CTOOLS)  tp1_top.cpp

simulator.x : fifo_gcd_master.o fifo_gcd_coprocessor.o tp1_top.o 
	$(CC) $(CFLAGS) -m32 -L. -L/users/outil/dsx/cctools/lib-linux -o simulator.x fifo_gcd_master.o fifo_gcd_coprocessor.o tp1_top.o -lsystemc 2>&1 
	
fast_fifo_gcd_master.o :
	$(CC) $(CFLAGS) $(FASTTOOLS) fifo_gcd_master.cpp -o fast_fifo_gcd_master.o

fast_fifo_gcd_coprocessor.o :
	$(CC) $(CFLAGS) $(FASTTOOLS) fifo_gcd_coprocessor.cpp -o fast_fifo_gcd_coprocessor.o

fast_tp1_top.o :
	$(CC) $(CFLAGS) $(FASTTOOLS)  tp1_top.cpp -o fast_tp1_top.o

fast_simulator.x : fast_fifo_gcd_master.o fast_fifo_gcd_coprocessor.o fast_tp1_top.o 
	$(CC) $(CFLAGS) -m32 -L. -L/users/outil/dsx/systemcass/lib -o simulator.x fifo_gcd_master.o fifo_gcd_coprocessor.o tp1_top.o -lsystemc 2>&1 

clean :
	rm -f *.o 
	rm -f *.x
	