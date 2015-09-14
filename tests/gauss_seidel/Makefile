
FAKECC=/home/bsc/bsc/OmpSs_Simulation/install/
DEFAULTCC=/home/bsc/bsc/OmpSs_Simulation/install/mcxx-default/bin/mcc

GITCC=/home/bsc/bsc/OmpSs_Simulation/install/mcxx-git/bin/mcc

MPISSTRACE_HOME=/home/bsc/bsc/tools/OmpSs_replay/install

CFLAGS = -O -g -I$(EXTRAE_HOME)/include
LFLAGS = -L$(EXTRAE_HOME)/lib -lseqtrace -L$(MPISSTRACE_HOME)/lib -lmpissTrace -Wl,-rpath=$(EXTRAE_HOME)/lib -Wl,-rpath=$(MPISSTRACE_HOME)/lib


all:
	$(GITCC) $(CFLAGS) gauss_seidel.c kernel.c -o executable.exe -K --ompss --v $(LFLAGS)

clean:
	rm *.row *.prv *.pcf *.o *.exe
