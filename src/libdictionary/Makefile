CC = /usr/local/bin/gcc-9
AR = /usr/local/bin/gcc-ar-9
CFLAGS = -g
LDFLAGS = -L.
LIBS = -ldictionary

all:
	$(CC) -c hsearch_r.c $(CFLAGS) -o hsearch_r.o
	$(CC) -c dictionary.c $(CFLAGS) -o dictionary.o 
	$(AR) rcs libdictionary.a dictionary.o hsearch_r.o
	$(CC) test.c $(LDFLAGS) $(LIBS)
