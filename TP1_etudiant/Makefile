CC=g++
CFLAGS=-Wno-deprecated -fpermissive
CCTOOLS=/users/outil/dsx/cctools/include 
CCTOOLS_FAST=/users/outil/dsx/systemcass/include

all: simulator fast_simulator

fifo_gcd_master:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c fifo_gcd_master.cpp

fifo_gcd_coprocessor:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c fifo_gcd_coprocessor.cpp

tp1_top:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS} -m32 -c tp1_top.cpp

simulator: fifo_gcd_master fifo_gcd_coprocessor tp1_top
	${CC} ${CFLAGS} -m32 -L. -L/users/outil/dsx/cctools/lib-linux -o simulator.x fifo_gcd_master.o fifo_gcd_coprocessor.o tp1_top.o -lsystemc 2>&1

fast_fifo_gcd_master:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS_FAST} -m32 -c fifo_gcd_master.cpp -o fast_fifo_gcd_master.o

fast_fifo_gcd_coprocessor:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS_FAST} -m32 -c fifo_gcd_coprocessor.cpp -o fast_fifo_gcd_coprocessor.o

fast_tp1_top:
	${CC} ${CFLAGS} -std=gnu++0x -I. -I${CCTOOLS_FAST} -m32 -c tp1_top.cpp -o fast_tp1_top.o

fast_simulator: fast_fifo_gcd_master fast_fifo_gcd_coprocessor fast_tp1_top
	${CC} ${CFLAGS} -m32 -L. -L/users/outil/dsx/systemcass/lib -o fast_simulator.x fast_fifo_gcd_master.o fast_fifo_gcd_coprocessor.o fast_tp1_top.o -lsystemc -ldl 2>&1

clean:
	rm *.o simulator.x fast_simulator.x
