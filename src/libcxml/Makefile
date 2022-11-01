CC = /usr/local/bin/gcc-9
CPP = /usr/local/bin/g++-9
AR = /usr/local/bin/gcc-ar-9


all:
	$(CPP) -c libcxml.cpp -o libcxml.o -O0 -lstdc++
	$(AR) rcs libcxml.a libcxml.o 
	$(CC) -c test.c -o test.o
#	$(CC) test.o libcxml.a
	$(CC) test.o -lcxml -L. -lstdc++
