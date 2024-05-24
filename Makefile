CC := gcc
LD := gcc
CCFLAGS := -c
LDFLAGS := 


# libraries and headers vor Vulkan
INCLUDE += $(VULKAN_SDK)/include
LIBPATHS += $(VULKAN_SDK)/lib
LIBRARIES += vulkan


# add libraries and headers to flags
CCFLAGS += $(foreach dir,$(INCLUDE),-I$(dir))
LDFLAGS += $(foreach dir,$(INCLUDE),-I$(dir)) 
LDFLAGS += $(foreach dir,$(LIBPATHS),-L$(dir))
LDFLAGS += $(foreach lib,$(LIBRARIES),-l$(lib))


# libraries for GLFW 3
LDFLAGS += $(shell pkg-config --static --libs glfw3)


# source and build directory
SRCDIR := ./src

ifndef OBJDIR
	OBJDIR := ./obj
endif

ifndef PROG
	PROG := ./a.out
endif


# lists of source and object files
SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))


# targets
.PHONY: default verbose debug release clean

default: debug

verbose: CCFLAGS += -v
verbose: LDFLAGS += -v
verbose: debug

debug: CCFLAGS += -DDEBUG -g
debug: $(PROG)

release: $(PROG)

$(PROG): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ $< $(CCFLAGS)

clean:
	@rm $(OBJ) $(PROG) 2> /dev/null; true
