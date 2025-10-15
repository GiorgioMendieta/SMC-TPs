CC=g++
CFLAGS=-Wno-deprecated -fpermissive

# Uncomment the following lines depending on your setup

# SESI paths
# SYSTEMCINC=/users/outil/dsx/cctools/include -m32 -c
# SYSTEMCLIB=/users/outil/dsx/cctools/lib-linux -m32 -c
# SYSCASSINC=/users/outil/dsx/systemcass/include -m32 -c
# SYSCASSLIB=/users/outil/dsx/systemcass/lib-linux -m32 -c

# Giorgio's paths for Mac with Apple Silicon
SYSTEMCINC=/opt/homebrew/Cellar/systemc/3.0.1/include
SYSTEMCLIB=/opt/homebrew/Cellar/systemc/3.0.1/lib
SYSCASSINC=/usr/local/include
SYSCASSLIB=/usr/local/lib-macosx

# Wilfrid's paths for Linux with Intel
# SYSTEMCINC=/home/wilfrid/systemc-2.3.3/include
# SYSTEMCLIB=/home/wilfrid/systemc-2.3.3/lib-linux64
# SYSTEMCASSINC=/usr/local/includew
# SYSTEMCASSLIB=/usr/local/lib-linux/

CTOOLS=-std=gnu++17 -I. -I $(SYSTEMCINC) -c
FASTTOOLS=-std=gnu++17 -I. -I $(SYSCASSINC) -c

all: simple_simulator.x multi_simulator.x
	if [ ! -d ./bin ]; then mkdir ./bin; fi
	mv *.o ./bin/
	
address_decoding_table.o :
	$(CC) $(CFLAGS) $(CTOOLS) address_decoding_table.cpp

address_masking_table.o :
	$(CC) $(CFLAGS) $(CTOOLS) address_masking_table.cpp

mapping_table.o :
	$(CC) $(CFLAGS) $(CTOOLS) mapping_table.cpp

segment.o :
	$(CC) $(CFLAGS) $(CTOOLS) segment.cpp

vci_gcd_coprocessor.o :
	$(CC) $(CFLAGS) $(CTOOLS) vci_gcd_coprocessor.cpp -o vci_gcd_coprocessor.o

vci_gcd_master.o :
	$(CC) $(CFLAGS) $(CTOOLS) vci_gcd_master.cpp

vci_vgsb.o:
	$(CC) $(CFLAGS) $(CTOOLS) vci_vgsb.cpp

tp2_simple_top.o :
	$(CC) $(CFLAGS) $(CTOOLS) tp2_simple_top.cpp -o tp2_simple_top.o

tp2_multi_top.o :
	$(CC) $(CFLAGS) $(CTOOLS) tp2_multi_top.cpp -o tp2_multi_top.o

simple_simulator.x : address_decoding_table.o address_masking_table.o mapping_table.o segment.o vci_vgsb.o vci_gcd_master.o vci_gcd_coprocessor.o tp2_simple_top.o 
	$(CC) $(CFLAGS) -L. -L$(SYSTEMCLIB) -o simple_simulator.x address_decoding_table.o address_masking_table.o mapping_table.o segment.o vci_vgsb.o vci_gcd_master.o vci_gcd_coprocessor.o tp2_simple_top.o -lsystemc 2>&1

multi_simulator.x : address_decoding_table.o address_masking_table.o mapping_table.o segment.o vci_vgsb.o vci_gcd_master.o vci_gcd_coprocessor.o tp2_multi_top.o 
	$(CC) $(CFLAGS) -L. -L$(SYSTEMCLIB) -o multi_simulator.x address_decoding_table.o address_masking_table.o mapping_table.o segment.o vci_vgsb.o vci_gcd_master.o vci_gcd_coprocessor.o tp2_multi_top.o -lsystemc 2>&1
clean :
	rm -f *.o 
	rm -f *.x
	