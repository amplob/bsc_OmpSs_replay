
export EXTRAE_HOME=/home/bsc/bsc/tools/extrae-install
export EXTRAE_CONFIG_FILE=../extrae.xml
# export LD_PRELOAD=${EXTRAE_HOME}/lib/libseqtrace.so:/home/bsc/bsc/tools/OmpSs_replay/install/lib/libmpissTrace.so

SIMULATION_HOME=/home/bsc/bsc/OmpSs_Simulation/install

DEFAULTCC=$(SIMULATION_HOME)/mcxx-default/bin/mcc
GITCC=$(SIMULATION_HOME)/mcxx-git/bin/mcc

MPISSTRACE_HOME=/home/bsc/bsc/tools/OmpSs_replay/install

CFLAGS = -O -g -I$(EXTRAE_HOME)/include
LFLAGS = -L$(EXTRAE_HOME)/lib -lseqtrace -L$(MPISSTRACE_HOME)/lib -lmpissTrace -Wl,-rpath=$(EXTRAE_HOME)/lib -Wl,-rpath=$(MPISSTRACE_HOME)/lib
# LFLAGS = 

all:
	$(GITCC) $(CFLAGS) test_commutative2.c -o executable.exe -K --ompss --v $(LFLAGS)
# 	$(DEFAULTCC) $(CFLAGS) test_commutative2.c -o executable.exe -K --ompss --v $(LFLAGS)

clean:
	rm *.row *.prv *.pcf *.o *.exe
