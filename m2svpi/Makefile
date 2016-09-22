#Outside Dependency DIRS and Programs
IDIR =/home/agostini/altera_lite/16.0/modelsim_ase/include
MODELSIM=/home/agostini/altera_lite/16.0/modelsim_ase/linuxaloem
MULTI2SIM=/home/agostini/multi2sim/multi2sim/src

#M2S Libs
AESIM=/home/agostini/multi2sim/multi2sim/src/lib/esim/libesim.a
ACPP=/home/agostini/multi2sim/multi2sim/src/lib/cpp/libcpp.a
AMHAN=/home/agostini/multi2sim/multi2sim/src/lib/mhandle/libmhandle.a
AUTIL=/home/agostini/multi2sim/multi2sim/src/lib/util/libutil.a
AM2S=/home/agostini/multi2sim/multi2sim/src/m2s.o
AMEM=/home/agostini/multi2sim/multi2sim/src/memory/libmemory.a
ANET=/home/agostini/multi2sim/multi2sim/src/network/libnetwork.a
AVIS=/home/agostini/multi2sim/multi2sim/src/visual/common/libcommon.a

#ARCHs
AARCHCOMMON=/home/agostini/multi2sim/multi2sim/src/arch/common/libcommon.a
AARCHX86EMU=/home/agostini/multi2sim/multi2sim/src/arch/x86/emulator/libemulator.a
AARCHX86DIS=/home/agostini/multi2sim/multi2sim/src/arch/x86/disassembler/libdisassembler.a
AARCHX86TIM=/home/agostini/multi2sim/multi2sim/src/arch/x86/timing/libtiming.a
AARCHS=$(AARCHCOMMON) $(AARCHX86EMU) $(AARCHX86DIS) $(AARCHX86TIM)

#Compiler options
CC=g++ -w
CFLAGS=-m32 -fPIC -std=c++0x -I$(IDIR) -I$(MULTI2SIM)
CPOSTFLAGS=
LDFLAGS=-m32 -shared -Bsymbolic
LDPOSTFLAGS=-lz
TARGET=vpi_interface.sl
TESTBENCHFILE=module_tb.v
TESTBENCH=top

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

VDIR=
_VPI=$(TESTBENCHFILE)
VPI=$(_VPI)


#--make------------------------------------------------------------------------#
$(ODIR)/%.o: %.cc $(DEPS)
	$(CC) -c -g $(CFLAGS) -o $@ $< $(CPOSTFLAGS)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ -Wl,--start-group $(AESIM) $(ACPP) $(AMHAN) $(AUTIL) $(AMEM) $(ANET) $(AARCHS) $(AVIS) -Wl,--end-group $(LDPOSTFLAGS)

#--make rule-------------------------------------------------------------------#
#.PHONY: m2s
#m2s:
#	$(CC) -c -g $(CFLAGS) -o $(ODIR)/m2s.o $(MULTI2SIM)/m2s.cc
#	make

.PHONY: clean
clean:
	rm -f -r $(ODIR)/*.o *.sl work *~ transcript

.PHONY: load
load:
	$(MODELSIM)/vlib work
	$(MODELSIM)/vlog $(VPI)

.PHONY: run
run:
	$(MODELSIM)/vsim -c -pli $(TARGET) $(TESTBENCH)

.PHONY: all
all:
	make clean
	make
	make load
	make run
