CC := gcc
LD := gcc
CCFLAGS := -c
LDFLAGS := 

INCLUDE += $(VULKAN_SDK)/include
LIBRARIES += $(VULKAN_SDK)/lib
LDFLAGS += -lvulkan

LIBRARIES += $(shell pkg-config --static --libs glfw3)

CCFLAGS += -I$(INCLUDE)
LDFLAGS += -I$(INCLUDE) -L$(LIBRARIES)

SRCDIR := ./src
OBJDIR := ./obj

SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

.PHONY: default debug release clean

default: debug

debug: CCFLAGS += -DDEBUG -g
debug: a.out

release: a.out

a.out: $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ $< $(CCFLAGS)

clean:
	rm $(OBJ) a.out
