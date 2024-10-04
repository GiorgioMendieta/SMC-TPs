CC=g++
CFLAGS=-Wno-deprecated -fpermissive
CCTOOLS=/users/outil/dsx/cctools/include 
CCTOOLS_FAST=/users/outil/dsx/systemcass/include

all: multi_simulator simple_simulator

vci_gcd_master:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c vci_gcd_master.cpp

vci_gcd_coprocessor:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c vci_gcd_coprocessor.cpp

vci_vgsb:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c vci_vgsb.cpp

segment:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c segment.cpp

mapping_table:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c mapping_table.cpp

address_decoding_table:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c address_decoding_table.cpp

address_masking_table:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c address_masking_table.cpp

tp2_simple_top:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c tp2_simple_top.cpp

tp2_multi_top:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c tp2_multi_top.cpp

simple_simulator: vci_gcd_master vci_gcd_coprocessor tp2_simple_top mapping_table segment address_decoding_table address_masking_table
	${CC} ${CFLAGS} -m32 -L. -L/users/outil/dsx/cctools/lib-linux -o simple_simulator.x vci_gcd_master.o vci_gcd_coprocessor.o tp2_simple_top.o mapping_table.o segment.o address_decoding_table.o address_masking_table.o -lsystemc 2>&1


multi_simulator: vci_gcd_master vci_gcd_coprocessor tp2_multi_top mapping_table segment address_decoding_table address_masking_table vci_vgsb
	${CC} ${CFLAGS} -m32 -L. -L/users/outil/dsx/cctools/lib-linux -o multi_simulator.x vci_vgsb.o vci_gcd_master.o vci_gcd_coprocessor.o tp2_multi_top.o mapping_table.o segment.o address_decoding_table.o address_masking_table.o -lsystemc 2>&1

clean:
	rm *.o simple_simulator.x multi_simulator.x