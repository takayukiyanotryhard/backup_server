### Makefile ###

CC  	    = /usr/local/bin/gcc-9
#CFLAGS	    = -g -Wall -O2 -v
CFLAGS	    = -g -Wall -O2
LDFLAGS     = -L../libcxml -L../libdictionary
LIBS 	    = -lcxml -lstdc++ -ldictionary
ODIR	    = ./out/objs
EXE 	    = server
SRCS	    = $(wildcard *.c)
OBJS	    = $(addprefix $(ODIR)/, $(SRCS:.c=.o))
OUTPUT_DIR  = ../bin

all: prepare server
	cp $(ODIR)/../$(EXE) $(OUTPUT_DIR)

prepare:
	mkdir -p ./out/objs 

server: $(OBJS)
	$(CC) -o $(ODIR)/../$(EXE) $(LDFLAGS) $(LIBS) $^

$(ODIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf ./out
	rm -f $(OUTPUT_DIR)/$(EXE)
