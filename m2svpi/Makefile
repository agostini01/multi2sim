#Outside Dependency DIRS and Programs
#MODELSIM=/home/agostini/altera_lite/16.0/modelsim_ase/linuxaloem
ifndef MODELSIM
$(error MODELSIM variable not set)
endif
IDIR=$(MODELSIM)/../include
MULTI2SIM=../src

#M2S Libs
AESIM=$(MULTI2SIM)/lib/esim/libesim.a
ACPP=$(MULTI2SIM)/lib/cpp/libcpp.a
AMHAN=$(MULTI2SIM)/lib/mhandle/libmhandle.a
AUTIL=$(MULTI2SIM)/lib/util/libutil.a
AM2S=$(MULTI2SIM)/m2s.o
AMEM=$(MULTI2SIM)/memory/libmemory.a
ANET=$(MULTI2SIM)/network/libnetwork.a
AVIS=$(MULTI2SIM)/visual/common/libcommon.a
ADRAM=$(MULTI2SIM)/dram/libdram.a

#ARCHs
AARCHCOMMON=$(MULTI2SIM)/arch/common/libcommon.a
AARCHX86EMU=$(MULTI2SIM)/arch/x86/emulator/libemulator.a
AARCHX86DIS=$(MULTI2SIM)/arch/x86/disassembler/libdisassembler.a
AARCHX86TIM=$(MULTI2SIM)/arch/x86/timing/libtiming.a
AARCHS=$(AARCHCOMMON) $(AARCHX86EMU) $(AARCHX86DIS) $(AARCHX86TIM)

#Compiler options
CC=g++ -w
CFLAGS=-m32 -fPIC -std=c++0x -I$(IDIR) -I$(MULTI2SIM)
CPOSTFLAGS=
LDFLAGS=-m32 -shared -Bsymbolic
LDPOSTFLAGS=-lz
TARGET=vpi_interface.sl
TESTBENCHFILE=module_tb.v
TESTBENCH=mem_controller_tb

#DDIR=
_DEPS=$(MULTI2SIM)/m2s.h
DEPS = $(_DEPS)

SDIR=
_SRC=vpi_user.cc show_all_signals_2_vpi.cc m2s_vpi.cc
SRC=$(_SRC)

ODIR=obj
_OBJ=vpi_user.o show_all_signals_2_vpi.o m2s_vpi.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ)) $(AM2S)

LDIR=lib
_LIB=
LIB=$(patsubst %,$(LDIR)/%,$(_LIB))

VDIR=vsrc
_VPI=$(VDIR)/$(TESTBENCHFILE) $(VDIR)/fifos.v $(VDIR)/mem_controller.v
VPI=$(_VPI)


#--make------------------------------------------------------------------------#
$(ODIR)/%.o: %.cc $(DEPS)
	$(CC) -c -g $(CFLAGS) -o $@ $< $(CPOSTFLAGS)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ -Wl,--start-group $(AESIM) $(ACPP) $(AMHAN) $(AUTIL) $(AMEM) $(ANET) $(AVIS) $(ADRAM) -Wl,--end-group $(LDPOSTFLAGS)

#--make rule-------------------------------------------------------------------#
#.PHONY: m2s
#m2s:
#	$(CC) -c -g $(CFLAGS) -o $(ODIR)/m2s.o $(MULTI2SIM)/m2s.cc
#	make

.PHONY: clean
clean:
	rm -f -r $(ODIR)/*.o *.sl work *~ transcript *debug-info.txt *report-info.txt  *trace-info.gz

.PHONY: load
load:
	$(MODELSIM)/vlib work
	$(MODELSIM)/vlog $(VPI)

.PHONY: run
run:
	$(MODELSIM)/vsim -c -pli $(TARGET) $(TESTBENCH)

.PHONY: visual
visual:
	$(MODELSIM)/vsim -pli $(TARGET) $(TESTBENCH) -do "waves/wave.do" -do "run 100ns"

.PHONY: all
all:
	make clean
	make
	make load
	make run
