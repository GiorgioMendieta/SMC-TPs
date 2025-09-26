CC=g++
CFLAGS=-Wno-deprecated -fpermissive -std=gnu++0x -I. -I/users/outil/dsx/cctools/include -m32 -c
CFLAGS2=-Wno-deprecated -fpermissive -m32 -L. -L/users/outil/dsx/cctools/lib-linux
SRC=fifo_gcd_coprocessor
 
all: fifo_gcd_master.o fifo_gcd_coprocessor.o tp1_top.o
	$(CC) $(CFLAGS) tp1_top.cpp
	$(CC) $(CFLAGS2) -o simulator.x fifo_gcd_master.o fifo_gcd_coprocessor.o tp1_top.o -lsystemc 2>&1 

clean:
    rm $(SRC).o
	rm simulator.x

