CC=g++
CFLAGS=-Wno-deprecated -fpermissive

all: simulator.x
	

fifo_gcd_master.o :
	$(CC) $(CFLAGS) -std=gnu++0x -I. -I/users/outil/dsx/cctools/include -m32 -c  fifo_gcd_master.cpp

fifo_gcd_coprocessor.o :
	$(CC) $(CFLAGS) -std=gnu++0x -I. -I/users/outil/dsx/cctools/include -m32 -c  fifo_gcd_coprocessor.cpp

tp1_top.o :
	$(CC) $(CFLAGS) -std=gnu++0x -I. -I/users/outil/dsx/cctools/include -m32 -c  tp1_top.cpp

simulator.x : fifo_gcd_master.o fifo_gcd_coprocessor.o tp1_top.o 
	$(CC) $(CFLAGS) -m32 -L. -L/users/outil/dsx/cctools/lib-linux -o simulator.x fifo_gcd_master.o fifo_gcd_coprocessor.o tp1_top.o -lsystemc 2>&1 

clean :
	rm -f *.o 
	rm -f simulator.x
	