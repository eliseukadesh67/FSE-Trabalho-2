CC = gcc
LDFLAGS = -lwiringPi -lpthread
BLDDIR = .
INCDIR = $(BLDDIR)/inc
SRCDIR = $(BLDDIR)/src
OBJDIR = $(BLDDIR)/obj
BINDIR = $(BLDDIR)/bin
CFLAGS = -c -Wall -I$(INCDIR)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXE = $(BINDIR)/trabalho-2
DEBUG_EXE = $(BINDIR)/trabalho-2-debug

all: clean $(EXE)

debug: clean $(DEBUG_EXE)
    
$(EXE): $(OBJ) 
	$(CC) $(OBJDIR)/*.o -o $@ $(LDFLAGS)

$(DEBUG_EXE): $(OBJ)
	$(CC) $(OBJDIR)/*.o -o $@ $(LDFLAGS) -DDEBUG

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/**

run: 
	./$(EXE)

run_debug:
	./$(DEBUG_EXE)
