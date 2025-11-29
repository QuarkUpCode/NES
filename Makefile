NAME := nes


CC := gcc

CFLAGS := -Iinclude/ -Isrc/
LIBS := 

SFILES := c
OFILES := o

SRCDIR := src/
OBJDIR := build/

SOURCES := $(shell find $(SRCDIR) -name "*.$(SFILES)")
OBJECTS := $(patsubst $(SRCDIR)%.$(SFILES), $(OBJDIR)%.$(OFILES), $(SOURCES))

OBJSUBDIRS := $(OBJDIR)/lib/quarklib/qprint/

all: directories $(NAME)

directories:
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJSUBDIRS)

$(NAME): $(OBJECTS)
	echo $(SOURCES)
	$(CC) $^ $(LIBS) -o $@

$(OBJDIR)%$(OFILES): $(SRCDIR)%$(SFILES)
# $(OBJECTS): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	rm $(NAME)
