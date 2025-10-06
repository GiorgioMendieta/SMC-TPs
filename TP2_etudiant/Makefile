CC=g++
CFLAGS=-Wno-deprecated -fpermissive
CTOOLS=-std=gnu++0x -I. -I /users/outil/dsx/cctools/include -m32 -c
FASTTOOLS=-std=gnu++0x -I. -I /users/outil/dsx/systemcass/include -m32 -c

all: simple_simulator.x
	
address_decoding_table.o :
	$(CC) $(CFLAGS) $(CTOOLS) address_decoding_table.cpp

address_masking_table.o :
	$(CC) $(CFLAGS) $(CTOOLS) address_masking_table.cpp

mapping_table.o :
	$(CC) $(CFLAGS) $(CTOOLS) mapping_table.cpp

segment.o :
	$(CC) $(CFLAGS) $(CTOOLS) segment.cpp

vci_gcd_coprocessor.o :
	$(CC) $(CFLAGS) $(CTOOLS) vci_gcd_coprocessor.cpp -o fifo_gcd_coprocessor.o

vci_gcd_master.o :
	$(CC) $(CFLAGS) $(CTOOLS) vci_gcd_master.cpp

vci_vgsb.o:
	$(CC) $(CFLAGS) $(CTOOLS) vci_vgsb.cpp

tp2_simple_top.o :
	$(CC) $(CFLAGS) $(CTOOLS) tp2_simple_top.cpp -o tp2_simple_top.o

simple_simulator.x : address_decoding_table.o address_masking_table.o mapping_table.o segment.o vci_vgsb.o vci_gcd_master.o vci_gcd_coprocessor.o tp2_simple_top.o 
	$(CC) $(CFLAGS) -m32 -L. -L/users/outil/dsx/cctools/lib-linux -o simple_simulator.x address_decoding_table.o address_masking_table.o mapping_table.o segment.o vci_vgsb.o vci_gcd_master.o vci_gcd_coprocessor.o tp2_simple_top.o -lsystemc 2>&1

# simulator.x : fifo_gcd_master.o fifo_gcd_coprocessor.o tp1_top.o 
# 	$(CC) $(CFLAGS) -m32 -L. -L/users/outil/dsx/cctools/lib-linux -o simulator.x fifo_gcd_master.o fifo_gcd_coprocessor.o tp1_top.o -lsystemc 2>&1 
	
# fast_fifo_gcd_master.o :
# 	$(CC) $(CFLAGS) $(FASTTOOLS) fifo_gcd_master.cpp -o fast_fifo_gcd_master.o

# fast_simulator.x : fast_fifo_gcd_master.o fast_fifo_gcd_coprocessor.o fast_tp1_top.o 
# 	$(CC) $(CFLAGS) -m32 -L. -L/users/outil/dsx/systemcass/lib -o fast_simulator.x fast_fifo_gcd_master.o fast_fifo_gcd_coprocessor.o fast_tp1_top.o -lsystemc -ldl 2>&1 

clean :
	rm -f *.o 
	rm -f *.x
	