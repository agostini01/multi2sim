lib_LIBRARIES = libmhandle.a
libmhandle_a_SOURCES = mhandle.c mhandle.h
AM_CFLAGS = -Wall

if ENABLE_VPI
AM_CFLAGS += -fPIC
AM_LDFLAGS = -rdynamic -Bsymbolic
endif

