##############################################################################
#
#    Makefile for Fixed Point arithmentic test and library
#    (c) 2005-2015 Tim Molteno
#
#

OBJS=Startup.o Fixed.o FixedVector.o FixedMatrix.o Quaternion.o f_int64.o

include makefile.arm

all: clean fixed

clean:
	@ echo "...cleaning"
	rm -f ${OBJS} *.o *.elf	*.hex *.s *.bin *.lst *.lnkh *.lnkt *.dl


arm:	test_arm.dl
	echo "Arm build complete"

###############################################################################
#
#	The following builds the testharness to run on the host computer
#	and defines the IOSTREAMS preprocessor macro so that debugging
#	is turned on and output to the console enabled.
#
#

DEFS=-D IOSTREAMS=1
HOST_CXX=g++
HOST_FLAGS=-g -Wall -Wunused -c ${DEFS}

fixed:	test_fixed.cpp Fixed.cpp Fixed.h FixedVector.cpp FixedMatrix.cpp f_int64.cpp
	${HOST_CXX} ${HOST_FLAGS} -o Fixed.o Fixed.cpp
	${HOST_CXX} ${HOST_FLAGS} -o FixedVector.o FixedVector.cpp
	${HOST_CXX} ${HOST_FLAGS} -o FixedMatrix.o FixedMatrix.cpp
	${HOST_CXX} ${HOST_FLAGS} -o Quaternion.o Quaternion.cpp
	${HOST_CXX} ${HOST_FLAGS} -o f_int64.o f_int64.cpp
	${HOST_CXX} ${HOST_FLAGS} -o test_fixed.o test_fixed.cpp
	${HOST_CXX} -o test_fixed test_fixed.o Fixed.o FixedVector.o FixedMatrix.o Quaternion.o f_int64.o -lstdc++
	./test_fixed
	
