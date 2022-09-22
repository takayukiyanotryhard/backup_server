### Makefile ###

CC  	= /usr/local/bin/gcc-9
CFLAGS	= -g -Wall -O2
LDFLAGS = 
LIBS 	=
ODIR	= ./out/objs
EXE 	= server
SRCS	= $(wildcard *.c)
OBJS	= $(addprefix $(ODIR)/, $(SRCS:.c=.o))

all: prepare server

prepare:
	mkdir -p ./out/objs 

server: $(OBJS)
	$(CC) $(LDFLAGS) $(LIBS) -o $(ODIR)/../$(EXE) $^

$(ODIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf ./out