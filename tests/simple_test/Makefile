FAKECC=/home/marc/OmpSs_Simulation/install/mcxx/bin/mcc
DEFAULTCC=/home/marc/OmpSs_Simulation/install/mcxx-default/bin/mcc


CFLAGS = -O -g -I$(EXTRAE_HOME)/include
LFLAGS = -L$(EXTRAE_HOME)/lib -lseqtrace -Wl,-rpath=$(EXTRAE_HOME)/lib


all:
	$(DEFAULTCC) $(CFLAGS) simple_test.c -o $@.exe -K --ompss --v $(LFLAGS)
	#./$@.exe2
	

clean:
	rm *.row *.prv *.pcf *.o *.exe
