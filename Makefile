CC = gcc
LDFLAGS = -lwiringPi
BLDDIR = .
INCDIR = $(BLDDIR)/inc
SRCDIR = $(BLDDIR)/src
OBJDIR = $(BLDDIR)/obj
BINDIR = $(BLDDIR)/bin
CFLAGS = -c -Wall -I$(INCDIR)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXE = $(BINDIR)/trabalho-2

all: clean $(EXE) 
    
$(EXE): $(OBJ) 
	$(CC) $(OBJDIR)/*.o -o $@ $(LDFLAGS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR)/*.o $(EXE)

run: 
	./$(EXE)
