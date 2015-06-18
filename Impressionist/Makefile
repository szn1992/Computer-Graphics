#
#  Makefile for fltk applications
#

#set local path to fltk
LOCAL = /cse/courses/cse557/local

INCLUDE = -I$(LOCAL)/include
LIBDIR = -L$(LOCAL)/lib -L/usr/X11R6/lib

LIBS = -lfltk -lfltk_gl -lXext -lX11 -lm -lGL -lGLU

CFLAGS = -g

CC = g++

.SUFFIXES: .o .cpp

.cpp.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

ALL.O = imageio.o impBrush.o \
        impressionist.o FltDesignUI.o \
        impressionistDoc.o impressionistUI.o \
	originalView.o paintView.o pointBrush.o

impressionist: $(ALL.O)
	$(CC) $(CFLAGS) -o $@ $(ALL.O) $(INCLUDE) $(LIBDIR) $(LIBS)
