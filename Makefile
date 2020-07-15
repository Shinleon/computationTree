
# The compiler: gcc for C programs
CC = gcc

# Compiler flags:
# -Wall for debugger warnings
# -std=c11 for updated standards
CFLAGS = -Wall -std=c11
ifdef DEBUG
	CFLAGS += -g
endif

# The name of the program that we are producing.
TARGET = interactible

# When we just run "make", what gets built? 
# This is a "phony" target
# that just tells make what other targets to build.
all: $(TARGET)

# All the .o files we need for our executable.
OBJS = charnode.o wordnode.o environmentTree.o compNodeUtils.o computer.o charToCompTranslation.o parserUtils.o interface.o

# An option for making a debug target
debug: CFLAGS += -g
debug: $(TARGET)

# The executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o interactible $(OBJS) -lm

# Individual source files
charnode.o: charnode.c constants.h
	$(CC) $(CFLAGS) -c charnode.c

wordnode.o: wordnode.c
	$(CC) $(CFLAGS) -c wordnode.c

environmentTree.o: environmentTree.c compNodeUtils.o compNodeDef.h
	$(CC) $(CFLAGS) -c environmentTree.c 

compNodeUtils.o: compNodeUtils.c compNodeDef.h wordnode.o charnode.o constants.h
	$(CC) $(CFLAGS) -c compNodeUtils.c

# only one that uses math.h so only one that needs -lm
computer.o: computer.c compNodeUtils.o compNodeDef.h wordnode.o charnode.o environmentTree.o constants.h
	$(CC) $(CFLAGS) -c computer.c -lm 

charToCompTranslation.o: charToCompTranslation.c compNodeUtils.o charnode.o compNodeDef.h constants.h
	$(CC) $(CFLAGS) -c charToCompTranslation.c

parserUtils.o: parserUtils.c parserDef.h compNodeUtils.o charToCompTranslation.o compNodeDef.h constants.h 
	$(CC) $(CFLAGS) -c parserUtils.c 

interface.o: interface.c  parserUtils.o compNodeUtils.o charnode.o environmentTree.o computer.o
	$(CC) $(CFLAGS) -c interface.c


# A "phony" target to remove built files
clean:
	rm -f *.o $(TARGET)
