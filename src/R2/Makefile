#
# Makefile for R2 library.
#

CXX=g++
CXXFLAGS=-Wall -I. -g -DUSE_JPEG

ifneq (,$(findstring Darwin,$(shell uname)))
  AR=libtool -static -o
else
  AR=ar -rcs
endif

libR2.a: R2Image.o R2Distance.o R2Line.o R2Point.o R2Segment.o R2Vector.o
	rm -f $@
	$(AR) $@ $^

R2Image.o R2Distance.o R2Line.o R2Point.o R2Segment.o R2Vector.o: R2.h

clean:
	rm -f *.o libR2.a

