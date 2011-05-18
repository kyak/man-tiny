
PROJECT = man
#DEBUG = -g

CC = gcc

ifndef CFLAGS
CFLAGS = -Os -Wall $(DEBUG) -DDEBUG
endif

ifndef STRIP
STRIP=strip
endif

PARTS = main.o

.c.o:
	$(RM) $@
#       $(CC) -MM $(CFLAGS) $*.c
	$(CC) -c $(CFLAGS) $*.c

.SILENT:

# Targets...

all: $(PROJECT)

$(PROJECT): $(PARTS)
	$(CC) $(LDFLAGS) $(LIBS) $(DEBUG) -o $(PROJECT) $(PARTS)
	$(STRIP) $(PROJECT)

clean:
	rm -f *.o $(PROJECT)

